/*
 * rcc.hpp
 *
 *  Created on: 17 мая 2021 г.
 *      Author: kochetkov
 */

#include "rcc.hpp"

namespace rcc_traits {
volatile uint32_t SYSTICK = 0;
}


extern "C" void SysTick_Handler(void) {
	rcc_traits::SYSTICK++;
}


