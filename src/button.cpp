/*
 * BTN.cpp
 *
 *  Created on: 9 мая 2021 г.
 *      Author: kochetkov
 */

#include "button.hpp"

extern "C" void EXTI0_1_IRQHandler(void) {
	// check rising edge on lines
	if (EXTI->RPR1 & exti::GetIrqMask<BTN::BTN1>()) {
		BTN::ButtonsCallback<BTN::BTN1>(exti_traits::edge_trigger_t::RISING);
		exti::ClearIrqFlag<BTN::BTN1, exti_traits::edge_trigger_t::RISING>();
	}
	if (EXTI->RPR1 & exti::GetIrqMask<BTN::BTN2>()) {
		BTN::ButtonsCallback<BTN::BTN2>(exti_traits::edge_trigger_t::RISING);
		exti::ClearIrqFlag<BTN::BTN2, exti_traits::edge_trigger_t::RISING>();
	}

	// check falling edge on lines
	if (EXTI->FPR1 & exti::GetIrqMask<BTN::BTN1>()) {
		BTN::ButtonsCallback<BTN::BTN1>(exti_traits::edge_trigger_t::FALLING);
		exti::ClearIrqFlag<BTN::BTN1, exti_traits::edge_trigger_t::FALLING>();
	}
	if (EXTI->FPR1 & exti::GetIrqMask<BTN::BTN2>()) {
		BTN::ButtonsCallback<BTN::BTN2>(exti_traits::edge_trigger_t::FALLING);
		exti::ClearIrqFlag<BTN::BTN2, exti_traits::edge_trigger_t::FALLING>();
	}
}






