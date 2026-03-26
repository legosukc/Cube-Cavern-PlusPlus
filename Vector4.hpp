
namespace Math {

	class Vector4 {

#ifdef USE_SIMD_INTRINSICS
		inline __m128 CreateSIMDVector() const {
			return _mm_loadu_ps(reinterpret_cast<const float*>(this));
		}
#endif

	public:
		inline Vector4() = default;
		constexpr explicit Vector4(float _X, float _Y, float _Z, float _W) : X(_X), Y(_Y), Z(_Z), W(_W) {}
		constexpr explicit Vector4(float Value) : Vector4(Value, Value, Value, Value) {};

		float X, Y, Z, W;

		constexpr float Dot(const Vector4& B) const;
		constexpr float Magnitude() const;

#ifdef USE_SIMD_INTRINSICS
		inline Vector4 Normalize() const;
#else
		constexpr Vector4 Normalize() const;
#endif
	};


	constexpr Vector4 operator+(const Vector4& A, const Vector4& B) {
		return Vector4(A.X + B.X, A.Y + B.Y, A.Z + B.Z, A.W + B.W);
	}
	constexpr Vector4 operator+(const Vector4& A, float B) {
		return Vector4(A.X + B, A.Y + B, A.Z + B, A.W + B);
	}

	constexpr void operator+=(Vector4& A, const Vector4& B) {
		A = A + B;
	}
	constexpr void operator+=(Vector4& A, float B) {
		A = A + B;
	}


	constexpr Vector4 operator-(const Vector4& A, const Vector4& B) {
		return Vector4(A.X - B.X, A.Y - B.Y, A.Z - B.Z, A.W - B.W);
	}
	constexpr Vector4 operator-(const Vector4& A, float B) {
		return Vector4(A.X - B, A.Y - B, A.Z - B, A.W - B);
	}
	constexpr Vector4 operator-(float A, const Vector4& B) {
		return Vector4(A - B.X, A - B.Y, A - B.Z, A - B.W);
	}
	constexpr Vector4 operator-(const Vector4& A) {
		return Vector4(-A.X, -A.Y, -A.Z, -A.W);
	}

	constexpr void operator-=(Vector4& A, const Vector4& B) {
		A = A - B;
	}
	constexpr void operator-=(Vector4& A, float B) {
		A = A - B;
	}


	constexpr Vector4 operator*(const Vector4& A, const Vector4& B) {
		return Vector4(A.X * B.X, A.Y * B.Y, A.Z * B.Z, A.W * B.W);
	}
	constexpr Vector4 operator*(const Vector4& A, float B) {
		return Vector4(A.X * B, A.Y * B, A.Z * B, A.W * B);
	}

	constexpr void operator*=(Vector4& A, const Vector4& B) {
		A = A * B;
	}
	constexpr void operator*=(Vector4& A, float B) {
		A = A * B;
	}


	constexpr Vector4 operator/(const Vector4& A, const Vector4& B) {
		return Vector4(A.X / B.X, A.Y / B.Y, A.Z / B.Z, A.W / B.W);
	}
	constexpr Vector4 operator/(const Vector4& A, float B) {
		return Vector4(A.X / B, A.Y / B, A.Z / B, A.W / B);
	}
	constexpr Vector4 operator/(float A, const Vector4& B) {
		return Vector4(A / B.X, A / B.Y, A / B.Z, A / B.W);
	}

	constexpr void operator/=(Vector4& A, const Vector4& B) {
		A = A / B;
	}
	constexpr void operator/=(Vector4& A, float B) {
		A = A / B;
	}


	constexpr float Vector4::Dot(const Vector4& B) const {

#ifdef USE_SIMD_INTRINSICS

		const __m128 V1 = this->CreateSIMDVector();
		const __m128 V2 = B.CreateSIMDVector();

		return (V1[0] * V2[0]) + (V1[1] * V2[1]) + (V1[2] * V2[2]) + (V1[3] * V2[3]);
#else
		return (this->X * B.X) + (this->Y * B.Y) + (this->Z * B.Z) + (this->W * B.W);
#endif
	}

	constexpr float Vector4::Magnitude() const {

#ifdef USE_SIMD_INTRINSICS
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(this->Dot(*this))));
#else
		return std::sqrt(this->Dot(*this));
#endif
	}


#ifdef USE_SIMD_INTRINSICS
	Vector4 Vector4::Normalize() const {
		const __m128 MVector = this->CreateSIMDVector();

		//__m128 MagnitudeVector = _mm_mul_ps(MVector, MVector);
		//MagnitudeVector = _mm_rsqrt_ss(_mm_hadd_ps(MagnitudeVector, MagnitudeVector));

		const __m128 Result = _mm_mul_ps(MVector, _mm_rsqrt_ps(_mm_set1_ps(this->Dot(*this)))); //_mm_mul_ps(MVector, _mm_moveldup_ps(MagnitudeVector));

		Vector4 ResultVector;
		_mm_storeu_ps(reinterpret_cast<float*>(&ResultVector), Result);

		return ResultVector;
#else
	constexpr Vector4 Vector4::Normalize() const {
		return *this * (1.f / this->Magnitude()));
#endif
	}
}