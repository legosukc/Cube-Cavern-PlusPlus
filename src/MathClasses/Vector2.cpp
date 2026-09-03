#include "Vector2.hpp"

#include "../define.h"

#include <SDL3/SDL_stdinc.h>
#include <cmath>

#include "../FunctionHeaders/Math.hpp"

template <typename _ComponentType, typename _DerivedType>
constexpr Math::etc::_base_vector2<_ComponentType,
                                   _DerivedType>::_base_vector2()
    : X(static_cast<_ComponentType>(0)), Y(static_cast<_ComponentType>(0)) {}

template <typename _ComponentType, typename _DerivedType>
constexpr Math::etc::_base_vector2<_ComponentType, _DerivedType>::_base_vector2(
    ComponentType _X,
    ComponentType _Y)
    : X(_X), Y(_Y) {}

template <typename _ComponentType, typename _DerivedType>
constexpr Math::etc::_base_vector2<_ComponentType, _DerivedType>::_base_vector2(
    ComponentType Value)
    : X(Value), Y(Value) {}

template <typename _ComponentType, typename _DerivedType>
constexpr _ComponentType&
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator[](
        int Index) {
    return reinterpret_cast<ComponentType*>(this)[Index];
}

template <typename _ComponentType, typename _DerivedType>
constexpr const _ComponentType&
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator[](
        int Index) const {
    return reinterpret_cast<const ComponentType*>(this)[Index];
}

template <typename _ComponentType, typename _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator+(
        const _DerivedType& B) const {
    return _DerivedType(this->X + B.X, this->Y + B.Y);
}

template <typename _ComponentType, typename _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator+(
        ComponentType B) const {
    return _DerivedType(this->X + B, this->Y + B);
}

template <typename _ComponentType, typename _DerivedType>
constexpr void
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator+=(
        const _DerivedType& B) {
    *this = *this + B;
}

template <typename _ComponentType, typename _DerivedType>
constexpr void
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator+=(
        ComponentType B) {
    *this = *this + B;
}

template <typename _ComponentType, typename _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator-(
        const _DerivedType& B) const {
    return _DerivedType(this->X - B.X, this->Y - B.Y);
}

template <typename _ComponentType, typename _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator-(
        ComponentType B) const {
    return _DerivedType(this->X - B, this->Y - B);
}

template <typename _ComponentType, typename _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator-() const {
    return _DerivedType(-this->X, -this->Y);
}

template <typename _ComponentType, typename _DerivedType>
constexpr void
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator-=(
        const _DerivedType& B) {
    *this = *this - B;
}

template <typename _ComponentType, typename _DerivedType>
constexpr void
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator-=(
        ComponentType B) {
    *this = *this - B;
}

template <typename _ComponentType, typename _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator*(
        const _DerivedType& B) const {
    return _DerivedType(this->X * B.X, this->Y * B.Y);
}

template <typename _ComponentType, typename _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator*(
        ComponentType B) const {
    return _DerivedType(this->X * B, this->Y * B);
}

template <typename _ComponentType, typename _DerivedType>
constexpr void
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator*=(
        const _DerivedType& B) {
    *this = *this * B;
}

template <typename _ComponentType, typename _DerivedType>
constexpr void
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator*=(
        ComponentType B) {
    *this = *this * B;
}

template <typename _ComponentType, typename _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator/(
        const _DerivedType& B) const {
    return _DerivedType(this->X / B.X, this->Y / B.Y);
}

template <typename _ComponentType, typename _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator/(
        ComponentType B) const {
    return _DerivedType(this->X / B, this->Y / B);
}

template <typename _ComponentType, typename _DerivedType>
constexpr void
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator/=(
        const _DerivedType& B) {
    *this = *this / B;
}

template <typename _ComponentType, typename _DerivedType>
constexpr void
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator/=(
        ComponentType B) {
    *this = *this / B;
}

template <typename _ComponentType, typename _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator%(
        const _DerivedType& B) const {
    return _DerivedType(Math::Mod<ComponentType>(this->X, B.X),
                        Math::Mod<ComponentType>(this->Y, B.Y));
}

template <typename _ComponentType, typename _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator%(
        ComponentType B) const {
    return _DerivedType(Math::Mod<ComponentType>(this->X, B),
                        Math::Mod<ComponentType>(this->Y, B));
}

template <typename _ComponentType, typename _DerivedType>
constexpr void
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator%=(
        const _DerivedType& B) {
    *this = this->operator%(B);
}

template <typename _ComponentType, typename _DerivedType>
constexpr void
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator%=(
        ComponentType B) {
    *this = *this % B;
}

template <typename _ComponentType, typename _DerivedType>
constexpr bool
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator==(
        const _DerivedType& B) const {
    return this->X == B.X && this->Y == B.Y;
}

template <typename _ComponentType, typename _DerivedType>
constexpr bool
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::operator!=(
        const _DerivedType& B) const {
    return this->X != B.X || this->Y != B.Y;
}

template <typename _ComponentType, typename _DerivedType>
constexpr _ComponentType
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::Dot(
        const _DerivedType& B) const {
    return (this->X * B.X) + (this->Y * B.Y);
}

template <typename _ComponentType, typename _DerivedType>
constexpr _ComponentType
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::Magnitude() const {
    if CONSTEXPR_IF (std::is_floating_point_v<ComponentType>) {
        return std::sqrt((this->X * this->X) + (this->Y * this->Y));
    } else {
        return static_cast<ComponentType>(std::sqrtl(static_cast<long double>(
            (this->X * this->X) + (this->Y * this->Y))));
    }
}

template <typename _ComponentType, typename _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::Lerp(
        const _DerivedType& B,
        float Alpha) const {
    return *(_DerivedType*)this + (B - *(_DerivedType*)this) * Alpha;
}

template <typename _ComponentType, typename _DerivedType>
constexpr _DerivedType
    Math::etc::_base_vector2<_ComponentType, _DerivedType>::Lerp(
        const _DerivedType& B,
        const _DerivedType& Alpha) const {
    return *(_DerivedType*)this + (B - *(_DerivedType*)this) * Alpha;
}

constexpr Math::Vector2 Math::Vector2::Floor() const {
    return Math::Vector2(std::floor(this->X), std::floor(this->Y));
}

constexpr Math::Vector2 Math::Vector2::Round() const {
    return Math::Vector2(std::round(this->X), std::round(this->Y));
}

constexpr Math::Vector2 Math::Vector2::Ceil() const {
    return Math::Vector2(std::ceil(this->X), std::ceil(this->Y));
}

/*
constexpr Math::Vector2 Math::Vector2::Normalize() const {
    return *this * (1.f / std::sqrt(this->Dot(*this)));
}*/

//
// VECTOR CASTS
//

constexpr Math::Vector2::operator Math::IVector2() const {
    return Math::IVector2(static_cast<Math::IVector2::ComponentType>(this->X),
                          static_cast<Math::IVector2::ComponentType>(this->Y));
}

constexpr Math::Vector2::operator Math::UVector2() const {
    return Math::UVector2(static_cast<Math::UVector2::ComponentType>(this->X),
                          static_cast<Math::UVector2::ComponentType>(this->Y));
}

constexpr Math::IVector2::operator Math::Vector2() const {
    return Math::Vector2(static_cast<Math::Vector2::ComponentType>(this->X),
                         static_cast<Math::Vector2::ComponentType>(this->Y));
}

constexpr Math::IVector2::operator Math::UVector2() const {
    return Math::UVector2(static_cast<Math::UVector2::ComponentType>(this->X),
                          static_cast<Math::UVector2::ComponentType>(this->Y));
}

constexpr Math::UVector2::operator Math::Vector2() const {
    return Math::Vector2(static_cast<Math::Vector2::ComponentType>(this->X),
                         static_cast<Math::Vector2::ComponentType>(this->Y));
}

constexpr Math::UVector2::operator Math::IVector2() const {
    return Math::IVector2(static_cast<Math::IVector2::ComponentType>(this->X),
                          static_cast<Math::IVector2::ComponentType>(this->Y));
}

template class Math::etc::_base_vector2<float, Math::Vector2>;
template class Math::etc::_base_vector2<Sint32, Math::IVector2>;
template class Math::etc::_base_vector2<Uint32, Math::UVector2>;