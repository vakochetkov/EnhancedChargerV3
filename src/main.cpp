#include "stm32g070kb.hpp"
#include "rcc_config.hpp"
#include "pin_usage.hpp"
#include "exti.hpp"

#include "controller.hpp"


int main() {

 	rcc::Init();
	rcc::ConfigSysClockMs(1);

	PORTA::ClockEnable();
	PORTB::ClockEnable();
	PORTC::ClockEnable();

	exti::Init();

	CTRL::Init();
	while(1) {
		CTRL::Tick();
	}

	return 0;
}
