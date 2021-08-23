#ifndef INCLUDE_INDICATOR_HPP_
#define INCLUDE_INDICATOR_HPP_

#include "tm1628_drv.hpp"


enum class indicator_t : uint8_t {
	IND1,
	IND2
};

class indicator_base_c {
	typedef enum : uint8_t  {
		I1 = 0x0,
		I2 = 0x2,
		I3 = 0x4,
		I4 = 0x6,
	} digit_index_t;

	static constexpr uint8_t digits[] = {
		0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f // 0..9
	};
	static constexpr uint8_t digitsSize = (sizeof(digits) / sizeof(digits[0]));

	static constexpr uint8_t letters[] = {
		0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x54, 0x5C, 0x3E, 0x77, 0x06, 0x38
	}; // A		b	  C		D	  E		F     n		o	  V		A	  I		L
	static constexpr uint8_t lettersSize = (sizeof(letters) / sizeof(letters[0]));

	static constexpr uint8_t miscSymbols[] = {
		0x80, 0x40, 0x48
	}; // .	 	-	 =
	static constexpr uint8_t miscSymbolsSize = (sizeof(miscSymbols) / sizeof(miscSymbols[0]));

	static constexpr uint8_t animationClockwise[] = {
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20
	};
	static constexpr uint8_t animationClockwiseSize =
					(sizeof(animationClockwise) / sizeof(animationClockwise[0]));

public:

	static void Init() noexcept {
		TM1628::Init();
	}

	static void Clear() noexcept {
		TM1628::WriteDigit(I1, 0);
		TM1628::WriteDigit(I2, 0);
		TM1628::WriteDigit(I3, 0);
		TM1628::WriteDigit(I4, 0);
	}

	static void ShowBootLogo() noexcept {
		TM1628::WriteDigit(I1, miscSymbols[0]);
		delay_ms(150);
		TM1628::WriteDigit(I2, miscSymbols[0]);
		delay_ms(150);
		TM1628::WriteDigit(I3, miscSymbols[0]);
		delay_ms(150);
		TM1628::WriteDigit(I4, miscSymbols[0]);
		delay_ms(150);

		for (uint8_t i = 0; i < animationClockwiseSize; i++) {
			TM1628::WriteDigit(I1, animationClockwise[i]);
			TM1628::WriteDigit(I2, animationClockwise[i]);
			TM1628::WriteDigit(I3, animationClockwise[i]);
			TM1628::WriteDigit(I4, animationClockwise[i]);
			delay_ms(100);
		}
		for (uint8_t i = animationClockwiseSize; i > 0; i--) {
			TM1628::WriteDigit(I1, animationClockwise[i-1]);
			TM1628::WriteDigit(I2, animationClockwise[i-1]);
			TM1628::WriteDigit(I3, animationClockwise[i-1]);
			TM1628::WriteDigit(I4, animationClockwise[i-1]);
			delay_ms(100);
		}
		Clear();
	}

	static void ShowTransition(indicator_t ind) noexcept {
		for (uint8_t i = 0; i < animationClockwiseSize; i++) {
			if (ind == indicator_t::IND2) {
				TM1628::WriteDigit(I1, animationClockwise[i]);
				TM1628::WriteDigit(I2, animationClockwise[i]);
			}
			if (ind == indicator_t::IND1) {
				TM1628::WriteDigit(I3, animationClockwise[i]);
				TM1628::WriteDigit(I4, animationClockwise[i]);
			}
			delay_ms(100);
		}
		Clear();
	}

	/*
	 * Blocks execution for 1000ms
	 */
	static void ShowAnimationOverload() noexcept {
		for (uint8_t i = 0; i < 5; i++) { // blink 5 times
			TM1628::WriteDigit(I1, digits[0]);
			TM1628::WriteDigit(I2, letters[11]);
			TM1628::WriteDigit(I3, digits[0]);
			TM1628::WriteDigit(I4, letters[11]);
			delay_ms(100);
			Clear();
			delay_ms(100);
		}
		Clear();
	}

	static void ShowVEqual(indicator_t ind) noexcept {
		if (ind == indicator_t::IND2) {
			TM1628::WriteDigit(I1, letters[8]);
			TM1628::WriteDigit(I2, miscSymbols[2]);
		}
		if (ind == indicator_t::IND1) {
			TM1628::WriteDigit(I3, letters[8]);
			TM1628::WriteDigit(I4, miscSymbols[2]);
		}
	}

	static void ShowAEqual(indicator_t ind) noexcept {
		if (ind == indicator_t::IND2) {
			TM1628::WriteDigit(I1, letters[9]);
			TM1628::WriteDigit(I2, miscSymbols[2]);
		}
		if (ind == indicator_t::IND1) {
			TM1628::WriteDigit(I3, letters[9]);
			TM1628::WriteDigit(I4, miscSymbols[2]);
		}
	}

	static void ShowIEqual(indicator_t ind) noexcept {
		if (ind == indicator_t::IND2) {
			TM1628::WriteDigit(I1, letters[10]);
			TM1628::WriteDigit(I2, miscSymbols[2]);
		}
		if (ind == indicator_t::IND1) {
			TM1628::WriteDigit(I3, letters[10]);
			TM1628::WriteDigit(I4, miscSymbols[2]);
		}
	}

	static void ShowMilli(uint16_t units, indicator_t ind) noexcept {
		uint16_t integer = units / 1000;
		uint16_t fractional = units % 1000;
		uint16_t portion = fractional / 100;
		if (fractional - (portion * 100) >= 50) {
			fractional = portion + 1;
		} else {
			fractional = portion;
		}

		if (integer > 9) {
			integer = 9;
		}
		if (fractional > 9) {
			fractional = 9;
		}

		if (ind == indicator_t::IND2) {
			TM1628::WriteDigit(I1, digits[integer] | miscSymbols[0]);
			TM1628::WriteDigit(I2, digits[fractional]);
		} else {
			TM1628::WriteDigit(I3, digits[integer] | miscSymbols[0]);
			TM1628::WriteDigit(I4, digits[fractional]);
		}
	}

	static void ShowValue(indicator_t ind, uint8_t value, bool with_dot = false) noexcept {
		if (value > 99) {
			return;
		}
		uint8_t dec  = value / 10;
		uint8_t ones = value % 10;

		if (ind == indicator_t::IND2) {
			TM1628::WriteDigit(I1, digits[dec]  | miscSymbols[0]);
			TM1628::WriteDigit(I2, (with_dot) ? digits[ones] | miscSymbols[0] : digits[ones]);
		} else {
			TM1628::WriteDigit(I3, digits[dec]  | miscSymbols[0]);
			TM1628::WriteDigit(I4, (with_dot) ? digits[ones] | miscSymbols[0] : digits[ones]);
		}
	}

};

typedef indicator_base_c IND;


#endif /* INCLUDE_INDICATOR_HPP_ */
