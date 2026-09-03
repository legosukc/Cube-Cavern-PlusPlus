#ifndef VECTOR2_HPP
#define VECTOR2_HPP 1

#include <SDL3/SDL_stdinc.h>
#include <type_traits>

#include <cmath>

namespace Math {
    namespace etc {
        template <typename _ComponentType, typename _DerivedType>
        struct _base_vector2;
    }
    struct Vector2;
    struct IVector2;
    struct UVector2;
}

template <typename _ComponentType, typename _DerivedType>
struct Math::etc::_base_vector2 {
    static constexpr int ComponentCount = 2;
    using ComponentType = _ComponentType;

    static_assert(std::is_integral_v<ComponentType> ||
                      std::is_floating_point_v<ComponentType>,
                  "Unexpected _base_vector2 template parameter. "
                  "Expected floating point or integral.");

    constexpr _base_vector2();
    constexpr _base_vector2(ComponentType _X, ComponentType _Y);
    constexpr explicit _base_vector2(ComponentType Value);

    union {
        ComponentType X, R, S;
    };
    union {
        ComponentType Y, G, T;
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

    constexpr _DerivedType Lerp(const _DerivedType& B, float Alpha) const;
    constexpr _DerivedType Lerp(const _DerivedType& B,
                                const _DerivedType& Alpha) const;
};

struct Math::Vector2 : Math::etc::_base_vector2<float, Math::Vector2> {
    using _base_vector2::_base_vector2;

    constexpr operator Math::IVector2() const;
    constexpr operator Math::UVector2() const;

    constexpr Math::Vector2 Floor() const;
    constexpr Math::Vector2 Round() const;
    constexpr Math::Vector2 Ceil() const;

    constexpr Math::Vector2 Normalize() const {
        return *this * (1.f / std::sqrt(this->Dot(*this)));
    }
};

struct Math::IVector2 : Math::etc::_base_vector2<Sint32, Math::IVector2> {
    using _base_vector2::_base_vector2;

    constexpr operator Math::Vector2() const;
    constexpr operator Math::UVector2() const;
};

struct Math::UVector2 : Math::etc::_base_vector2<Uint32, Math::UVector2> {
    using _base_vector2::_base_vector2;

    constexpr operator Math::Vector2() const;
    constexpr operator Math::IVector2() const;
};

#endif