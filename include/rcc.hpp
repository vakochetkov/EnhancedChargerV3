/*
 * rcc.hpp
 *
 *  Created on: 1 апр. 2021 г.
 *      Author: kochetkov
 */

#ifndef INCLUDE_RCC_HPP_
#define INCLUDE_RCC_HPP_


extern "C" {
#include "stm32g0xx.h"
}

extern "C" void delay_ms(uint32_t ms);

namespace rcc_traits {
enum class hsclk_src_t : uint8_t {
	HSE = 0,
	HSI
};

typedef struct {
	uint32_t M;
	uint32_t N;
	uint32_t R;
	uint32_t P;
} pll_config_t;

extern volatile uint32_t SYSTICK;
}


template<rcc_traits::hsclk_src_t TClk, const uint32_t TClkFreq, const rcc_traits::pll_config_t& TPLLConfig,
		 uint16_t TAHBDiv, uint16_t TAPBDiv, const uint32_t TDesiredFreq>
class rcc_c {

public:

	static void Init() noexcept {
		static_assert((TPLLConfig.M >= 1) && (TPLLConfig.M <= 8),  "PLL M not in range!");
		static_assert((TPLLConfig.N >= 8) && (TPLLConfig.N <= 86), "PLL N not in range!");
		static_assert((TPLLConfig.R >= 2) && (TPLLConfig.R <= 8),  "PLL R not in range!");
		static_assert((TPLLConfig.P >= 2) && (TPLLConfig.P <= 32), "PLL P not in range!");
		static_assert((TAHBDiv >= 1) && (TAHBDiv <= 512) && ((TAHBDiv % 2 == 0) || (TAHBDiv == 1)), "AHBDiv not in range!");
		static_assert((TAPBDiv >= 1) && (TAPBDiv <= 16) && ((TAPBDiv % 2 == 0) || (TAPBDiv == 1)), "APBDiv not in range!");

		// Set power scale and flash latency at max
		MODIFY_REG(PWR->CR1, PWR_CR1_VOS, PWR_CR1_VOS_0);
		while((PWR->SR2 & PWR_SR2_VOSF) != 0);
		MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_2);
		while((FLASH->ACR & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_2);

		static_assert(TClk == rcc_traits::hsclk_src_t::HSI, "Only HSI as PLL source implementation is supported");
		static_assert(TClkFreq == 16000000, "HSI is 16MHz");
		while((RCC->CR & RCC_CR_HSIRDY) == 0);

		RCC->CR &= ~RCC_CR_PLLON;
		MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, ((TPLLConfig.M - 1) << RCC_PLLCFGR_PLLM_Pos));
		MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN, ((TPLLConfig.N) << RCC_PLLCFGR_PLLN_Pos));
		MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLR, ((TPLLConfig.R - 1) << RCC_PLLCFGR_PLLR_Pos));
		MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLP, ((TPLLConfig.P - 1) << RCC_PLLCFGR_PLLP_Pos));
		MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC, RCC_PLLCFGR_PLLSRC_HSI);
		RCC->CR |= RCC_CR_PLLON;
		while((RCC->CR & RCC_CR_PLLRDY) == 0);
		RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN | RCC_PLLCFGR_PLLPEN;

		// set APB and AHB prescalers to 1
		MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, 0b0000UL < RCC_CFGR_HPRE_Pos);
		MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE, 0b000UL < RCC_CFGR_PPRE_Pos);

		// switch sysclock source
		MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, 0b010UL << RCC_CFGR_SW_Pos);
		while((RCC->CFGR & RCC_CFGR_SW) != (0b010UL << RCC_CFGR_SW_Pos));

		static_assert(TDesiredFreq ==
					 (TClkFreq / TPLLConfig.M * TPLLConfig.N / TPLLConfig.R),
					 "SYSCLOCK frequency not match");
		static_assert(TDesiredFreq ==
					 (TClkFreq / TPLLConfig.M * TPLLConfig.N / TPLLConfig.R / TAHBDiv),
					 "HCLK frequency not match");
		static_assert(TDesiredFreq ==
					 (TClkFreq / TPLLConfig.M * TPLLConfig.N / TPLLConfig.R / TAHBDiv / TAPBDiv),
					 "HCLK frequency not match");

		SystemCoreClockUpdate(); 				// update CMSIS variable in run-time
		if (SystemCoreClock != TDesiredFreq) {  // call debugger if not equal
			//__BKPT(0);
		}
	}

	static inline void Reset() noexcept {
		NVIC_SystemReset();
	}

	static void ConfigSysClockMs(uint32_t period_ms) noexcept {
		SysTick_Config(TDesiredFreq / 1000 * period_ms);
	}

	static inline volatile uint32_t GetTick() noexcept {
		return rcc_traits::SYSTICK;
	}
};


#endif /* INCLUDE_RCC_HPP_ */
