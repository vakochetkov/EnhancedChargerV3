/*
 * button.hpp
 *
 *  Created on: 8 мая 2021 г.
 *      Author: kochetkov
 */

#ifndef INCLUDE_BUTTON_HPP_
#define INCLUDE_BUTTON_HPP_

#include <type_traits>
#include "stm32g070kb.hpp"
#include "rcc_config.hpp"
#include "pin_usage.hpp"
#include "exti.hpp"

class button_c {

public:
	using BTN1 = PB0;
	using BTN2 = PB1;

	static void Init() noexcept {
		BTN1::ConfigAsIn(gpio_traits::pullup_t::DOWN);
		BTN2::ConfigAsIn(gpio_traits::pullup_t::DOWN);
		exti::ConfigureExtiPin<BTN1,exti_traits::exti_type_t::INTERRUPT,
		 	 	 	 	 	 	 	exti_traits::edge_trigger_t::BOTH>();
		exti::ConfigureExtiPin<BTN2,exti_traits::exti_type_t::INTERRUPT,
		 	 	 	 	 	 	 	exti_traits::edge_trigger_t::BOTH>();

		NVIC_SetPriority(EXTI0_1_IRQn, 3); // lowest priority (2 bits on Cortex-M0+)
		NVIC_EnableIRQ(EXTI0_1_IRQn);
	}

	// 896	      8	   1568	   2472	    9a8
	// button dispatcher
	template<typename TBtn>
	static constexpr void ButtonsCallback(exti_traits::edge_trigger_t edge) {
		if constexpr (std::is_same<TBtn, BTN1>::value) {
			Btn1Callback(edge);
		}
		if constexpr (std::is_same<TBtn, BTN2>::value) {
			Btn2Callback(edge);
		}
	}

	static void Btn1Callback(exti_traits::edge_trigger_t edge) noexcept {
		if (edge == exti_traits::edge_trigger_t::RISING) {
			__BKPT(0);
		} else {
			__BKPT(1);
		}
	}

	static void Btn2Callback(exti_traits::edge_trigger_t edge) noexcept {
		if (edge == exti_traits::edge_trigger_t::RISING) {
			__BKPT(0);
		} else {
			__BKPT(1);
		}
	}
};


typedef button_c BUTTON;

#endif /* INCLUDE_BUTTON_HPP_ */
