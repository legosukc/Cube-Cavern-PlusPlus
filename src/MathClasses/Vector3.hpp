#pragma once

#include "../define.h"

#include <type_traits>
#include <cmath>

#include "../FunctionHeaders/Math.hpp"

#include <SDL3/SDL_stdinc.h>

namespace Math {
    namespace etc {
        template <typename _ComponentType, class _DerivedType>
        struct _base_vector3;
    }
    struct Vector3;
    struct IVector3;
    struct UVector3;
}

template <typename _ComponentType, class _DerivedType>
struct Math::etc::_base_vector3 {
    static constexpr int ComponentCount = 3;
    using ComponentType = _ComponentType;

    static_assert(std::is_integral_v<ComponentType> ||
                      std::is_floating_point_v<ComponentType>,
                  "Unexpected _base_vector3 template parameter. "
                  "Expected floating point or integral.");

    constexpr _base_vector3();
    constexpr _base_vector3(ComponentType _X,
                            ComponentType _Y,
                            ComponentType _Z);
    constexpr explicit _base_vector3(ComponentType Value);

    union {
        ComponentType X, R, S;
    };
    union {
        ComponentType Y, G, T;
    };
    union {
        ComponentType Z, B, P;
    };

    constexpr ComponentType& operator[](int Index);
    constexpr const ComponentType& operator[](int Index) const;

    constexpr _DerivedType operator+(const _DerivedType& B) const;
    constexpr _DerivedType operator+(ComponentType B) const;

    constexpr void operator+=(const _DerivedType& B);
    constexpr void operator+=(ComponentType B);

    constexpr _DerivedType operator-(const _DerivedType& B) const;
    constexpr _DerivedType operator-(ComponentType B) const;
    constexpr _DerivedType operator-() const;

    constexpr void operator-=(const _DerivedType& B);
    constexpr void operator-=(ComponentType B);

    constexpr _DerivedType operator*(const _DerivedType& B) const;
    constexpr _DerivedType operator*(ComponentType B) const;

    constexpr void operator*=(const _DerivedType& B);
    constexpr void operator*=(ComponentType B);

    constexpr _DerivedType operator/(const _DerivedType& B) const;
    constexpr _DerivedType operator/(ComponentType B) const;

    constexpr void operator/=(const _DerivedType& B);
    constexpr void operator/=(ComponentType B);

    constexpr _DerivedType operator%(const _DerivedType& B) const;
    constexpr _DerivedType operator%(ComponentType B) const;

    constexpr void operator%=(const _DerivedType& B);
    constexpr void operator%=(ComponentType B);

    constexpr bool operator==(const _DerivedType& B) const;
    constexpr bool operator!=(const _DerivedType& B) const;

    constexpr ComponentType Dot(const _DerivedType& B) const;
    constexpr ComponentType Magnitude() const;
    constexpr _DerivedType Cross(const _DerivedType& B) const;
};

template <typename _ComponentType, class _DerivedType>
constexpr Math::etc::_base_vector3<_ComponentType,
                                   _DerivedType>::_base_vector3()
    : _base_vector3(static_cast<ComponentType>(0)) {}

template <typename _ComponentType, class _DerivedType>
constexpr Math::etc::_base_vector3<_ComponentType, _DerivedType>::_base_vector3(
    ComponentType _X,
    ComponentType _Y,
    ComponentType _Z)
    : X(_X), Y(_Y), Z(_Z) {}

template <typename _ComponentType, class _DerivedType>
constexpr Math::etc::_base_vector3<_ComponentType, _DerivedType>::_base_vector3(
    ComponentType Value)
    : X(Value), Y(Value), Z(Value) {}

template <typename _ComponentType, class _DerivedType>
constexpr _ComponentType&
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator[](
        int Index) {
    return reinterpret_cast<_ComponentType*>(this)[Index];
}

template <typename _ComponentType, class _DerivedType>
constexpr const _ComponentType&
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator[](
        int Index) const {
    return reinterpret_cast<const ComponentType*>(this)[Index];
}

template <typename _ComponentType, class _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator+(
        const _DerivedType& B) const {
    return _DerivedType(this->X + B.X, this->Y + B.Y, this->Z + B.Z);
}

template <typename _ComponentType, class _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator+(
        ComponentType B) const {
    return _DerivedType(this->X + B, this->Y + B, this->Z + B);
}

template <typename _ComponentType, class _DerivedType>
constexpr void
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator+=(
        const _DerivedType& B) {
    *this = *this + B;
}

template <typename _ComponentType, class _DerivedType>
constexpr void
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator+=(
        ComponentType B) {
    *this = *this + B;
}

template <typename _ComponentType, class _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator-(
        const _DerivedType& B) const {
    return _DerivedType(this->X - B.X, this->Y - B.Y, this->Z - B.Z);
}

template <typename _ComponentType, class _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator-(
        ComponentType B) const {
    return _DerivedType(this->X - B, this->Y - B, this->Z - B);
}

template <typename _ComponentType, class _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator-() const {
    return _DerivedType(-this->X, -this->Y, -this->Z);
}

template <typename _ComponentType, class _DerivedType>
constexpr void
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator-=(
        const _DerivedType& B) {
    *this = *this - B;
}

template <typename _ComponentType, class _DerivedType>
constexpr void
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator-=(
        _ComponentType B) {
    *this = *this - B;
}

template <typename _ComponentType, class _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator*(
        const _DerivedType& B) const {
    return _DerivedType(this->X * B.X, this->Y * B.Y, this->Z * B.Z);
}

template <typename _ComponentType, class _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator*(
        ComponentType B) const {
    return _DerivedType(this->X * B, this->Y * B, this->Z * B);
}

template <typename _ComponentType, class _DerivedType>
constexpr void
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator*=(
        const _DerivedType& B) {
    *this = *this * B;
}

template <typename _ComponentType, class _DerivedType>
constexpr void
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator*=(
        ComponentType B) {
    *this = *this * B;
}

template <typename _ComponentType, class _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator/(
        const _DerivedType& B) const {
    return _DerivedType(this->X / B.X, this->Y / B.Y, this->Z / B.Z);
}

template <typename _ComponentType, class _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator/(
        ComponentType B) const {
    if CONSTEXPR_IF (std::is_integral_v<ComponentType>) {
        return _DerivedType(this->X / B, this->Y / B, this->Z / B);
    } else {
        return this->operator*(static_cast<ComponentType>(1) / B);
    }
}

template <typename _ComponentType, class _DerivedType>
constexpr void
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator/=(
        const _DerivedType& B) {
    *this = *this / B;
}

template <typename _ComponentType, class _DerivedType>
constexpr void
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator/=(
        ComponentType B) {
    *this = *this / B;
}

template <typename _ComponentType, class _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator%(
        const _DerivedType& B) const {
    return _DerivedType(Math::Mod<ComponentType>(this->X, B.X),
                        Math::Mod<ComponentType>(this->Y, B.Y),
                        Math::Mod<ComponentType>(this->Z, B.Z));
}

template <typename _ComponentType, class _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator%(
        ComponentType B) const {
    return _DerivedType(Math::Mod<ComponentType>(this->X, B),
                        Math::Mod<ComponentType>(this->Y, B),
                        Math::Mod<ComponentType>(this->Z, B));
}

template <typename _ComponentType, class _DerivedType>
constexpr void
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator%=(
        const _DerivedType& B) {
    *this = *this % B;
}

template <typename _ComponentType, class _DerivedType>
constexpr void
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator%=(
        ComponentType B) {
    *this = *this % B;
}

template <typename _ComponentType, class _DerivedType>
constexpr bool
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator==(
        const _DerivedType& B) const {
    return this->X == B.X && this->Y == B.Y && this->Z == B.Z;
}

template <typename _ComponentType, class _DerivedType>
constexpr bool
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::operator!=(
        const _DerivedType& B) const {
    return this->X != B.X || this->Y != B.Y || this->Z != B.Z;
}

template <typename _ComponentType, class _DerivedType>
constexpr _ComponentType
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::Dot(
        const _DerivedType& B) const {
    return (this->X * B.X) + (this->Y * B.Y) + (this->Z * B.Z);
}

template <typename _ComponentType, class _DerivedType>
constexpr _ComponentType
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::Magnitude() const {
    if CONSTEXPR_IF (std::is_integral_v<ComponentType>) {
        return static_cast<ComponentType>(std::sqrtl(static_cast<long double>(
            (this->X * this->X) + (this->Y * this->Y) + (this->Z * this->Z))));
    } else {
        return std::sqrt((this->X * this->X) + (this->Y * this->Y) +
                         (this->Z * this->Z));
    }
}

template <typename _ComponentType, class _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector3<_ComponentType, _DerivedType>::Cross(
        const _DerivedType& B) const {
    return _DerivedType(this->Y * B.Z - B.Y * this->Z,
                        this->Z * B.X - B.Z * this->X,
                        this->X * B.Y - B.X * this->Y);
}

struct Math::Vector3 : Math::etc::_base_vector3<float, Math::Vector3> {
    using _base_vector3::_base_vector3;

    constexpr Math::Vector3 Lerp(const Math::Vector3& B, float Alpha) const;
    constexpr Math::Vector3 Lerp(const Math::Vector3& B,
                                 const Math::Vector3& Alpha) const;

    constexpr Math::Vector3 Floor() const;
    constexpr Math::Vector3 Round() const;
    constexpr Math::Vector3 Ceil() const;

    SSE3_FUNCTION
    constexpr Math::Vector3 Normalize() const;

    constexpr operator Math::IVector3() const;
    constexpr operator Math::UVector3() const;
};

constexpr Math::Vector3 Math::Vector3::Lerp(const Math::Vector3& B,
                                            float Alpha) const {
    return *this + (B - *this) * Alpha;
}

constexpr Math::Vector3 Math::Vector3::Lerp(const Math::Vector3& B,
                                            const Math::Vector3& Alpha) const {
    return *this + (B - *this) * Alpha;
}

constexpr Math::Vector3 Math::Vector3::Floor() const {
    return Math::Vector3(std::floor(this->X), std::floor(this->Y),
                         std::floor(this->Z));
}

constexpr Math::Vector3 Math::Vector3::Round() const {
    return Math::Vector3(std::round(this->X), std::round(this->Y),
                         std::round(this->Z));
}

constexpr Math::Vector3 Math::Vector3::Ceil() const {
    return Math::Vector3(std::ceil(this->X), std::ceil(this->Y),
                         std::ceil(this->Z));
}

SSE3_FUNCTION
constexpr Math::Vector3 Math::Vector3::Normalize() const {
#ifdef USE_SIMD_INTRINSICS
    return this->operator*(_mm_rsqrt_ss(_mm_set_ss(this->Dot(*this)))[0]);
#else
    return this->operator/(std::sqrt(this->Dot(*this)));
#endif
}

constexpr Math::Vector3 operator+(Math::Vector3::ComponentType A,
                                  const Math::Vector3& B) {
    return Math::Vector3(A) + B;
}

constexpr Math::Vector3 operator-(Math::Vector3::ComponentType A,
                                  const Math::Vector3& B) {
    return Math::Vector3(A) - B;
}

constexpr Math::Vector3 operator*(Math::Vector3::ComponentType A,
                                  const Math::Vector3& B) {
    return Math::Vector3(A) * B;
}

constexpr Math::Vector3 operator/(Math::Vector3::ComponentType A,
                                  const Math::Vector3& B) {
    return Math::Vector3(static_cast<Math::Vector3::ComponentType>(1) / A) * B;
}

constexpr Math::Vector3 operator%(Math::Vector3::ComponentType A,
                                  const Math::Vector3& B) {
    return Math::Vector3(A) % B;
}

struct Math::IVector3 : Math::etc::_base_vector3<Sint32, IVector3> {
    using _base_vector3::_base_vector3;

    constexpr Math::IVector3 Abs() const;

    constexpr operator Math::Vector3() const;
    constexpr operator Math::UVector3() const;
};

constexpr Math::IVector3 Math::IVector3::Abs() const {
    return Math::IVector3(std::abs(this->X), std::abs(this->Y),
                          std::abs(this->Z));
}

constexpr Math::IVector3 operator+(Math::IVector3::ComponentType A,
                                   const Math::IVector3& B) {
    return Math::IVector3(A + B.X, A + B.Y, A + B.Z);
}

constexpr Math::IVector3 operator-(Math::IVector3::ComponentType A,
                                   const Math::IVector3& B) {
    return Math::IVector3(A - B.X, A - B.Y, A - B.Z);
}

constexpr Math::IVector3 operator*(Math::IVector3::ComponentType A,
                                   const Math::IVector3& B) {
    return Math::IVector3(A * B.X, A * B.Y, A * B.Z);
}

constexpr Math::IVector3 operator/(Math::IVector3::ComponentType A,
                                   const Math::IVector3& B) {
    return Math::IVector3(A / B.X, A / B.Y, A / B.Z);
}

constexpr Math::IVector3 operator%(Math::IVector3::ComponentType A,
                                   const Math::IVector3& B) {
    return Math::IVector3(A % B.X, A % B.Y, A % B.Z);
}

struct Math::UVector3 : Math::etc::_base_vector3<Uint32, UVector3> {
    using _base_vector3::_base_vector3;

    constexpr operator Math::Vector3() const;
    constexpr operator Math::IVector3() const;
};

constexpr Math::UVector3 operator+(Math::UVector3::ComponentType A,
                                   const Math::UVector3& B) {
    return Math::UVector3(A + B.X, A + B.Y, A + B.Z);
}

constexpr Math::UVector3 operator-(Math::UVector3::ComponentType A,
                                   const Math::UVector3& B) {
    return Math::UVector3(A - B.X, A - B.Y, A - B.Z);
}

constexpr Math::UVector3 operator*(Math::UVector3::ComponentType A,
                                   const Math::UVector3& B) {
    return Math::UVector3(A * B.X, A * B.Y, A * B.Z);
}

constexpr Math::UVector3 operator/(Math::UVector3::ComponentType A,
                                   const Math::UVector3& B) {
    return Math::UVector3(A / B.X, A / B.Y, A / B.Z);
}

constexpr Math::UVector3 operator%(Math::UVector3::ComponentType A,
                                   const Math::UVector3& B) {
    return Math::IVector3(A % B.X, A % B.Y, A % B.Z);
}

constexpr Math::Vector3::operator Math::IVector3() const {
    return Math::IVector3(static_cast<Math::IVector3::ComponentType>(this->X),
                          static_cast<Math::IVector3::ComponentType>(this->Y),
                          static_cast<Math::IVector3::ComponentType>(this->Z));
}

constexpr Math::Vector3::operator Math::UVector3() const {
    return Math::UVector3(static_cast<Math::UVector3::ComponentType>(this->X),
                          static_cast<Math::UVector3::ComponentType>(this->Y),
                          static_cast<Math::UVector3::ComponentType>(this->Z));
}

constexpr Math::IVector3::operator Math::Vector3() const {
    return Math::Vector3(static_cast<Math::Vector3::ComponentType>(this->X),
                         static_cast<Math::Vector3::ComponentType>(this->Y),
                         static_cast<Math::Vector3::ComponentType>(this->Z));
}

constexpr Math::IVector3::operator Math::UVector3() const {
    return Math::UVector3(static_cast<Math::UVector3::ComponentType>(this->X),
                          static_cast<Math::UVector3::ComponentType>(this->Y),
                          static_cast<Math::UVector3::ComponentType>(this->Z));
}

constexpr Math::UVector3::operator Math::Vector3() const {
    return Math::Vector3(static_cast<Math::Vector3::ComponentType>(this->X),
                         static_cast<Math::Vector3::ComponentType>(this->Y),
                         static_cast<Math::Vector3::ComponentType>(this->Z));
}

constexpr Math::UVector3::operator Math::IVector3() const {
    return Math::IVector3(static_cast<Math::IVector3::ComponentType>(this->X),
                          static_cast<Math::IVector3::ComponentType>(this->Y),
                          static_cast<Math::IVector3::ComponentType>(this->Z));
}