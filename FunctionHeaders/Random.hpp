#pragma once

#include <random>

#include <SDL3/SDL_stdinc.h>

#include "../MathClasses/Vector2.hpp"
#include "../MathClasses/Vector3.hpp"

#include "BitOp.hpp"




namespace {

	static thread_local Uint64 _Random_Seed = static_cast<Uint64>(std::random_device()());

	// Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs"
	static Uint64 xorshift() noexcept {
		::_Random_Seed ^= ::_Random_Seed << 13;
		::_Random_Seed ^= ::_Random_Seed >> 7;
		::_Random_Seed ^= ::_Random_Seed << 17;
		return ::_Random_Seed;
	}

	template<typename T>
	constexpr static inline T GenerateInt(T Minimum, T Maximum) {

		unlikely_branch
		if (Minimum == static_cast<T>(0) && Maximum == static_cast<T>(0)) {
			return static_cast<T>(0);
		}

		return Minimum + BitOp::Bitcast<T>(xorshift()) % Maximum - std::abs(Minimum);
	}

	template<typename T>
	constexpr static inline T GenerateUint(T Minimum, T Maximum) {

		unlikely_branch
		if (Minimum == static_cast<T>(0) && Maximum == static_cast<T>(0)) {
			return static_cast<T>(0);
		}

		return Minimum + static_cast<T>(xorshift() % Maximum - Minimum);
	}
}

namespace Random {

	inline Sint8 GenerateInt8(Sint8 Minimum = INT8_MIN, Sint8 Maximum = INT8_MAX) {
		return GenerateInt<Sint8>(Minimum, Maximum);
	}

	inline Sint16 GenerateInt16(Sint16 Minimum = INT16_MIN, Sint16 Maximum = INT16_MAX) {
		return GenerateInt<Sint16>(Minimum, Maximum);
	}

	inline Sint32 GenerateInt32(Sint32 Minimum = INT32_MIN, Sint32 Maximum = INT32_MAX) {
		return GenerateInt<Sint32>(Minimum, Maximum);
	}

	inline Sint64 GenerateInt64(Sint64 Minimum = INT64_MIN, Sint64 Maximum = INT64_MAX) {
		return GenerateInt<Sint64>(Minimum, Maximum);
	}



	inline Uint8 GenerateUint8(Uint8 Minimum = 0, Uint8 Maximum = UINT8_MAX) {
		return GenerateUint<Uint8>(Minimum, Maximum);
	}

	inline Uint16 GenerateUint16(Uint16 Minimum = 0, Uint16 Maximum = UINT16_MAX) {
		return GenerateUint<Uint16>(Minimum, Maximum);
	}

	inline Uint32 GenerateUint32(Uint32 Minimum = 0, Uint32 Maximum = UINT32_MAX) {
		return GenerateUint<Uint32>(Minimum, Maximum);
	}

	inline Uint64 GenerateUint64(Uint64 Minimum = 0, Uint64 Maximum = UINT64_MAX) {
		return GenerateUint<Uint64>(Minimum, Maximum);
	}


	inline float GenerateFloat() {
		return BitOp::Bitcast<float>(xorshift() % UINT32_MAX);
	}
	inline float GenerateFloat(float Minimum, float Maximum) {
		return Minimum + std::fmod(BitOp::Bitcast<float>(xorshift() % UINT32_MAX), Maximum - abs(Minimum));
	}

	inline double GenerateDouble() {
		return BitOp::Bitcast<double>(xorshift());
	}
	inline double GenerateDouble(double Minimum, double Maximum) {
		return Minimum + std::fmod(BitOp::Bitcast<double>(xorshift()), Maximum - abs(Minimum));
	}
	

	namespace Vector {

		inline Math::Vector2 GenerateVector2() {
			return Math::Vector2(Random::GenerateFloat(), Random::GenerateFloat());
		}

		inline Math::Vector2 GenerateVector2(const Math::Vector2& Minimum, const Math::Vector2& Maximum) {
			return Math::Vector2(Random::GenerateFloat(Minimum.X, Maximum.X), Random::GenerateFloat(Minimum.Y, Maximum.Y));
		}

		inline Math::Vector2 GenerateVector2(float Minimum, float Maximum) {
			return Math::Vector2(Random::GenerateFloat(Minimum, Maximum), Random::GenerateFloat(Minimum, Maximum));
		}


		inline Math::Vector3 GenerateVector3() {
			return Math::Vector3(
				Random::GenerateFloat(),
				Random::GenerateFloat(),
				Random::GenerateFloat()
			);
		}

		inline Math::Vector3 GenerateVector3(const Math::Vector3& Minimum, const Math::Vector3& Maximum) {
			return Math::Vector3(
				Random::GenerateFloat(Minimum.X, Maximum.X),
				Random::GenerateFloat(Minimum.Y, Maximum.Y),
				Random::GenerateFloat(Minimum.Z, Maximum.Z)
			);
		}

		inline Math::Vector3 GenerateVector3(float Minimum, float Maximum) {
			return Math::Vector3(
				Random::GenerateFloat(Minimum, Maximum),
				Random::GenerateFloat(Minimum, Maximum),
				Random::GenerateFloat(Minimum, Maximum)
			);
		}



		inline Math::IVector2 GenerateIVector2(const Math::IVector2& Minimum, const Math::IVector2& Maximum) {
			return Math::IVector2(Random::GenerateInt32(Minimum.X, Maximum.X), Random::GenerateInt32(Minimum.Y, Maximum.Y));
		}

		inline Math::IVector2 GenerateIVector2(int32_t Minimum = INT32_MIN, int32_t Maximum = INT32_MAX) {
			return Math::IVector2(Random::GenerateInt32(Minimum, Maximum), Random::GenerateInt32(Minimum, Maximum));
		}
	}
}