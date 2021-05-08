
#include "stm32g070kb.hpp"
#include "adc.hpp"
#include "rcc_config.hpp"
#include "pin_usage.hpp"


int main() {

	rcc::Init();
	PORTA::ClockEnable();
	PORTB::ClockEnable();
	PORTC::ClockEnable();

	PA1::ConfigAsPP();

	while(1);

	return 0;
}
