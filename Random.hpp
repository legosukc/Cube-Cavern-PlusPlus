#ifndef _RANDOM_H
#define _RANDOM_H

#include <random>
#include <stdint.h>

#include "Vector2.hpp"
#include "Vector3.hpp"

#include "FunctionHeaders/BitOp.hpp"




static thread_local uint64_t Seed = static_cast<uint64_t>(std::random_device()());

namespace {

	// Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs"
	static uint64_t xorshift() {
		Seed ^= Seed << 13;
		Seed ^= Seed >> 7;
		Seed ^= Seed << 17;
		return Seed;
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

	inline int8_t GenerateInt8(int8_t Minimum = INT8_MIN, int8_t Maximum = INT8_MAX) {
		return GenerateInt<int8_t>(Minimum, Maximum);
	}

	inline int16_t GenerateInt16(int16_t Minimum = INT16_MIN, int16_t Maximum = INT16_MAX) {
		return GenerateInt<int16_t>(Minimum, Maximum);
	}

	inline int32_t GenerateInt32(int32_t Minimum = INT32_MIN, int32_t Maximum = INT32_MAX) {
		return GenerateInt<int32_t>(Minimum, Maximum);
	}

	inline int64_t GenerateInt64(int64_t Minimum = INT64_MIN, int64_t Maximum = INT64_MAX) {
		return GenerateInt<int64_t>(Minimum, Maximum);
	}



	inline uint8_t GenerateUint8(uint8_t Minimum = 0, uint8_t Maximum = UINT8_MAX) {
		return GenerateUint<uint8_t>(Minimum, Maximum);
	}

	inline uint16_t GenerateUint16(uint16_t Minimum = 0, uint16_t Maximum = UINT16_MAX) {
		return GenerateUint<uint16_t>(Minimum, Maximum);
	}

	inline uint32_t GenerateUint32(uint32_t Minimum = 0, uint32_t Maximum = UINT32_MAX) {
		return GenerateUint<uint32_t>(Minimum, Maximum);
	}

	inline uint64_t GenerateUint64(uint64_t Minimum = 0, uint64_t Maximum = UINT64_MAX) {
		return GenerateUint<uint64_t>(Minimum, Maximum);
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


#endif