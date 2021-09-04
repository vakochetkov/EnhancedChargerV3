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
#include "delay_us.h"
}


class adc_c {
	// x = (val / 4095 * 3300 * 2)
	static constexpr uint32_t COEF_MVOLT_DIV = 4095;
	static constexpr uint32_t COEF_MVOLT_MUL = 3300 * 2;
	static constexpr uint32_t MVOLT_OFFSET = 70;
	static constexpr uint32_t MVOLT_OFFSET_CH1 = 5;
	static constexpr uint32_t MVOLT_OFFSET_CH2 = 30;

	static uint32_t convertValue(uint32_t value) noexcept {
		return (((value * COEF_MVOLT_MUL) / COEF_MVOLT_DIV) + MVOLT_OFFSET);
	}

	static void startSelfCalibration() noexcept {
		// make sure intertnal VREG is on and ADC is off
		ADC1->CR |= ADC_CR_ADVREGEN;
		delay_us(100000); // wait vreg ramp

		ADC1->CR &= ~ADC_CR_ADEN;
		ADC1->CFGR1 &= ~ADC_CFGR1_DMAEN;

		// ADCAL=1, cleared by hardware
		// ADC->DR & ADC_CALFACT will consist the calibration factor
		ADC1->CR |= ADC_CR_ADCAL;
		while(!(ADC1->ISR & ADC_ISR_EOCAL));
		ADC1->ISR |= ADC_ISR_EOCAL; // clear flag
	}

	static void setSamplingTime() noexcept {
		// 7.5 clocks for channels #0 and #1 using SMP1
		ADC1->SMPR &= ~ADC_SMPR_SMPSEL0;
		ADC1->SMPR &= ~ADC_SMPR_SMPSEL1;
		ADC1->SMPR |= (0b010UL << ADC_SMPR_SMP1_Pos); // 0b010
	}

	static void configOversampler() noexcept {
		// 16x oversampler (OVSR=0b011, 0xFFF0) + 4bit shift (OVSS=0b0100, 0x0FFF)
		// warn! align is always right in oversample mode!
		ADC1->CFGR2 |= (0b011UL << ADC_CFGR2_OVSR_Pos);
		ADC1->CFGR2 |= (0b0100UL << ADC_CFGR2_OVSS_Pos);
		ADC1->CFGR2 |= ADC_CFGR2_OVSE;
	}

	static void configMeasureMode() noexcept {
		// single-shot for channels #1 and #2
		ADC1->ISR |= ADC_ISR_CCRDY; // clear flag
		ADC1->CFGR1 &= ~ADC_CFGR1_CHSELRMOD; // CHSELRMOD=0

		ADC1->CHSELR |= (ADC_CHSELR_CHSEL0 | ADC_CHSELR_CHSEL1);
		while(!(ADC1->ISR & ADC_ISR_CCRDY)) {}
		ADC1->ISR |= ADC_ISR_CCRDY;

		ADC1->CFGR1 &= ~ADC_CFGR1_CONT;
		ADC1->CFGR1 &= ~ADC_CFGR1_DISCEN; // single conversion
		ADC1->CFGR1 |= ADC_CFGR1_WAIT;	  // wait for read before next conversion
	}

	static inline void start() noexcept {
		ADC1->CR |= ADC_CR_ADSTART;
	}

	static inline uint8_t isAdcReady() noexcept {
		return (ADC1->ISR & ADC_ISR_ADRDY); // ADC ready for next conversion
	}

	static inline uint8_t isResultReady() noexcept {
		return (ADC1->ISR & ADC_ISR_EOC); // conversion finished
	}

	static inline uint16_t read() noexcept {
		return (ADC1->DR); // automatically clear EOC flag
	}

public:
	static void Init() noexcept {
		// sysclk as clock source
		RCC->CCIPR &= ~RCC_CCIPR_ADCSEL;
		RCC->APBENR2 |= RCC_APBENR2_ADCEN;

		// source = PCLK / 4 / div = 64 / 4 / 1 = 16 MHz
		ADC1->CFGR2 |= ADC_CFGR2_CKMODE_1;
		ADC->CCR &= ~ADC_CCR_PRESC;

		startSelfCalibration();
		ADC1->CR |= ADC_CR_ADEN; 		 // ADC enable
		ADC1->CFGR1 &= ~ADC_CFGR1_EXTEN; // software start
		ADC1->CFGR1 &= ~ADC_CFGR1_RES;	 // 12 bit

		configOversampler();
		setSamplingTime();
		configMeasureMode();
	}

	static uint16_t MeasureVoltage(uint8_t adcChannel) noexcept {
		uint16_t valueChannel1 = 0;
		uint16_t valueChannel2 = 0;

		start();
		while(!isResultReady()) {}
		valueChannel1 = read();
		while(!isResultReady()) {}
		valueChannel2 = read();

		switch(adcChannel) {
		case 0: return (convertValue(valueChannel1) + MVOLT_OFFSET_CH1);
		case 1: return (convertValue(valueChannel2) + MVOLT_OFFSET_CH2);
		default: return 0;
		}
	}

	static uint16_t MeasureVoltageMock(uint8_t adcChannel) noexcept {
		static uint16_t val = 0;
		val += 10;
		return val;
	}

};

typedef adc_c ADCD;

#endif /* INCLUDE_ADC_HPP_ */
