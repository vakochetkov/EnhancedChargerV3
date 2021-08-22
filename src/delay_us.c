#include "delay_us.h"
#include "stm32g070xx.h"

/*
 * it's not precise because of loop overhead
 */
static void delay_1us() {
	for (uint8_t i = 0; i < 64; i++) {
		__NOP();
	}
}

void inline delay_us(uint32_t us) {
	while(--us) {
		delay_1us();
	}
}


