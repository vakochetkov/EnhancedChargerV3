#ifndef INCLUDE_CURRENT_SELECTOR_HPP_
#define INCLUDE_CURRENT_SELECTOR_HPP_

#include "stm32g070kb.hpp"
#include "pin_usage.hpp"


namespace cs_traits {
	enum class channel_t : uint8_t {
		BAT1 = 0,
		BAT2
	};

	enum class current_t : uint8_t {
		CUR_0A5 = 1,
		CUR_1A0 = 2,
		CUR_1A5 = 4,
		CUR_2A0 = 8,
		CUR_2A5 = 16
	};

	template<typename P0A5, typename P1A0,
			 typename P1A5, typename P2A0, typename P2A5>
	struct pin_group_t
	{
		static inline void Init() noexcept {
			P0A5::ConfigAsPP(gpio_traits::speed_t::HIGH, gpio_traits::pullup_t::DOWN);
			P1A0::ConfigAsPP(gpio_traits::speed_t::HIGH, gpio_traits::pullup_t::DOWN);
			P1A5::ConfigAsPP(gpio_traits::speed_t::HIGH, gpio_traits::pullup_t::DOWN);
			P2A0::ConfigAsPP(gpio_traits::speed_t::HIGH, gpio_traits::pullup_t::DOWN);
			P2A5::ConfigAsPP(gpio_traits::speed_t::HIGH, gpio_traits::pullup_t::DOWN);
		}

		static void SetComb(uint8_t comb) noexcept {
			if (comb & 0x1) {
				P0A5::Set();
			} else {
				P0A5::Clear();
			}
			if (comb & 0x2) {
				P1A0::Set();
			} else {
				P1A0::Clear();
			}
			if (comb & 0x4) {
				P1A5::Set();
			} else {
				P1A5::Clear();
			}
			if (comb & 0x8) {
				P2A0::Set();
			} else {
				P2A0::Clear();
			}
			if (comb & 0x10) {
				P2A5::Set();
			} else {
				P2A5::Clear();
			}
		}
	};

	typedef pin_group_t<PB4, PB5, PB6, PB7, PB8>  PinsBat1;
	typedef pin_group_t<PB2, PA8, PA9, PC6, PA10> PinsBat2;
}

template<cs_traits::channel_t TChannel, typename TPins>
class current_selector_c {
	static constexpr auto channel = TChannel;
	using pins = TPins;

	static cs_traits::current_t current;

public:
	static inline void Init() noexcept {
		pins::Init();
		Set(cs_traits::current_t::CUR_0A5);
	}

	/**
	 * Set current limit
	 */
	static void Set(cs_traits::current_t _current) noexcept {
		pins::SetComb(static_cast<uint8_t>(_current));
		current = _current;
	}

	/**
	 *	Release all gpio lines for default IC current
	 */
	static inline void Reset() noexcept {
		pins::SetComb(0);
		current = cs_traits::current_t::CUR_2A5;
	}

	static inline cs_traits::current_t Get() noexcept {
		return current;
	}

	static void Next() noexcept {
		auto newCurrent = (current == cs_traits::current_t::CUR_2A5)
				? cs_traits::current_t::CUR_0A5
				: static_cast<cs_traits::current_t>((static_cast<uint8_t>(current) << 1));
		Set(newCurrent);
	}
};

template<cs_traits::channel_t TChannel, typename TPins>
cs_traits::current_t current_selector_c<TChannel, TPins>::current = cs_traits::current_t::CUR_0A5;


typedef current_selector_c<cs_traits::channel_t::BAT1, cs_traits::PinsBat1> CURSEL1;
typedef current_selector_c<cs_traits::channel_t::BAT2, cs_traits::PinsBat2> CURSEL2;


#endif /* INCLUDE_CURRENT_SELECTOR_HPP_ */
