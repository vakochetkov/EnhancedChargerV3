#ifndef INCLUDE_TM1628_HPP_
#define INCLUDE_TM1628_HPP_


#include "stm32g070kb.hpp"
#include "pin_usage.hpp"
#include "rcc_config.hpp"
extern "C" {
#include "delay_us.h"
}

// warn! read (key-scan) is not implemented
// datasheet https://www.promelec.ru/fs/sources/75/23/e7/63/160e8a5188c04ff3811526fc.pdf
template<typename NSS, typename DIO, typename CLK>
class tm1628_drv_c {

	static void send(uint8_t data) noexcept {
		for (uint8_t i = 0; i < 8; i++) {
			CLK::Clear();
			delay_us(8);

			DIO::Write((data & 0x1) ? 0x1 : 0x0);
			data >>= 1;
			delay_us(8);

			CLK::Set();
			delay_us(8);
		}
	}

	/**
	 * transaction is followed by low NSS
	 * @param cmd - cmd byte to send
	 */
	static void sendCmd(uint8_t cmd) noexcept {
		NSS::Clear();
		delay_us(8);
		send(cmd);
		delay_us(8);
		NSS::Set();
		delay_us(8);
	}

	/**
	 * Update data on addr cell
	 * @param addr - addr
	 * @param data - data to be written
	 */
	static void sendData(uint8_t addr, uint8_t data) noexcept {
		NSS::Clear();
		delay_us(8);
		// 0b01000100
		//    ^normal mode
		//        ^fixed address
		//		   ^^write data
		send(0x44);
		delay_us(8);
		NSS::Set();
		delay_us(8);

		NSS::Clear();
		delay_us(8);
		// 0b1100_XXXX
		//   ^^indicates address command
		send(0xC0 | addr);  // address
		send(data); 		// data for display RAM
		delay_us(8);
		NSS::Set();
		delay_us(8);
	}

public:

	static void Init() noexcept {
		// GPIOs are open-drain bc FET level shifter
		NSS::ConfigAsOD(gpio_traits::speed_t::LOW, gpio_traits::pullup_t::UP);
		DIO::ConfigAsOD(gpio_traits::speed_t::LOW, gpio_traits::pullup_t::UP);
		CLK::ConfigAsOD(gpio_traits::speed_t::LOW, gpio_traits::pullup_t::UP);

		NSS::Set();
		CLK::Set();
		// command #1
		sendCmd(0x2);
		// command #4 - setup display
		SetupDisplay(0x1, 0x6);
	}

	/**
	 * @param enable  - display on/off
	 * @param dimming - pulse width 1/16...14/16
	 * 					(two frames reserved for key scan)
	 */
	static void SetupDisplay(uint8_t enable, uint8_t dimming) noexcept {
		sendCmd(0x80 | (enable ? 0x8 : 0x0) | ((dimming > 0x7) ? 0x7 : dimming));
	}

	static void WriteDigit(uint8_t digit, uint8_t code) noexcept {
		sendData((digit > 13 ? 13 : digit), code);
	}

};

typedef tm1628_drv_c<PA4, PA2, PA5> TM1628;

#endif /* INCLUDE_TM1628_HPP_ */
