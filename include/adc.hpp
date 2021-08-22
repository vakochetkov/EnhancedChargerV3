/*
 * adc.hpp
 *
 * Use ADC1
 * Dividers are 10k/10k 1%
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
	static void startSelfCalibration() noexcept {
		// make sure intertnal VREG is on and ADC is off
		ADC1->CR |= ADC_CR_ADVREGEN;
		ADC1->CR &= ~ADC_CR_ADEN;
		// ADCAL=1, cleared by hardware
		// AdC->DR will consist the calibration factor
	}

	static void setSamplingTime() noexcept {

	}

	static void configOversampler() noexcept {

	}

public:
	static void Init() noexcept {
		// sysclk as clock source
		RCC->CCIPR &= ~RCC_CCIPR_ADCSEL;
		RCC->APBENR2 |= RCC_APBENR2_ADCEN;

		// source = PCLK / 4 / div = 64 / 4 / 1 = 16 MHz
		ADC->CCR &= ~ADC_CCR_PRESC;
		ADC1->CFGR2 |= ADC_CFGR2_CKMODE_1;
	}

	static uint16_t MeasureVoltage(uint8_t adcChannel) noexcept {
		return 4100; // XXX: stub
	}

};

typedef adc_c ADCD;

#endif /* INCLUDE_ADC_HPP_ */
