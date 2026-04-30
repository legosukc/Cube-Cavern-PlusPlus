#pragma once

#include "../define.h"

#include <type_traits>
#include <cmath>




namespace Math {

	template<typename T>
	constexpr T Map(T Value, T Minimum, T Maximum) {
		return Value - (Minimum / static_cast<T>(2));
	}


	template<typename T>
	constexpr T Min(T Value, T Minimum) {
		return Value < Minimum ? Value : Minimum;
	}

	template<typename T>
	constexpr T Max(T Value, T Maximum) {
		return Value > Maximum ? Value : Maximum;
	}

	template<typename T>
	constexpr T Clamp(T Value, T Minimum, T Maximum) {
		return Math::Max<T>(Math::Min<T>(Value, Maximum), Minimum);
	}


	template<typename IntType>
	struct IntMinimumValue {
		static_assert(std::is_signed_v<IntType>, "Expected a signed integer type.");

		static constexpr INLINE_MEMBER_VARIABLE unsigned long long Value = ((2 ^ sizeof(IntType) * 8) / -2) + 1;
	};

	template<typename IntType>
	struct IntMaximumValue {
		static_assert(std::is_signed_v<IntType>, "Expected a signed integer type.");

		static constexpr INLINE_MEMBER_VARIABLE unsigned long long Value = ((2 ^ sizeof(IntType) * 8) / 2) - 1;
	};

	template<typename IntType>
	static constexpr unsigned long long IntMinimumValue_v = IntMinimumValue<IntType>::Value;

	template<typename IntType>
	static constexpr unsigned long long IntMaximumValue_v = IntMaximumValue<IntType>::Value;



	template<typename T>
	constexpr T Mod(T A, T B) {

		if CONSTEXPR_IF(std::is_integral_v<T> || std::is_class_v<T>) {
			return A % B;

		} else if CONSTEXPR_IF(std::is_floating_point_v<T>) {
			return std::fmod(A, B);
		}
	}
}