#include "stm32g070kb.hpp"
#include "adc.hpp"
#include "exti.hpp"
#include "button.hpp"
#include "rcc_config.hpp"
#include "pin_usage.hpp"


int main() {

	rcc::Init();
	rcc::ConfigSysClockMs(1);

	PORTA::ClockEnable();
	PORTB::ClockEnable();
	PORTC::ClockEnable();

	exti::Init();
	BUTTON::Init();

	while(1) {

	}

	return 0;
}
