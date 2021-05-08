/*
 * adc.hpp
 *
 *  Created on: 2 апр. 2021 г.
 *      Author: kochetkov
 */

#ifndef INCLUDE_ADC_HPP_
#define INCLUDE_ADC_HPP_

extern "C" {
#include "stm32g0xx.h"
}

class adc_c {

public:
	static void Init() noexcept {
		// sysclk as clock source
		RCC->CCIPR &= ~RCC_CCIPR_ADCSEL;
		RCC->APBENR2 |= RCC_APBENR2_ADCEN;


	}

	static void StartConversion() noexcept {

	}


};



#endif /* INCLUDE_ADC_HPP_ */
