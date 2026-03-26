
namespace {

	template<typename T>
	struct _ToBitBuffer_struct {
		//static_assert(false, "bro you FUCKED up LOLLLLLLL");
	};

	template<>
	struct _ToBitBuffer_struct<int8_t> {
		using _BitBuffer = uint8_t;
	};

	template<>
	struct _ToBitBuffer_struct<int16_t> {
		using _BitBuffer = uint16_t;
	};

	template<>
	struct _ToBitBuffer_struct<int32_t> {
		using _BitBuffer = uint32_t;
	};

	template<>
	struct _ToBitBuffer_struct<int64_t> {
		using _BitBuffer = uint64_t;
	};

	
	template<>
	struct _ToBitBuffer_struct<float> {
		using _BitBuffer = uint32_t;
	};

	template<>
	struct _ToBitBuffer_struct<double> {
		using _BitBuffer = uint64_t;
	};
}

namespace BitOp {

	template<typename To_T, typename From_T>
	constexpr inline To_T Bitcast(From_T CastValue) {
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