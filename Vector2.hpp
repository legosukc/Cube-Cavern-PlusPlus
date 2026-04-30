#pragma once

#include "define.h"


#include <SDL2/SDL_stdinc.h>
#include <type_traits>

#include <xmmintrin.h>

#include "FunctionHeaders/Math.hpp"


namespace {

	template<typename _ComponentType, class _DerivedType>
	struct _base_vector2 {

		static constexpr inline int ComponentCount = 2;
		using ComponentType = _ComponentType;

		static_assert(std::is_integral_v<ComponentType> || std::is_floating_point_v<ComponentType>, "Unexpected _base_vector2 template parameter. Expected floating point or integral.");


		inline _base_vector2() {}
		constexpr _base_vector2(ComponentType _X, ComponentType _Y) : X(_X), Y(_Y) {}
		constexpr explicit _base_vector2(ComponentType Value) : X(Value), Y(Value) {}

		union {
			ComponentType X, R, S;
		};
		union {
			ComponentType Y, G, T;
		};

		template<typename IndexType>
		constexpr ComponentType& operator[](IndexType Index) {
			return reinterpret_cast<ComponentType*>(this)[Index];
		}

		template<typename IndexType>
		constexpr const ComponentType& operator[](IndexType Index) const {
			return reinterpret_cast<const ComponentType*>(this)[Index];
		}



		constexpr _DerivedType operator+(const _DerivedType& B) const {
			return _DerivedType(this->X + B.X, this->Y + B.Y);
		}

		constexpr _DerivedType operator+(ComponentType B) const {
			return _DerivedType(this->X + B, this->Y + B);
		}


		constexpr void operator+=(const _DerivedType& B) {
			*this = this->operator+(B);
		}

		constexpr void operator+=(ComponentType B) {
			*this = this->operator+(B);
		}



		constexpr _DerivedType operator-(const _DerivedType& B) const {
			return _DerivedType(this->X - B.X, this->Y - B.Y);
		}

		constexpr _DerivedType operator-(ComponentType B) const {
			return _DerivedType(this->X - B, this->Y - B);
		}

		constexpr _DerivedType operator-() const {
			return _DerivedType(-this->X, -this->Y);
		}


		constexpr void operator-=(const _DerivedType& B) {
			*this = this->operator-(B);
		}
		constexpr void operator-=(ComponentType B) {
			*this = this->operator-(B);
		}



		constexpr _DerivedType operator*(const _DerivedType& B) const {
			return _DerivedType(this->X * B.X, this->Y * B.Y);
		}

		constexpr _DerivedType operator*(ComponentType B) const {
			return _DerivedType(this->X * B, this->Y * B);
		}


		constexpr void operator*=(const _DerivedType& B) {
			*this = this->operator*(B);
		}

		constexpr void operator*=(ComponentType B) {
			*this = this->operator*(B);
		}



		constexpr _DerivedType operator/(const _DerivedType& B) const {
			return _DerivedType(this->X / B.X, this->Y / B.Y);
		}

		constexpr _DerivedType operator/(ComponentType B) const {
			return _DerivedType(this->X / B, this->Y / B);
		}


		constexpr void operator/=(const _DerivedType& B) {
			*this = this->operator/(B);
		}

		constexpr void operator/=(ComponentType B) {
			*this = this->operator/(B);
		}

		constexpr _DerivedType operator%(const _DerivedType& B) const {
			return _DerivedType(Math::Mod<ComponentType>(this->X, B.X), Math::Mod<ComponentType>(this->Y, B.Y));
		}
		constexpr _DerivedType operator%(ComponentType B) const {
			return _DerivedType(Math::Mod<ComponentType>(this->X, B), Math::Mod<ComponentType>(this->Y, B));
		}

		constexpr void operator%=(const _DerivedType& B) {
			*this = this->operator%(B);
		}
		constexpr void operator%=(ComponentType B) {
			*this = this->operator%(B);
		}



		constexpr bool operator==(const _DerivedType& B) const {
			return this->X == B.X && this->Y == B.Y;
		}
		constexpr bool operator!=(const _DerivedType& B) const {
			return this->X != B.X || this->Y != B.Y;
		}



		constexpr ComponentType Dot(const _DerivedType& B) const {
			return (this->X * B.X) + (this->Y * B.Y);
		}

		constexpr ComponentType Magnitude() const {

			if CONSTEXPR_IF(std::is_floating_point_v<ComponentType>) {
				return std::sqrt((this->X * this->X) + (this->Y * this->Y));
			} else {
				return static_cast<ComponentType>(std::sqrtl(static_cast<long double>((this->X * this->X) + (this->Y * this->Y))));
			}
		}
	};
}

namespace Math {
	struct Vector2;
	struct IVector2;
	struct UVector2;
}

struct Math::Vector2 : ::_base_vector2<float, Math::Vector2> {

	inline Vector2() : _base_vector2() {}
	constexpr Vector2(float _X, float _Y) : _base_vector2(_X, _Y) {}
	constexpr explicit Vector2(float Value) : _base_vector2(Value) {}


	constexpr operator Math::IVector2() const;
	constexpr operator Math::UVector2() const;

	constexpr Math::Vector2 Normalize() const {

		return this->operator*(

		#ifdef USE_SIMD_INTRINSICS
			_mm_rsqrt_ss(_mm_set_ss(this->Dot(*this)))[0]
		#else
			1.f / std::sqrt(this->Dot(*this))
		#endif
		);
	}
};

constexpr Math::Vector2 operator+(Math::Vector2::ComponentType A, const Math::Vector2& B) {
	return Math::Vector2(A) + B;
}

constexpr Math::Vector2 operator-(Math::Vector2::ComponentType A, const Math::Vector2& B) {
	return Math::Vector2(A) - B;
}

constexpr Math::Vector2 operator*(Math::Vector2::ComponentType A, const Math::Vector2& B) {
	return Math::Vector2(A) * B;
}

constexpr Math::Vector2 operator/(Math::Vector2::ComponentType A, const Math::Vector2& B) {
	return Math::Vector2(A) / B;
}

constexpr Math::Vector2 operator%(Math::Vector2::ComponentType A, const Math::Vector2& B) {
	return Math::Vector2(A) % B;
}




struct Math::IVector2 : ::_base_vector2<Sint32, Math::IVector2> {

	inline IVector2() : _base_vector2() {}
	constexpr IVector2(Sint32 _X, Sint32 _Y) : _base_vector2(_X, _Y) {}
	constexpr explicit IVector2(Sint32 Value) : _base_vector2(Value) {}

	constexpr operator Math::Vector2() const;
	constexpr operator Math::UVector2() const;
};

constexpr Math::IVector2 operator+(Math::IVector2::ComponentType A, const Math::IVector2& B) {
	return Math::IVector2(A) + B;
}

constexpr Math::IVector2 operator-(Math::IVector2::ComponentType A, const Math::IVector2& B) {
	return Math::IVector2(A) - B;
}

constexpr Math::IVector2 operator*(Math::IVector2::ComponentType A, const Math::IVector2& B) {
	return Math::IVector2(A) * B;
}

constexpr Math::IVector2 operator/(Math::IVector2::ComponentType A, const Math::IVector2& B) {
	return Math::IVector2(A) / B;
}

constexpr Math::IVector2 operator%(Math::IVector2::ComponentType A, const Math::IVector2& B) {
	return Math::IVector2(A) % B;
}





struct Math::UVector2 : ::_base_vector2<Uint32, Math::UVector2> {

	inline UVector2() : _base_vector2() {}
	constexpr UVector2(Uint32 _X, Uint32 _Y) : _base_vector2(_X, _Y) {}
	constexpr explicit UVector2(Uint32 Value) : _base_vector2(Value) {}

	constexpr operator Math::Vector2() const;
	constexpr operator Math::IVector2() const;
};

constexpr Math::UVector2 operator+(Math::UVector2::ComponentType A, const Math::UVector2& B) {
	return Math::UVector2(A) + B;
}

constexpr Math::UVector2 operator-(Math::UVector2::ComponentType A, const Math::UVector2& B) {
	return Math::UVector2(A) - B;
}

constexpr Math::UVector2 operator*(Math::UVector2::ComponentType A, const Math::UVector2& B) {
	return Math::UVector2(A) * B;
}

constexpr Math::UVector2 operator/(Math::UVector2::ComponentType A, const Math::UVector2& B) {
	return Math::UVector2(A) / B;
}

constexpr Math::UVector2 operator%(Math::UVector2::ComponentType A, const Math::UVector2& B) {
	return Math::UVector2(A) % B;
}





constexpr Math::Vector2::operator Math::IVector2() const {
	return Math::IVector2(
		static_cast<Math::IVector2::ComponentType>(this->X),
		static_cast<Math::IVector2::ComponentType>(this->Y)
	);
}

constexpr Math::Vector2::operator Math::UVector2() const {
	return Math::UVector2(
		static_cast<Math::UVector2::ComponentType>(this->X),
		static_cast<Math::UVector2::ComponentType>(this->Y)
	);
}



constexpr Math::IVector2::operator Math::Vector2() const {
	return Math::Vector2(
		static_cast<Math::Vector2::ComponentType>(this->X),
		static_cast<Math::Vector2::ComponentType>(this->Y)
	);
}

constexpr Math::IVector2::operator Math::UVector2() const {
	return Math::UVector2(
		static_cast<Math::UVector2::ComponentType>(this->X),
		static_cast<Math::UVector2::ComponentType>(this->Y)
	);
}



constexpr Math::UVector2::operator Math::Vector2() const {
	return Math::Vector2(
		static_cast<Math::Vector2::ComponentType>(this->X),
		static_cast<Math::Vector2::ComponentType>(this->Y)
	);
}

constexpr Math::UVector2::operator Math::IVector2() const {
	return Math::IVector2(
		static_cast<Math::IVector2::ComponentType>(this->X),
		static_cast<Math::IVector2::ComponentType>(this->Y)
	);
}