/*
 * pin_usage.hpp
 *
 *  Created on: 31 мар. 2021 г.
 *      Author: kochetkov
 */

#ifndef INCLUDE_PIN_USAGE_HPP_
#define INCLUDE_PIN_USAGE_HPP_


#include "pin.hpp"


// VBAT1 VBAT2
using PA0 = pin_c<gpio_traits::port_t::PORTA, 0>;
using PA1 = pin_c<gpio_traits::port_t::PORTA, 1>;

// MDIO NSS MCLK
using PA2 = pin_c<gpio_traits::port_t::PORTA, 2>;
using PA4 = pin_c<gpio_traits::port_t::PORTA, 4>;
using PA5 = pin_c<gpio_traits::port_t::PORTA, 5>;

// BTN1 BTN2
using PB0 = pin_c<gpio_traits::port_t::PORTB, 0>;
using PB1 = pin_c<gpio_traits::port_t::PORTB, 1>;

// ISET 1
using PB4 = pin_c<gpio_traits::port_t::PORTB, 4>;
using PB5 = pin_c<gpio_traits::port_t::PORTB, 5>;
using PB6 = pin_c<gpio_traits::port_t::PORTB, 6>;
using PB7 = pin_c<gpio_traits::port_t::PORTB, 7>;
using PB8 = pin_c<gpio_traits::port_t::PORTB, 8>;

// ISET 2
using PB2  = pin_c<gpio_traits::port_t::PORTB, 2>;
using PA8  = pin_c<gpio_traits::port_t::PORTA, 8>;
using PA9  = pin_c<gpio_traits::port_t::PORTA, 9>;
using PC6  = pin_c<gpio_traits::port_t::PORTC, 6>;
using PA10 = pin_c<gpio_traits::port_t::PORTA, 10>;


#endif /* INCLUDE_PIN_USAGE_HPP_ */
