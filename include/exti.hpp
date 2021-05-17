/*
 * exti.hpp
 *
 *  Created on: 8 мая 2021 г.
 *      Author: kochetkov
 */

#ifndef INCLUDE_EXTI_HPP_
#define INCLUDE_EXTI_HPP_

#include <map>
#include "rcc_config.hpp"
#include "pin_usage.hpp"
#include "template_map.hpp"

namespace exti_traits {
	enum class edge_trigger_t : uint8_t {
		RISING = 0,
		FALLING,
		BOTH
	};

	enum class exti_type_t : uint8_t {
		INTERRUPT = 0,
		EVENT,
		BOTH
	};

	using port_map_keys = template_map::KeyValueContainer<gpio_traits::port_t,
			gpio_traits::port_t::PORTA,
			gpio_traits::port_t::PORTB,
			gpio_traits::port_t::PORTC,
			gpio_traits::port_t::PORTD,
			gpio_traits::port_t::PORTF
			>;

	using port_map_values = template_map::KeyValueContainer<uint8_t,
			0x00,
			0x01,
			0x02,
			0x03,
			0x05
			>;

	constexpr template_map::Map<port_map_keys, port_map_values> port_map;

}

class exti_c {

	template<const uint8_t pin>
	static constexpr uint8_t _calc_cr_pos() noexcept {
		static_assert(0 <= pin && pin <= 15);
		if (pin < 4) {
			return 0;
		} else if (pin < 8) {
			return 1;
		} else if (pin < 12) {
			return 2;
		} else if (pin < 16) {
			return 3;
		}
	}

	template<gpio_traits::port_t port, const uint8_t pin>
	static constexpr uint32_t _calc_cr_shift() noexcept {
		static_assert(0 <= pin && pin <= 15);
		return static_cast<uint32_t>(exti_traits::port_map[port] << (8 * (pin % 4)));
	}

	template<exti_traits::edge_trigger_t edge>
	static constexpr uint32_t _get_enable_rise_mask(const uint8_t pin) noexcept {
		if (edge == exti_traits::edge_trigger_t::RISING ||
			edge == exti_traits::edge_trigger_t::BOTH) {
			return (1 << pin);
		} else {
			return (0 << pin);
		}
	}

	template<exti_traits::edge_trigger_t edge>
	static constexpr uint32_t _get_enable_fall_mask(const uint8_t pin) noexcept {
		if (edge == exti_traits::edge_trigger_t::FALLING ||
			edge == exti_traits::edge_trigger_t::BOTH) {
			return (1 << pin);
		} else {
			return (0 << pin);
		}
	}

	template<exti_traits::exti_type_t type>
	static constexpr uint32_t _get_enable_interrupt_mask(const uint8_t line) noexcept {
		if (type == exti_traits::exti_type_t::INTERRUPT ||
			type == exti_traits::exti_type_t::BOTH) {
			return (1 << line);
		} else {
			return (0 << line);
		}
	}

	template<exti_traits::exti_type_t type>
	static constexpr uint32_t _get_enable_event_mask(const uint8_t line) noexcept {
		if (type == exti_traits::exti_type_t::EVENT ||
			type == exti_traits::exti_type_t::BOTH) {
			return (1 << line);
		} else {
			return (0 << line);
		}
	}

	static constexpr uint32_t _get_pos_flag_mask(const uint8_t line) noexcept {
		return static_cast<uint32_t>(1 << line);
	}

public:
	static void Init() noexcept {
		RCC->APBENR2 |= RCC_APBENR2_SYSCFGEN; // enable to be sure, not used
	}

	// NB: only one GPIO port can obtain EXTI line (e.g. PA2 excludes PB2 EXTI usage)
	// NB: all constexprs was checked to be located in ROM section
	// 1. Enable EXTI line (GPIO lines from 0 to 15)
	// 2. Enable edge triggers
	// 3. Enable IRQ (event or interrupt)
	template<typename TPin,
			 exti_traits::exti_type_t type,	exti_traits::edge_trigger_t edge>
	static constexpr void ConfigureExtiPin() noexcept {
		const auto PORT = TPin::PORT_TYPE;
		const auto PIN = TPin::PIN;
		EXTI->EXTICR[_calc_cr_pos<PIN>()] |= _calc_cr_shift<PORT, PIN>();

		EXTI->RTSR1 |= _get_enable_rise_mask<edge>(PIN);
		EXTI->FTSR1 |= _get_enable_fall_mask<edge>(PIN);

		EXTI->IMR1 |= _get_enable_interrupt_mask<type>(PIN);
		EXTI->EMR1 |= _get_enable_event_mask<type>(PIN);

	}

	template<typename TPin>
	static constexpr uint32_t GetIrqMask() noexcept {
		return static_cast<uint32_t>(1 << TPin::PIN);
	}

	template<typename TPin, exti_traits::edge_trigger_t edge>
	static constexpr void ClearIrqFlag() noexcept {
		static_assert(edge != exti_traits::edge_trigger_t::BOTH, "BOTH edge is invalid!");
		if constexpr (edge == exti_traits::edge_trigger_t::FALLING) {
			EXTI->FPR1 |= _get_pos_flag_mask(TPin::PIN);
		}
		if constexpr (edge == exti_traits::edge_trigger_t::RISING) {
			EXTI->RPR1 |= _get_pos_flag_mask(TPin::PIN);
		}
	}


};

typedef exti_c exti;

#endif /* INCLUDE_EXTI_HPP_ */
