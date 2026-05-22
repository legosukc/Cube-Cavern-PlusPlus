#pragma once

#include <SDL3/SDL_stdinc.h>


namespace {

	template<typename T>
	struct _ToBitBuffer_struct {
		//static_assert(false, "bro you FUCKED up LOLLLLLLL");
	};

	template<>
	struct _ToBitBuffer_struct<Sint8> {
		using _BitBuffer = Uint8;
	};

	template<>
	struct _ToBitBuffer_struct<Sint16> {
		using _BitBuffer = Uint16;
	};

	template<>
	struct _ToBitBuffer_struct<Sint32> {
		using _BitBuffer = Uint32;
	};

	template<>
	struct _ToBitBuffer_struct<Sint64> {
		using _BitBuffer = Uint64;
	};

	
	template<>
	struct _ToBitBuffer_struct<float> {
		using _BitBuffer = Uint32;
	};

	template<>
	struct _ToBitBuffer_struct<double> {
		using _BitBuffer = Uint64;
	};
}

namespace BitOp {

	template<typename To_T, typename From_T>
	constexpr inline To_T Bitcast(From_T CastValue) noexcept {
		return *reinterpret_cast<To_T*>(&CastValue);
	}

	template<typename T>
	using ToBitBuffer = typename _ToBitBuffer_struct<T>::_BitBuffer;



	template<typename T>
	constexpr inline bool CheckBit(T Value, T BitIndex) {
		return static_cast<bool>((Bitcast<ToBitBuffer<T>>(Value) >> BitIndex) & 1);
	}

	template<typename T>
	constexpr inline T SetBit(T Value, T BitIndex, bool Flipped) {

		if (Flipped) {
			return Value | (1 << BitIndex);
		} else {
			return Value & !!(1 << BitIndex);
		}
	}
}