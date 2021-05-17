/*
 * pin.hpp
 *
 *  Created on: 31 мар. 2021 г.
 *      Author: kochetkov
 */

#ifndef INCLUDE_PIN_HPP_
#define INCLUDE_PIN_HPP_

#include "port.hpp"

template<gpio_traits::port_t TPort, uint8_t TPin>
class pin_c {

public:
	using PORT = port_c<TPort>;
	static constexpr auto PORT_TYPE = TPort;
	static constexpr auto PIN = TPin;

	static constexpr void ConfigAsIn(gpio_traits::pullup_t pulltype) noexcept
	{
		static_assert(TPin >= 0 && TPin <= 15, "Pin number not in range!");
		PORT::ConfigAsIn(TPin, pulltype);
	}

	static constexpr void ConfigAsPP(gpio_traits::speed_t speed, gpio_traits::pullup_t pulltype) noexcept
	{
		static_assert(TPin >= 0 && TPin <= 15, "Pin number not in range!");
		PORT::ConfigAsPP(TPin, speed, pulltype);
	}

	static constexpr void ConfigAsOD(gpio_traits::speed_t speed, gpio_traits::pullup_t pulltype) noexcept
	{
		static_assert(TPin >= 0 && TPin <= 15, "Pin number not in range!");
		PORT::ConfigAsOD(TPin, speed, pulltype);
	}

	static constexpr void ConfigAsAF(gpio_traits::afline_t afline, gpio_traits::otype_t otype,
									gpio_traits::speed_t speed, gpio_traits::pullup_t pulltype) noexcept
	{
		static_assert(TPin >= 0 && TPin <= 15, "Pin number not in range!");
		PORT::ConfigAsAF(TPin, afline, otype, speed, pulltype);
	}

	static constexpr void ConfigAsAnalog() noexcept
	{
		static_assert(TPin >= 0 && TPin <= 15, "Pin number not in range!");
		PORT::ConfigAsAnalog(TPin);
	}

	static constexpr void Write(uint8_t state) noexcept {
		PORT::WritePin(TPin, state);
	}

	static constexpr uint8_t Read() noexcept {
		return PORT::ReadPin(TPin);
	}

	static constexpr void Set() noexcept {
		Write(1);
	}

	static constexpr void Clear() noexcept {
		Write(0);
	}

	static constexpr bool IsSet() noexcept {
		return (Read());
	}

	static constexpr bool IsClear() noexcept {
		return (!Read());
	}

};

inline void test_pin_gpio() noexcept {
	using TEST_PA2 = pin_c<gpio_traits::port_t::PORTA, 2>;
	TEST_PA2::ConfigAsIn(gpio_traits::pullup_t::UP);
	TEST_PA2::ConfigAsPP(gpio_traits::speed_t::LOW, gpio_traits::pullup_t::NONE);
	TEST_PA2::ConfigAsOD(gpio_traits::speed_t::LOW, gpio_traits::pullup_t::NONE);
	TEST_PA2::ConfigAsAF(gpio_traits::afline_t::AF0, gpio_traits::otype_t::OD,
					gpio_traits::speed_t::HIGH, gpio_traits::pullup_t::NONE);
	TEST_PA2::ConfigAsAnalog();
}


#endif /* INCLUDE_PIN_HPP_ */
