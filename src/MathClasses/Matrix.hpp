#pragma once

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

namespace {

    template <int _ColumnCount, typename ColumnType>
    struct _MatrixBase_ColumnMembers;

    template <typename ColumnType>
    struct _MatrixBase_ColumnMembers<2, ColumnType> {
        constexpr _MatrixBase_ColumnMembers() = default;
        constexpr _MatrixBase_ColumnMembers(const ColumnType& _R0,
                                            const ColumnType& _R1)
            : R0(_R0), R1(_R1) {}

        constexpr _MatrixBase_ColumnMembers(
            typename ColumnType::ComponentType Value)
            : R0(Value), R1(Value) {}

        ColumnType R0, R1;
    };

    template <typename ColumnType>
    struct _MatrixBase_ColumnMembers<3, ColumnType> {
        constexpr _MatrixBase_ColumnMembers() = default;
        constexpr _MatrixBase_ColumnMembers(const ColumnType& _R0,
                                            const ColumnType& _R1,
                                            const ColumnType& _R2)
            : R0(_R0), R1(_R1), R2(_R2) {}

        constexpr _MatrixBase_ColumnMembers(
            typename ColumnType::ComponentType Value)
            : R0(Value), R1(Value), R2(Value) {}

        ColumnType R0, R1, R2;
    };

    template <typename ColumnType>
    struct _MatrixBase_ColumnMembers<4, ColumnType> {
        constexpr _MatrixBase_ColumnMembers() = default;
        constexpr _MatrixBase_ColumnMembers(const ColumnType& _R0,
                                            const ColumnType& _R1,
                                            const ColumnType& _R2,
                                            const ColumnType& _R3)
            : R0(_R0), R1(_R1), R2(_R2), R3(_R3) {}

        constexpr _MatrixBase_ColumnMembers(
            typename ColumnType::ComponentType Value)
            : R0(Value), R1(Value), R2(Value), R3(Value) {}

        ColumnType R0, R1, R2, R3;
    };

    template <class _ColumnType, int _RowCount, int _ColumnCount>
    struct _MatrixBase : _MatrixBase_ColumnMembers<_ColumnCount, _ColumnType> {
        using ColumnType = _ColumnType;
        using ComponentType = typename _ColumnType::ComponentType;

        static constexpr inline int RowCount = _RowCount;
        static constexpr inline int ColumnCount = _ColumnCount;

        using _MatrixBase_ColumnMembers<_ColumnCount,
                                        _ColumnType>::_MatrixBase_ColumnMembers;

        constexpr ColumnType& operator[](int Index) {
            return reinterpret_cast<ColumnType*>(this)[Index];
        }
        constexpr const ColumnType& operator[](int Index) const {
            return reinterpret_cast<const ColumnType*>(this)[Index];
        }

        constexpr typename ColumnType::ComponentType& IndexElement(int Index) {
            return reinterpret_cast<ComponentType*>(this)[Index];
        }
        constexpr typename ColumnType::ComponentType IndexElement(
            int Index) const {
            return reinterpret_cast<const ComponentType*>(this)[Index];
        }

        constexpr _MatrixBase operator*(const _MatrixBase& B) const {
            _MatrixBase Result;
            for (int i = 0; i < _ColumnCount; ++i) {
                for (int j = 0; j < _RowCount; ++j) {
                    for (int k = 0; k < _ColumnCount; ++k) {
                        Result[i][j] += this->operator[](i)[k] * B[k][j];
                    }
                }
            }

            return Result;
        }

        constexpr void operator*=(const _MatrixBase& B) {
            *this = this->operator*(B);
        }

        constexpr bool operator==(const _MatrixBase& B) const {
            for (int i = 0; i < _ColumnCount; ++i) {
                if (this->operator[](i) != B[i]) {
                    return false;
                }
            }

            return true;
        }

        constexpr _MatrixBase Lerp(const _MatrixBase& B, float Alpha) const {
            _MatrixBase Result;
            for (int i = 0; i < _ColumnCount; ++i) {
                for (int j = 0; j < _RowCount; ++j) {
                    Result[i][j] = this->operator[](i)[j] +
                                   (B[i][j] - this->operator[](i)[j]) * Alpha;
                }
            }

            return Result;
        }
    };
}

namespace Math {
    using Mat2 = _MatrixBase<Math::Vector2, 2, 2>;
    using Mat3 = _MatrixBase<Math::Vector3, 3, 3>;
    using Mat4 = _MatrixBase<Math::Vector4, 4, 4>;
}