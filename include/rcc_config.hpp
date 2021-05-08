/*
 * rcc_config.hpp
 *
 *  Created on: 1 апр. 2021 г.
 *      Author: kochetkov
 */

#ifndef INCLUDE_RCC_CONFIG_HPP_
#define INCLUDE_RCC_CONFIG_HPP_


#include "rcc.hpp"

static constexpr rcc_traits::pll_config_t pll_config = { 2, 16, 2, 2};

typedef rcc_c <
		rcc_traits::hsclk_src_t::HSI, 16000000,
		pll_config,
		1, 1
		> rcc;


#endif /* INCLUDE_RCC_CONFIG_HPP_ */
