/*
 * template_map.hpp
 *
 *  Created on: 8 мая 2021 г.
 *      Author: kochetkov
 */

#ifndef INCLUDE_TEMPLATE_MAP_HPP_
#define INCLUDE_TEMPLATE_MAP_HPP_

#include <type_traits>

namespace template_map {

/*
 *  Usage:
 * 	using keys =   KeyValueContainer<int, 1, 2, 3>;
 * 	using values = KeyValueContainer<int, 4, 5, 6>;
 * 	constexpr template_map::Map<keys, values> testMap;
 * 	static_assert (testMap[10] == 20, "Error");
 * 	static_assert (testMap[11] == 21, "Error");
 * 	static_assert (testMap[23] == 70, "Error");
 */

template<typename MemberType, MemberType... values>
struct KeyValueContainer {
	using Type = MemberType;
	constexpr static MemberType members[] { values... };
	constexpr static size_t size { sizeof...(values) };
};

template<typename keys, typename values>
class Map
{
	using ValueType = typename values::Type;
	using KeyType = typename keys::Type;

	constexpr static auto size = keys::size;

    constexpr ValueType _find (KeyType key, size_t i = 0) const noexcept {
        return (i == size) ?
        		_keyNotFound() :
               (keys::members[i] == key) ? values::members[i] : _find(key, i + 1);
    }

    // It is not constexpr, so it should throw an error in compile-time
    // It will be silent in run-time!
    static ValueType _keyNotFound() noexcept {
        return {};
    }

public:
    constexpr ValueType operator[] (KeyType key) const {
        return _find(key);
    }

    constexpr ValueType at(KeyType key) const {
    	return _find(key);
    }

    constexpr ValueType get(KeyType key) const {
    	return _find(key);
    }
};

}




#endif /* INCLUDE_TEMPLATE_MAP_HPP_ */
