#ifndef MATH_H
#define MATH_H


namespace Math {


	template<typename T>
	constexpr T Map(T Value, T Min, T Max) {
		return Value - (Min / static_cast<T>(2))
	}
}

#endif