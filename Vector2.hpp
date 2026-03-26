
namespace {

	template<typename T, typename _SIMD_VecType, class _DerivedType>
	class _base_vector2 {
		static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>, "Unexpected _base_vector2 template parameter. Expected floating point or integral.");

	protected:
		inline _base_vector2() {}
		constexpr explicit _base_vector2(T _X, T _Y) : X(_X), Y(_Y) {}
		constexpr _base_vector2(T Value) : X(Value), Y(Value) {}

		using SIMDVectorType = _SIMD_VecType;
#ifdef USE_SIMD_INTRINSICS
		constexpr SIMDVectorType CreateSIMDVector() const;
#endif

	public:
		T X, Y;

		using ComponentType = T;

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

		constexpr _DerivedType operator%(const _DerivedType& B) const;
		constexpr _DerivedType operator%(ComponentType B) const;

		constexpr void operator%=(const _DerivedType& B) {
			*this = this->operator%(B);
		}

		constexpr void operator%=(ComponentType B) {
			*this = this->operator%(B);
		}


		constexpr ComponentType Dot(const _DerivedType& B) const {

#ifdef USE_SIMD_INTRINSICS
			SIMDVectorType V1 = static_cast<const _DerivedType*>(this)->CreateSIMDVector();
			SIMDVectorType V2 = B.CreateSIMDVector();

			return (V1[0] * V2[0]) + (V1[1] * V2[1]);
#else
			return (this->X * B.X) + (this->Y * B.Y);
#endif
		}

		constexpr ComponentType Magnitude() const {
			return static_cast<ComponentType>(sqrtf(static_cast<float>(this->Dot(*this))));
		}
	};
}

namespace Math {
	class Vector2;
	class IVector2;
}

class Math::Vector2 : public ::_base_vector2<float, __m128, Math::Vector2> {

	friend _base_vector2;

#ifdef USE_SIMD_INTRINSICS
	constexpr SIMDVectorType CreateSIMDVector() const {
		return _mm_setr_ps(this->X, this->Y, 0.f, 0.f);
	}
#endif

public:
	inline Vector2() : _base_vector2() {}
	constexpr explicit Vector2(float _X, float _Y) : _base_vector2(_X, _Y) {}
	constexpr Vector2(float Value) : _base_vector2(Value) {}

	/*
	constexpr Math::Vector2 operator+(const Math::Vector2& B) const;
	constexpr Math::Vector2 operator+(float B) const;

	constexpr inline void operator+=(const Math::Vector2& B);
	constexpr inline void operator+=(float B);


	constexpr inline Math::Vector2 operator-(const Math::Vector2& B) const;
	constexpr inline Math::Vector2 operator-(float B) const;
	constexpr inline Math::Vector2 operator-() const;

	constexpr inline void operator-=(const Math::Vector2& B);
	constexpr inline void operator-=(float B);


	constexpr inline Math::Vector2 operator*(const Math::Vector2& B) const;
	constexpr inline Math::Vector2 operator*(float B) const;

	constexpr inline void operator*=(const Math::Vector2& B);
	constexpr inline void operator*=(float B);


	constexpr inline Math::Vector2 operator/(const Math::Vector2& B) const;
	constexpr inline Math::Vector2 operator/(float B) const;

	constexpr inline void operator/=(const Math::Vector2& B);
	constexpr inline void operator/=(float B);


	constexpr inline Math::Vector2 operator%(const Math::Vector2& B) const;
	constexpr inline Math::Vector2 operator%(float B) const;

	constexpr inline void operator%=(const Math::Vector2& B);
	constexpr inline void operator%=(float B);*/


	constexpr Math::Vector2 operator%(const Math::Vector2& B) const {
		return Math::Vector2(fmodf(this->X, B.X), fmodf(this->Y, B.Y));
	}
	constexpr Math::Vector2 operator%(float B) const {
		return Math::Vector2(fmodf(this->X, B), fmodf(this->Y, B));
	}


	constexpr operator Math::IVector2() const;

	constexpr Math::Vector2 Normalize() const {

#ifdef USE_SIMD_INTRINSICS
		return this->operator*(_mm_rsqrt_ss(_mm_set_ss(this->Dot(*this)))[0]);
#else
		return this->operator*(1.f / sqrtf(this->Dot(*this)));
#endif
	}
};






class Math::IVector2 : public _base_vector2<int, __m64, Math::IVector2> {

	friend _base_vector2;
#ifdef USE_SIMD_INTRINSICS
	constexpr SIMDVectorType CreateSIMDVector() const {
		return _mm_setr_pi32(this->X, this->Y);
	}
#endif

public:
	inline IVector2() : _base_vector2() {}
	constexpr explicit IVector2(int _X, int _Y) : _base_vector2(_X, _Y) {}
	constexpr IVector2(int Value) : _base_vector2(Value) {}


	/*
	constexpr inline Math::IVector2 operator+(const Math::IVector2& B) const;
	constexpr inline Math::IVector2 operator+(int B) const;

	constexpr inline void operator+=(const Math::IVector2& B);
	constexpr inline void operator+=(int B);


	constexpr inline Math::IVector2 operator-(const Math::IVector2& B) const;
	constexpr inline Math::IVector2 operator-(int B) const;
	constexpr inline Math::IVector2 operator-() const;

	constexpr inline void operator-=(const Math::IVector2& B);
	constexpr inline void operator-=(int B);


	constexpr inline Math::IVector2 operator*(const Math::IVector2& B) const;
	constexpr inline Math::IVector2 operator*(int B) const;

	constexpr inline void operator*=(const Math::IVector2& B);
	constexpr inline void operator*=(int B);


	constexpr inline Math::IVector2 operator/(const Math::IVector2& B) const;
	constexpr inline Math::IVector2 operator/(int B) const;

	constexpr inline void operator/=(const Math::IVector2& B);
	constexpr inline void operator/=(int B);


	constexpr inline Math::IVector2 operator%(const Math::IVector2& B) const;
	constexpr inline Math::IVector2 operator%(int B) const;

	constexpr inline void operator%=(const Math::IVector2& B);
	constexpr inline void operator%=(int B);*/


	constexpr operator Math::Vector2() const;
};
/*
constexpr Math::IVector2 operator+(int A, const Math::IVector2& B);
constexpr Math::IVector2 operator-(int A, const Math::IVector2& B);
constexpr Math::IVector2 operator*(int A, const Math::IVector2& B);
constexpr Math::IVector2 operator/(int A, const Math::IVector2& B);
constexpr Math::IVector2 operator%(int A, const Math::IVector2& B);
*/



constexpr Math::Vector2::operator Math::IVector2() const {
	return Math::IVector2(static_cast<int>(this->X), static_cast<int>(this->Y));
}

constexpr Math::IVector2::operator Math::Vector2() const {
	return Math::Vector2(static_cast<float>(this->X), static_cast<float>(this->Y));
}



void test() {
	
}