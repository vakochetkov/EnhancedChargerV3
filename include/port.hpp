/*
 * port.hpp
 *
 *  Created on: 31 мар. 2021 г.
 *      Author: kochetkov
 */

#ifndef PORT_HPP_
#define PORT_HPP_

extern "C" {
#include "stm32g0xx.h"
}

namespace gpio_traits {
	enum class port_t : uint32_t {
		PORTA = GPIOA_BASE,
		PORTB = GPIOB_BASE,
		PORTC = GPIOC_BASE,
		PORTD = GPIOD_BASE,
		PORTF = GPIOF_BASE
	};

	enum class afline_t : uint8_t {
		AF0 = 0b0000,
		AF1 = 0b0001,
		AF2 = 0b0010,
		AF3 = 0b0011,
		AF4 = 0b0100,
		AF5 = 0b0101,
		AF6 = 0b0110,
		AF7 = 0b0111
	};

	enum class mode_t : uint8_t {
		INPUT 	  = 0b00,
		OUTPUT 	  = 0b01,
		ALTERNATE = 0b10,
		ANALOG	  = 0b11	// reset state
	};

	enum class otype_t : uint8_t {
		PP 	  = 0b0,	// reset state
		OD	  = 0b1
	};

	// NB: The FT GPIOs cannot be set to high speed
	enum class speed_t : uint8_t {
		VERY_LOW  = 0b00,
		LOW	  	  = 0b01,
		HIGH	  = 0b10,
		VERY_HIGH = 0b11
	};

	enum class pullup_t : uint8_t {
		NONE = 0b00,
		UP	 = 0b01,
		DOWN = 0b10
	};
}

template<gpio_traits::port_t TPort>
class port_c {

	static inline constexpr GPIO_TypeDef volatile * PORTx()
	{
		return (reinterpret_cast<GPIO_TypeDef*>(TPort));
	};

public:
	static void ConfigAsIn(uint8_t pin, gpio_traits::pullup_t pulltype) noexcept {
		// reset MODER to INPUT state
		PORTx()->MODER   &= ~static_cast<uint32_t>(static_cast<uint8_t>(gpio_traits::mode_t::ANALOG) << (pin * 2));
		PORTx()->PUPDR   |= static_cast<uint32_t>(static_cast<uint8_t>(pulltype) << (pin * 2));
	}

	static void ConfigAsPP(uint8_t pin, gpio_traits::speed_t speed, gpio_traits::pullup_t pulltype) noexcept
	{
		// reset MODER to INPUT state
		PORTx()->MODER   &= ~static_cast<uint32_t>(static_cast<uint8_t>(gpio_traits::mode_t::ANALOG) << (pin * 2));
		PORTx()->OSPEEDR |= static_cast<uint32_t>(static_cast<uint8_t>(speed) << (pin * 2));
		PORTx()->OTYPER  |= static_cast<uint32_t>(static_cast<uint8_t>(gpio_traits::otype_t::PP) << (pin));
		PORTx()->PUPDR   |= static_cast<uint32_t>(static_cast<uint8_t>(pulltype) << (pin * 2));
		PORTx()->MODER   |= static_cast<uint32_t>(static_cast<uint8_t>(gpio_traits::mode_t::OUTPUT) << (pin * 2));
	}

	static void ConfigAsOD(uint8_t pin, gpio_traits::speed_t speed, gpio_traits::pullup_t pulltype) noexcept
	{
		PORTx()->MODER   &= ~static_cast<uint32_t>(static_cast<uint8_t>(gpio_traits::mode_t::ANALOG) << (pin * 2));
		PORTx()->OSPEEDR |= static_cast<uint32_t>(static_cast<uint8_t>(speed) << (pin * 2));
		PORTx()->OTYPER  |= static_cast<uint32_t>(static_cast<uint8_t>(gpio_traits::otype_t::OD) << (pin));
		PORTx()->PUPDR   |= static_cast<uint32_t>(static_cast<uint8_t>(pulltype) << (pin * 2));
		PORTx()->MODER   |= static_cast<uint32_t>(static_cast<uint8_t>(gpio_traits::mode_t::OUTPUT) << (pin * 2));
	}

	static void ConfigAsAF(uint8_t pin, gpio_traits::afline_t afline, gpio_traits::otype_t otype,
							gpio_traits::speed_t speed, gpio_traits::pullup_t pulltype) noexcept
	{
		PORTx()->MODER   &= ~static_cast<uint32_t>(static_cast<uint8_t>(gpio_traits::mode_t::ANALOG) << (pin * 2));

		PORTx()->OTYPER  |= static_cast<uint32_t>(static_cast<uint8_t>(otype) 	 << (pin));
		PORTx()->OSPEEDR |= static_cast<uint32_t>(static_cast<uint8_t>(speed) 	 << (pin * 2));
		PORTx()->PUPDR   |= static_cast<uint32_t>(static_cast<uint8_t>(pulltype) << (pin * 2));
		PORTx()->AFR[((pin > 7) ? 1 : 0)] |= (static_cast<uint8_t>(afline) << (pin * 4));

		PORTx()->MODER   |= static_cast<uint32_t>(static_cast<uint8_t>(gpio_traits::mode_t::ALTERNATE) << (pin * 2));
	}

	static void ConfigAsAnalog(uint8_t pin) noexcept {
		PORTx()->MODER   &= ~static_cast<uint32_t>(static_cast<uint8_t>(gpio_traits::mode_t::ANALOG) << (pin * 2));
		PORTx()->PUPDR   |= static_cast<uint32_t>(static_cast<uint8_t>(gpio_traits::pullup_t::NONE) << (pin * 2));
		PORTx()->MODER   |= static_cast<uint32_t>(static_cast<uint8_t>(gpio_traits::mode_t::ANALOG) << (pin * 2));
	}

	static inline void WritePort(uint16_t state) noexcept {
		if (state) {
			PORTx()->BSRR = static_cast<uint16_t>(state);
		} else {
			PORTx()->BSRR = static_cast<uint32_t>(static_cast<uint16_t>(state) << 16);
		}
	}

	static inline void WritePin(uint8_t pin, bool state) noexcept {
		if (state) {
			PORTx()->BSRR |= static_cast<uint32_t>(1 << pin);
		} else {
			PORTx()->BSRR |= static_cast<uint32_t>(1 << (pin + 16));
		}
	}

	static inline uint16_t ReadPort() noexcept {
		return (static_cast<uint16_t>(PORTx()->IDR));
	}

	static inline uint16_t ReadPortOut() noexcept {
		return (static_cast<uint16_t>(PORTx()->ODR));
	}

	static inline uint8_t ReadPin(uint8_t pin) noexcept {
		return (static_cast<uint8_t>(((PORTx()->IDR) >> pin) & 0x1));
	}

	static inline uint8_t ReadPinOut(uint8_t pin) noexcept {
		return (static_cast<uint8_t>(((PORTx()->ODR) >> pin) & 0x1));
	}

	static constexpr void ClockEnable() noexcept {
		static_assert((TPort == gpio_traits::port_t::PORTA) ||
					  (TPort == gpio_traits::port_t::PORTB) ||
					  (TPort == gpio_traits::port_t::PORTC) ||
					  (TPort == gpio_traits::port_t::PORTD) ||
					  (TPort == gpio_traits::port_t::PORTF), "Invalid port");
		if (TPort == gpio_traits::port_t::PORTA) {
			RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
		} else if (TPort == gpio_traits::port_t::PORTB) {
			RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
		} else if (TPort == gpio_traits::port_t::PORTC) {
			RCC->IOPENR |= RCC_IOPENR_GPIOCEN;
		} else if (TPort == gpio_traits::port_t::PORTD) {
			RCC->IOPENR |= RCC_IOPENR_GPIODEN;
		} else if (TPort == gpio_traits::port_t::PORTF) {
			RCC->IOPENR |= RCC_IOPENR_GPIOFEN;
		}
	}

	static constexpr void ClockDisable() noexcept {
		static_assert((TPort == gpio_traits::port_t::PORTA) ||
					  (TPort == gpio_traits::port_t::PORTB) ||
					  (TPort == gpio_traits::port_t::PORTC) ||
					  (TPort == gpio_traits::port_t::PORTD) ||
					  (TPort == gpio_traits::port_t::PORTF), "Invalid port");
		if (TPort == gpio_traits::port_t::PORTA) {
			RCC->IOPENR &= ~RCC_IOPENR_GPIOAEN;
		} else if (TPort == gpio_traits::port_t::PORTB) {
			RCC->IOPENR &= ~RCC_IOPENR_GPIOBEN;
		} else if (TPort == gpio_traits::port_t::PORTC) {
			RCC->IOPENR &= ~RCC_IOPENR_GPIOCEN;
		} else if (TPort == gpio_traits::port_t::PORTD) {
			RCC->IOPENR &= ~RCC_IOPENR_GPIODEN;
		} else if (TPort == gpio_traits::port_t::PORTF) {
			RCC->IOPENR &= ~RCC_IOPENR_GPIOFEN;
		}
	}

};


typedef port_c <gpio_traits::port_t::PORTA> PORTA;
typedef port_c <gpio_traits::port_t::PORTB> PORTB;
typedef port_c <gpio_traits::port_t::PORTC> PORTC;
typedef port_c <gpio_traits::port_t::PORTD> PORTD;
typedef port_c <gpio_traits::port_t::PORTF> PORTF;


#endif /* PORT_HPP_ */
