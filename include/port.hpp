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
}

template<gpio_traits::port_t TPort>
class port_c {

	static inline constexpr GPIO_TypeDef volatile* PORTx()
	{
		return reinterpret_cast<GPIO_TypeDef*>(TPort);
	};

public:
	static void ConfigAsIn(uint8_t pin) noexcept {

	}

	static void ConfigAsPP(uint8_t pin) noexcept {

	}

	static void ConfigAsOD(uint8_t pin) noexcept {

	}

	static void ConfigAsAF(uint8_t pin) noexcept {

	}

	static inline void WritePort(uint16_t state) noexcept {
		PORTx()->ODR = state;
	}

	static inline void WritePin(uint8_t pin, bool state) noexcept {
		if (state) {
			PORTx()->BSRR |= static_cast<uint32_t>(1 << pin);
		} else {
			PORTx()->BSRR |= static_cast<uint32_t>(1 << (pin + 16));
		}
	}

	static inline uint16_t ReadPort() noexcept {
		return static_cast<uint16_t>(PORTx()->IDR);
	}

	static inline uint16_t ReadPortOut() noexcept {
		return static_cast<uint16_t>(PORTx()->ODR);
	}

	static inline uint8_t ReadPin(uint8_t pin) noexcept {
		return static_cast<uint8_t>(((PORTx()->IDR) >> pin) & 0x1);
	}

	static inline uint8_t ReadPinOut(uint8_t pin) noexcept {
		return static_cast<uint8_t>(((PORTx()->ODR) >> pin) & 0x1);
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

///////////////////////////////////////////////////////////
//typedef struct
//{
//  __IO uint32_t MODER;       /*!< GPIO port mode register,               Address offset: 0x00      */
//  __IO uint32_t OTYPER;      /*!< GPIO port output type register,        Address offset: 0x04      */
//  __IO uint32_t OSPEEDR;     /*!< GPIO port output speed register,       Address offset: 0x08      */
//  __IO uint32_t PUPDR;       /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
//  __IO uint32_t IDR;         /*!< GPIO port input data register,         Address offset: 0x10      */
//  __IO uint32_t ODR;         /*!< GPIO port output data register,        Address offset: 0x14      */
//  __IO uint32_t BSRR;        /*!< GPIO port bit set/reset  register,     Address offset: 0x18      */
//  __IO uint32_t LCKR;        /*!< GPIO port configuration lock register, Address offset: 0x1C      */
//  __IO uint32_t AFR[2];      /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
//  __IO uint32_t BRR;         /*!< GPIO Bit Reset register,               Address offset: 0x28      */
//} GPIO_TypeDef;



#endif /* PORT_HPP_ */
