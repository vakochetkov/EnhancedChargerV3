/*
 * pin_usage.hpp
 *
 *  Created on: 31 мар. 2021 г.
 *      Author: kochetkov
 */

#ifndef INCLUDE_PIN_USAGE_HPP_
#define INCLUDE_PIN_USAGE_HPP_


#include "pin.hpp"


// ADC
using PA0 = pin_c<gpio_traits::port_t::PORTA, 0>; // VBAT1
using PA1 = pin_c<gpio_traits::port_t::PORTA, 1>; // VBAT2

// TM1628
using PA2 = pin_c<gpio_traits::port_t::PORTA, 2>; // MDIO
using PA4 = pin_c<gpio_traits::port_t::PORTA, 4>; // NSS (LATCH)
using PA5 = pin_c<gpio_traits::port_t::PORTA, 5>; // MCLK

// BTN1 BTN2
using PB0 = pin_c<gpio_traits::port_t::PORTB, 0>;
using PB1 = pin_c<gpio_traits::port_t::PORTB, 1>;

// ISET 1
using PB4 = pin_c<gpio_traits::port_t::PORTB, 4>; // 0.5 A
using PB5 = pin_c<gpio_traits::port_t::PORTB, 5>; // 1.0 A
using PB6 = pin_c<gpio_traits::port_t::PORTB, 6>; // 1.5 A
using PB7 = pin_c<gpio_traits::port_t::PORTB, 7>; // 2.0 A
using PB8 = pin_c<gpio_traits::port_t::PORTB, 8>; // 2.5 A

// ISET 2
using PB2  = pin_c<gpio_traits::port_t::PORTB, 2>;  // 0.5 A
using PA8  = pin_c<gpio_traits::port_t::PORTA, 8>;  // 1.0 A
using PA9  = pin_c<gpio_traits::port_t::PORTA, 9>;  // 1.5 A
using PC6  = pin_c<gpio_traits::port_t::PORTC, 6>;  // 2.0 A
using PA10 = pin_c<gpio_traits::port_t::PORTA, 10>; // 2.5 A


#endif /* INCLUDE_PIN_USAGE_HPP_ */
