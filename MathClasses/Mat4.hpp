#pragma once

#include "Vector4.hpp"


namespace Math {
	struct Mat4;
}


struct Math::Mat4 {

	using ColumnType = Math::Vector4;
	static constexpr inline int RowCount = 4;
	static constexpr inline int ColumnCount = 4;

	constexpr Mat4() = default;
	constexpr Mat4(ColumnType::ComponentType Value) : R0(Value), R1(Value), R2(Value), R3(Value) {}
	constexpr Mat4(
		const ColumnType& _R0,
		const ColumnType& _R1,
		const ColumnType& _R2,
		const ColumnType& _R3
	) : R0(_R0), R1(_R1), R2(_R2), R3(_R3) {}


	ColumnType R0, R1, R2, R3;

	inline ColumnType& operator[](int Index) {
		return reinterpret_cast<ColumnType*>(this)[Index];
	}
	inline const ColumnType& operator[](int Index) const {
		return reinterpret_cast<const ColumnType*>(this)[Index];
	}


	constexpr Math::Mat4 operator*(const Math::Mat4& B) const {

		Math::Mat4 Result;
		for (int i = 0; i < Math::Mat4::ColumnCount; ++i) {

			for (int j = 0; j < Math::Mat4::RowCount; ++j) {

				for (int k = 0; k < Math::Mat4::ColumnCount; ++k) {
					Result[i][j] += this->operator[](i)[k] * B[k][j];
				}
			}
		}

		return Result;
	}

	constexpr void operator*=(const Math::Mat4& B) {
		*this = this->operator*(B);
	}


	constexpr bool operator==(const Math::Mat4& B) const {

		for (int i = 0; i < Math::Mat4::ColumnCount; ++i) {

			if (this->operator[](i) != B[i]) {
				return false;
			}
		}

		return true;
	}
};