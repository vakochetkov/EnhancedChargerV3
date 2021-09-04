#ifndef INCLUDE_CONTROLLER_HPP_
#define INCLUDE_CONTROLLER_HPP_

#include <cmath>
#include "timeout.hpp"
#include "adc.hpp"
#include "indicator.hpp"
#include "current_selector.hpp"
#include "button.hpp"


using cs_traits::current_t;
using cs_traits::channel_t;


enum class ctrl_mode_t {
	SHOW,
	CSET
};


class controller_c {
	enum class show_mode_t : uint8_t {
		VOLTAGE_PRE,
		VOLTAGE_POST,
		CURRENT_PRE,
		CURRENT_POST,
		LAST_STATE
	};

	static constexpr uint16_t CURRENT_LIMIT_MV = 4050;
	static constexpr uint16_t MILLIVOLT_HYSTERESIS = 20;
	static constexpr uint16_t timeoutMs = 3000;
	static constexpr uint16_t showTimeoutMs = 1500;

	static uint16_t voltageBat1;
	static uint16_t voltageBat2;

	static ctrl_mode_t modeBat1;
	static ctrl_mode_t modeBat2;
	static Timeout t1;
	static Timeout t2;

	static void measureVoltages() noexcept {
		voltageBat1 = ADCD::MeasureVoltage(static_cast<uint8_t>(channel_t::BAT1));
		voltageBat2 = ADCD::MeasureVoltage(static_cast<uint8_t>(channel_t::BAT2));
	}

	static void checkCurrentLimiter() noexcept {
		static bool firstTime = true;

		measureVoltages();
		if ((voltageBat1 < CURRENT_LIMIT_MV)
			&& (voltageBat2 < CURRENT_LIMIT_MV)) {

			CURSEL1::SetMax(current_t::CUR_1A0);
			CURSEL2::SetMax(current_t::CUR_1A0);

			if (firstTime) {
				if (static_cast<uint8_t>(CURSEL1::Get()) > static_cast<uint8_t>(current_t::CUR_1A0)) {
					CURSEL1::Set(current_t::CUR_1A0);
				}

				if (static_cast<uint8_t>(CURSEL2::Get()) > static_cast<uint8_t>(current_t::CUR_1A0)) {
					CURSEL2::Set(current_t::CUR_1A0);
				}

				// IND::ShowAnimationOverload(); //disable animation
				firstTime = false;
			}
		} else {
			CURSEL1::SetMax(current_t::CUR_2A5);
			CURSEL2::SetMax(current_t::CUR_2A5);
			firstTime = true;
		}
	}

	static void changeCurrent(channel_t channel) {
		if (channel == channel_t::BAT1) {
			CURSEL1::Next();
		} else if (channel == channel_t::BAT2) {
			CURSEL2::Next();
		}
	}

	static uint8_t currentToValue(current_t cur) noexcept {
		uint8_t pow = 1;
		uint8_t number = static_cast<uint8_t>(cur);
		while (number >>= 1) pow++;
		return (pow * 5); // log2(cur) * 0.5A
	}

	static void showVoltage(indicator_t ind) noexcept {
		static Timeout t;
		static uint16_t showVoltage1 = 0;
		static uint16_t showVoltage2 = 0;

		if (!t.IsSet()) { // update shown digits every x ms + hysteresis
			// cast to unsigned is necessary!
			if (static_cast<uint16_t>(voltageBat1 - showVoltage1) > MILLIVOLT_HYSTERESIS) {
				showVoltage1 = voltageBat1;
			}
			if (static_cast<uint16_t>(voltageBat2 - showVoltage2) > MILLIVOLT_HYSTERESIS) {
				showVoltage2 = voltageBat2;
			}
			t.Set(showTimeoutMs);
		} else {
			t.Update();
			if (t.IsTimeOut()) {
				t.Clear();
			}
		}

		if (ind == indicator_t::IND1) {
			IND::ShowMilli(showVoltage1, ind);
		} else if (ind == indicator_t::IND2) {
			IND::ShowMilli(showVoltage2, ind);
		}
	}

	static void showCurrent(indicator_t ind) noexcept {
		if (ind == indicator_t::IND1) {
			IND::ShowValue(ind, currentToValue(CURSEL1::Get()));
		} else if (ind == indicator_t::IND2) {
			IND::ShowValue(ind, currentToValue(CURSEL2::Get()));
		}
	}

	static void showCurrentWithDot(indicator_t ind) noexcept {
		if (ind == indicator_t::IND1) {
			IND::ShowValue(ind, currentToValue(CURSEL1::Get()), true);
		} else if (ind == indicator_t::IND2) {
			IND::ShowValue(ind, currentToValue(CURSEL2::Get()), true);
		}
	}

	static show_mode_t getNextState(show_mode_t currentState) noexcept {
		static Timeout t;

		if (!t.IsSet()) {
			t.Set(showTimeoutMs);
		} else {
			t.Update();
			if (t.IsTimeOut()) {
				t.Clear();
				return static_cast<show_mode_t>(
						(static_cast<uint8_t>(currentState) + 1) %
						 static_cast<uint8_t>(show_mode_t::LAST_STATE)
				);
			}
		}
		return currentState;
	}

	static void showSequencer(indicator_t ind) noexcept {
		static show_mode_t mode = show_mode_t::VOLTAGE_PRE;

		mode = getNextState(mode);

		if (mode == show_mode_t::VOLTAGE_PRE) {
			IND::ShowVEqual(ind);
		} else if (mode == show_mode_t::VOLTAGE_POST) {
			showVoltage(ind);
		} else if (mode == show_mode_t::CURRENT_PRE) {
			IND::ShowAEqual(ind);
		} else if (mode == show_mode_t::CURRENT_POST) {
			showCurrent(ind);
		}
	}

public:

	static void Init() noexcept {
		CURSEL1::Init();
		CURSEL2::Init();

		BTN::Init();
		ADCD::Init();

		IND::Init();
		IND::ShowBootLogo();

		CURSEL1::Set(current_t::CUR_0A5);
		CURSEL2::Set(current_t::CUR_0A5);
	}

	static void Tick() noexcept {
		if (t1.IsSet()) {
			t1.Update();
		}
		if (t2.IsSet()) {
			t2.Update();
		}

		checkCurrentLimiter();

		// handle bat1
		if (modeBat1 == ctrl_mode_t::SHOW) {
			if (BTN::IsBtn1Pressed()) {
				IND::ShowTransition(indicator_t::IND1);
				t1.Set(timeoutMs);
				modeBat1 = ctrl_mode_t::CSET;
			}

			// show voltage and current
			showSequencer(indicator_t::IND1);

		} else if (modeBat1 == ctrl_mode_t::CSET) {
			if (t1.IsTimeOut()) {
				IND::ShowTransition(indicator_t::IND1);
				t1.Clear();
				modeBat1 = ctrl_mode_t::SHOW;
			}

			if (BTN::IsBtn1Pressed()) {
				t1.Clear();
				t1.Set(timeoutMs);
				changeCurrent(channel_t::BAT1);
			}
			showCurrentWithDot(indicator_t::IND1);
		}

		// handle bat2
		if (modeBat2 == ctrl_mode_t::SHOW) {
			// show voltage and current
			showSequencer(indicator_t::IND2);

			if (BTN::IsBtn2Pressed()) {
				IND::ShowTransition(indicator_t::IND2);
				t2.Set(timeoutMs);
				modeBat2 = ctrl_mode_t::CSET;
			}

		} else if (modeBat2 == ctrl_mode_t::CSET) {
			if (t2.IsTimeOut()) {
				IND::ShowTransition(indicator_t::IND2);
				t2.Clear();
				modeBat2 = ctrl_mode_t::SHOW;
			}

			if (BTN::IsBtn2Pressed()) {
				t2.Clear();
				t2.Set(timeoutMs);
				changeCurrent(channel_t::BAT2);
			}
			showCurrentWithDot(indicator_t::IND2);
		}
	}

};

uint16_t controller_c::voltageBat1 = 0;
uint16_t controller_c::voltageBat2 = 0;

ctrl_mode_t controller_c::modeBat1 = ctrl_mode_t::SHOW;
ctrl_mode_t controller_c::modeBat2 = ctrl_mode_t::SHOW;

Timeout controller_c::t1;
Timeout controller_c::t2;

typedef controller_c CTRL;

#endif /* INCLUDE_CONTROLLER_HPP_ */
