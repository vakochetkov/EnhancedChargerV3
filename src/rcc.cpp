#include "rcc.hpp"
#include "rcc_config.hpp"

namespace rcc_traits {
volatile uint32_t SYSTICK = 0;
}


extern "C" void SysTick_Handler(void) {
	rcc_traits::SYSTICK++;
}

extern "C" void delay_ms(uint32_t ms) {
	// suppose SysTick period is 1 ms
	uint32_t start = rcc_traits::SYSTICK;
	while ((rcc_traits::SYSTICK - start) < ms);
}

