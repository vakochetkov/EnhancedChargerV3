#include "stm32g070kb.hpp"
#include "rcc_config.hpp"
#include "pin_usage.hpp"
#include "timeout.hpp"
#include "indicator.hpp"
#include "current_selector.hpp"
#include "adc.hpp"
#include "exti.hpp"
#include "button.hpp"

#include "controller.hpp"


int main() {

 	rcc::Init();
	rcc::ConfigSysClockMs(1);

	PORTA::ClockEnable();
	PORTB::ClockEnable();
	PORTC::ClockEnable();

	// TODO: move Init's to CTRL::Init
	CURSEL1::Init();
	CURSEL2::Init();

	exti::Init();
	BTN::Init();

	IND::Init();
	IND::ShowBootLogo();

	while(1) {
		CTRL::Tick();
	}

	return 0;
}
