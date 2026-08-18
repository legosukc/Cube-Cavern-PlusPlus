#ifndef VECTOR2_HPP
#define VECTOR2_HPP 1

#include "../define.h"

#include <SDL3/SDL_stdinc.h>
#include <cmath>
#include <type_traits>

#include "../FunctionHeaders/Math.hpp"

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
    static constexpr inline int ComponentCount = 2;
    using ComponentType = _ComponentType;

    static_assert(std::is_integral_v<ComponentType> ||
                      std::is_floating_point_v<ComponentType>,
                  "Unexpected _base_vector2 template parameter. "
                  "Expected floating point or integral.");

    constexpr _base_vector2()
        : X(static_cast<_ComponentType>(0)),
          Y(static_cast<_ComponentType>(0)) {}
    constexpr _base_vector2(ComponentType _X, ComponentType _Y)
        : X(_X), Y(_Y) {}
    constexpr explicit _base_vector2(ComponentType Value)
        : X(Value), Y(Value) {}

    union {
        ComponentType X, R, S;
    };
    union {
        ComponentType Y, G, T;
    };

    template <typename IndexType>
    constexpr ComponentType& operator[](IndexType Index) {
        return reinterpret_cast<ComponentType*>(this)[Index];
    }

    template <typename IndexType>
    constexpr const ComponentType& operator[](IndexType Index) const {
        return reinterpret_cast<const ComponentType*>(this)[Index];
    }

    constexpr _DerivedType operator+(const _DerivedType& B) const {
        return _DerivedType(this->X + B.X, this->Y + B.Y);
    }
    constexpr _DerivedType operator+(ComponentType B) const {
        return _DerivedType(this->X + B, this->Y + B);
    }

    constexpr void operator+=(const _DerivedType& B) {
        *this = this->operator+(B);
    }
    constexpr void operator+=(ComponentType B) { *this = this->operator+(B); }

    constexpr _DerivedType operator-(const _DerivedType& B) const {
        return _DerivedType(this->X - B.X, this->Y - B.Y);
    }

    constexpr _DerivedType operator-(ComponentType B) const {
        return _DerivedType(this->X - B, this->Y - B);
    }

    constexpr _DerivedType operator-() const {
        return _DerivedType(-this->X, -this->Y);
    }

    constexpr void operator-=(const _DerivedType& B) {
        *this = this->operator-(B);
    }
    constexpr void operator-=(ComponentType B) { *this = this->operator-(B); }

    constexpr _DerivedType operator*(const _DerivedType& B) const {
        return _DerivedType(this->X * B.X, this->Y * B.Y);
    }

    constexpr _DerivedType operator*(ComponentType B) const {
        return _DerivedType(this->X * B, this->Y * B);
    }

    constexpr void operator*=(const _DerivedType& B) {
        *this = this->operator*(B);
    }
    constexpr void operator*=(ComponentType B) { *this = this->operator*(B); }

    constexpr _DerivedType operator/(const _DerivedType& B) const {
        return _DerivedType(this->X / B.X, this->Y / B.Y);
    }
    constexpr _DerivedType operator/(ComponentType B) const {
        return _DerivedType(this->X / B, this->Y / B);
    }

    constexpr void operator/=(const _DerivedType& B) {
        *this = this->operator/(B);
    }
    constexpr void operator/=(ComponentType B) { *this = this->operator/(B); }

    constexpr _DerivedType operator%(const _DerivedType& B) const {
        return _DerivedType(Math::Mod<ComponentType>(this->X, B.X),
                            Math::Mod<ComponentType>(this->Y, B.Y));
    }
    constexpr _DerivedType operator%(ComponentType B) const {
        return _DerivedType(Math::Mod<ComponentType>(this->X, B),
                            Math::Mod<ComponentType>(this->Y, B));
    }

    constexpr void operator%=(const _DerivedType& B) {
        *this = this->operator%(B);
    }
    constexpr void operator%=(ComponentType B) { *this = this->operator%(B); }

    constexpr bool operator==(const _DerivedType& B) const {
        return this->X == B.X && this->Y == B.Y;
    }
    constexpr bool operator!=(const _DerivedType& B) const {
        return this->X != B.X || this->Y != B.Y;
    }

    constexpr ComponentType Dot(const _DerivedType& B) const {
        return (this->X * B.X) + (this->Y * B.Y);
    }

    constexpr ComponentType Magnitude() const {
        if CONSTEXPR_IF (std::is_floating_point_v<ComponentType>) {
            return std::sqrt((this->X * this->X) + (this->Y * this->Y));
        } else {
            return static_cast<ComponentType>(
                std::sqrtl(static_cast<long double>((this->X * this->X) +
                                                    (this->Y * this->Y))));
        }
    }
};

struct Math::Vector2 : Math::etc::_base_vector2<float, Math::Vector2> {
    using _base_vector2::_base_vector2;

    constexpr operator Math::IVector2() const;
    constexpr operator Math::UVector2() const;

    constexpr Math::Vector2 Floor() const {
        return Math::Vector2(std::floor(this->X), std::floor(this->Y));
    }

    constexpr Math::Vector2 Round() const {
        return Math::Vector2(std::round(this->X), std::round(this->Y));
    }

    constexpr Math::Vector2 Ceil() const {
        return Math::Vector2(std::ceil(this->X), std::ceil(this->Y));
    }

    constexpr Math::Vector2 Normalize() const {
        return *this *

#ifdef USE_SIMD_INTRINSICS
               _mm_rsqrt_ss(_mm_set_ss(this->Dot(*this)))[0];
#else
               1.f / std::sqrt(this->Dot(*this));
#endif
    }
};

constexpr Math::Vector2 operator+(Math::Vector2::ComponentType A,
                                  const Math::Vector2& B) {
    return Math::Vector2(A) + B;
}

constexpr Math::Vector2 operator-(Math::Vector2::ComponentType A,
                                  const Math::Vector2& B) {
    return Math::Vector2(A) - B;
}

constexpr Math::Vector2 operator*(Math::Vector2::ComponentType A,
                                  const Math::Vector2& B) {
    return Math::Vector2(A) * B;
}

constexpr Math::Vector2 operator/(Math::Vector2::ComponentType A,
                                  const Math::Vector2& B) {
    return Math::Vector2(A) / B;
}

constexpr Math::Vector2 operator%(Math::Vector2::ComponentType A,
                                  const Math::Vector2& B) {
    return Math::Vector2(A) % B;
}

struct Math::IVector2 : Math::etc::_base_vector2<Sint32, Math::IVector2> {
    using _base_vector2::_base_vector2;

    constexpr operator Math::Vector2() const;
    constexpr operator Math::UVector2() const;
};

constexpr Math::IVector2 operator+(Math::IVector2::ComponentType A,
                                   const Math::IVector2& B) {
    return Math::IVector2(A) + B;
}

constexpr Math::IVector2 operator-(Math::IVector2::ComponentType A,
                                   const Math::IVector2& B) {
    return Math::IVector2(A) - B;
}

constexpr Math::IVector2 operator*(Math::IVector2::ComponentType A,
                                   const Math::IVector2& B) {
    return Math::IVector2(A) * B;
}

constexpr Math::IVector2 operator/(Math::IVector2::ComponentType A,
                                   const Math::IVector2& B) {
    return Math::IVector2(A) / B;
}

constexpr Math::IVector2 operator%(Math::IVector2::ComponentType A,
                                   const Math::IVector2& B) {
    return Math::IVector2(A) % B;
}

struct Math::UVector2 : Math::etc::_base_vector2<Uint32, Math::UVector2> {
    using _base_vector2::_base_vector2;

    constexpr operator Math::Vector2() const;
    constexpr operator Math::IVector2() const;
};

constexpr Math::UVector2 operator+(Math::UVector2::ComponentType A,
                                   const Math::UVector2& B) {
    return Math::UVector2(A) + B;
}

constexpr Math::UVector2 operator-(Math::UVector2::ComponentType A,
                                   const Math::UVector2& B) {
    return Math::UVector2(A) - B;
}

constexpr Math::UVector2 operator*(Math::UVector2::ComponentType A,
                                   const Math::UVector2& B) {
    return Math::UVector2(A) * B;
}

constexpr Math::UVector2 operator/(Math::UVector2::ComponentType A,
                                   const Math::UVector2& B) {
    return Math::UVector2(A) / B;
}

constexpr Math::UVector2 operator%(Math::UVector2::ComponentType A,
                                   const Math::UVector2& B) {
    return Math::UVector2(A) % B;
}

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
#endif