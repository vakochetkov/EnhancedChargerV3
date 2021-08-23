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
#include "timeout.hpp"
#include "exti.hpp"



class button_c {

	static inline uint8_t btn1click = 0;
	static inline uint8_t btn2click = 0;

public:
	using BTN1 = PB1; // buttons and indicators are mixed up on PCB
	using BTN2 = PB0;

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
			btn1click = 1;
		} else {
			btn1click = 0;
		}
	}

	static void Btn2Callback(exti_traits::edge_trigger_t edge) noexcept {
		if (edge == exti_traits::edge_trigger_t::RISING) {
			btn2click = 1;
		} else {
			btn2click = 0;
		}
	}

	static bool IsBtn1Pressed() noexcept {
		if (btn1click) {
			btn1click = 0;
			return true;
		} else {
			return false;
		}
	}

	static bool IsBtn2Pressed() noexcept {
		if (btn2click) {
			btn2click = 0;
			return true;
		} else {
			return false;
		}
	}

};

typedef button_c BTN;

#endif /* INCLUDE_BUTTON_HPP_ */
