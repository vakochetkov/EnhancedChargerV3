#ifndef INCLUDE_DELAY_US_H_
#define INCLUDE_DELAY_US_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// dummy us delay on NOP's for slow TM1628 IC
void delay_us(uint32_t us);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_DELAY_US_H_ */
