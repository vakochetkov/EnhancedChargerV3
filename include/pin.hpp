/*
 * pin.hpp
 *
 *  Created on: 31 мар. 2021 г.
 *      Author: kochetkov
 */

#ifndef INCLUDE_PIN_HPP_
#define INCLUDE_PIN_HPP_

#include "port.hpp"

template<gpio_traits::port_t TPort, uint8_t TPin>
class pin_c {

public:
	using PORT = port_c<TPort>;

	static constexpr void ConfigAsIn() noexcept {
		static_assert(TPin >= 0 && TPin <= 15, "Pin number not in range!");
		PORT::ConfigAsIn(TPin);
	}

	static constexpr void ConfigAsPP() noexcept {
		static_assert(TPin >= 0 && TPin <= 15, "Pin number not in range!");
		PORT::ConfigAsPP(TPin);
	}

	static constexpr void ConfigAsOD() noexcept {
		static_assert(TPin >= 0 && TPin <= 15, "Pin number not in range!");
		PORT::ConfigAsOD(TPin);
	}

	static constexpr void ConfigAsAF() noexcept {
		static_assert(TPin >= 0 && TPin <= 15, "Pin number not in range!");
		PORT::ConfigAsAF(TPin);
	}

};


#endif /* INCLUDE_PIN_HPP_ */
