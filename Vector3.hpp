
namespace Math {

	class Vector3 {

		constexpr inline __m128 CreateSIMDVector() const;

	public:
		constexpr inline Vector3() : X(0.f), Y(0.f), Z(0.f) {};
		constexpr inline Vector3(float _X, float _Y, float _Z) : X(_X), Y(_Y), Z(_Z) {};
		constexpr inline Vector3(float Value) : Vector3(Value, Value, Value) {};

		union {
			float X, R;
		};
		union {
			float Y, G;
		};
		union {
			float Z, B;
		};

		template<typename T>
		constexpr inline float& operator[](T i);

		template<typename T>
		constexpr inline float operator[](T i) const;


		constexpr inline Math::Vector3 operator+(const Math::Vector3& B) const;
		constexpr inline Math::Vector3 operator+(float B) const;

		constexpr inline void operator+=(const Math::Vector3& B);
		constexpr inline void operator+=(float B);


		constexpr inline Math::Vector3 operator-(const Math::Vector3& B) const;
		constexpr inline Math::Vector3 operator-(float B) const;
		constexpr inline Math::Vector3 operator-() const;

		constexpr inline void operator-=(const Math::Vector3& B);
		constexpr inline void operator-=(float B);


		constexpr inline Math::Vector3 operator*(const Math::Vector3& B) const;
		constexpr inline Math::Vector3 operator*(float B) const;

		constexpr inline void operator*=(const Math::Vector3& B);
		constexpr inline void operator*=(float B);


		constexpr inline Math::Vector3 operator/(const Math::Vector3& B) const;
		constexpr inline Math::Vector3 operator/(float B) const;

		constexpr inline void operator/=(const Math::Vector3& B);
		constexpr inline void operator/=(float B);


		constexpr inline Math::Vector3 operator%(const Math::Vector3& B) const;
		constexpr inline Math::Vector3 operator%(float B) const;

		constexpr inline void operator%=(const Math::Vector3& B);
		constexpr inline void operator%=(float B);



		constexpr float Dot(const Math::Vector3& B) const;
		constexpr float Magnitude() const;

		SSE3_FUNCTION
		constexpr Math::Vector3 Normalize() const;
	};
}

constexpr inline Math::Vector3 operator+(float A, const Math::Vector3& B);
constexpr inline Math::Vector3 operator-(float A, const Math::Vector3& B);
constexpr inline Math::Vector3 operator*(float A, const Math::Vector3& B);
constexpr inline Math::Vector3 operator/(float A, const Math::Vector3& B);
constexpr inline Math::Vector3 operator%(float A, const Math::Vector3& B);



template<typename T>
constexpr float& Math::Vector3::operator[](T i) {
	return reinterpret_cast<float*>(this)[i];
}

template<typename T>
constexpr float Math::Vector3::operator[](T i) const {
	return reinterpret_cast<const float*>(this)[i];
}

constexpr Math::Vector3 operator+(float A, const Math::Vector3& B) {
	return Math::Vector3(A + B.X, A + B.Y, A + B.Z);
}

constexpr Math::Vector3 Math::Vector3::operator-(float B) const {
	return Math::Vector3(this->X - B, this->Y - B, this->Z - B);
}

constexpr Math::Vector3 operator*(float A, const Math::Vector3& B) {
	return Math::Vector3(A * B.X, A * B.Y, A * B.Z);
}

constexpr Math::Vector3 operator/(float A, const Math::Vector3& B) {
	return Math::Vector3(A / B.X, A / B.Y, A / B.Z);
}

constexpr Math::Vector3 operator%(float A, const Math::Vector3& B) {
	return Math::Vector3(std::fmod(A, B.X), std::fmod(A, B.Y), std::fmod(A, B.Z));
}



constexpr __m128 Math::Vector3::CreateSIMDVector() const {
	return _mm_setr_ps(this->X, this->Y, this->Z, 0.f);
}


constexpr Math::Vector3 Math::Vector3::operator+(const Math::Vector3& B) const {
	return Math::Vector3(this->X + B.X, this->Y + B.Y, this->Z + B.Z);
}

constexpr Math::Vector3 Math::Vector3::operator+(float B) const {
	return Math::Vector3(this->X + B, this->Y + B, this->Z + B);
}


constexpr void Math::Vector3::operator+=(const Math::Vector3& B) {
	*this = this->operator+(B);
}
constexpr void Math::Vector3::operator+=(float B) {
	*this = this->operator+(B);
}



constexpr Math::Vector3 Math::Vector3::operator-(const Math::Vector3& B) const {
	return Math::Vector3(this->X - B.X, this->Y - B.Y, this->Z - B.Z);
}

constexpr Math::Vector3 operator-(float A, const Math::Vector3& B) {
	return Math::Vector3(A - B.X, A - B.Y, A - B.Z);
}

constexpr Math::Vector3 Math::Vector3::operator-() const {
	return Math::Vector3(-this->X, -this->Y, -this->Z);
}


constexpr void Math::Vector3::operator-=(const Math::Vector3& B) {
	*this = this->operator-(B);
}

constexpr void Math::Vector3::operator-=(float B) {
	*this = this->operator-(B);
}



constexpr Math::Vector3 Math::Vector3::operator*(const Math::Vector3& B) const {
	return Math::Vector3(this->X * B.X, this->Y * B.Y, this->Z * B.Z);
}

constexpr Math::Vector3 Math::Vector3::operator*(float B) const {
	return Math::Vector3(this->X * B, this->Y * B, this->Z * B);
}


constexpr void Math::Vector3::operator*=(const Math::Vector3& B) {
	*this = this->operator*(B);
}

constexpr void Math::Vector3::operator*=(float B) {
	*this = this->operator*(B);
}



constexpr Math::Vector3 Math::Vector3::operator/(const Math::Vector3& B) const {
	return Math::Vector3(this->X / B.X, this->Y / B.Y, this->Z / B.Z);
}

constexpr Math::Vector3 Math::Vector3::operator/(float B) const {
	return Math::Vector3(this->X / B, this->Y / B, this->Z / B);
}


constexpr void Math::Vector3::operator/=(const Math::Vector3& B) {
	*this = this->operator/(B);
}

constexpr void Math::Vector3::operator/=(float B) {
	*this = this->operator/(B);
}



constexpr Math::Vector3 Math::Vector3::operator%(const Math::Vector3& B) const {
	return Math::Vector3(std::fmod(this->X, B.X), std::fmod(this->Y, B.Y), std::fmod(this->Z, B.Z));
}

constexpr Math::Vector3 Math::Vector3::operator%(float B) const {
	return Math::Vector3(std::fmod(this->X, B), std::fmod(this->Y, B), std::fmod(this->Z, B));
}


constexpr void Math::Vector3::operator%=(const Math::Vector3& B) {
	*this = this->operator%(B);
}

constexpr void Math::Vector3::operator%=(float B) {
	*this = this->operator%(B);
}



constexpr float Math::Vector3::Dot(const Math::Vector3& B) const {

#if defined(USE_SIMD_INTRINSICS)

	const __m128 V1 = this->CreateSIMDVector();
	const __m128 V2 = B.CreateSIMDVector();

	return (V1[0] * V2[0]) + (V1[1] * V2[1]) + (V1[2] * V2[2]);
#else
	return (this->X * B.X) + (this->Y * B.Y) + (this->Z * B.Z);
#endif
}

constexpr float Math::Vector3::Magnitude() const {
	return std::sqrt(this->Dot(*this));
}

SSE3_FUNCTION
constexpr Math::Vector3 Math::Vector3::Normalize() const {

#if defined(USE_SIMD_INTRINSICS)
	return *this * _mm_rsqrt_ss(_mm_set_ss(this->Dot(*this)))[0];
#else
	return *this * 1.f / std::sqrt(this->Dot(*this));
#endif
}