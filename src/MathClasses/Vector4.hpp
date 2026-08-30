#ifndef VECTOR4_HPP
#define VECTOR4_HPP 1

#include "../define.h"

#include <cmath>

#include <SDL3/SDL_stdinc.h>

namespace Math {
    namespace etc {
        template <typename _ComponentType, class _DerivedType>
        struct _base_vector4;
    }
    struct Vector4;
    struct IVector4;
    struct UVector4;
}

template <typename _ComponentType, class _DerivedType>
struct Math::etc::_base_vector4 {

    static constexpr int ComponentCount = 4;
    using ComponentType = _ComponentType;

    constexpr _base_vector4<_ComponentType, _DerivedType>()
        : _base_vector4<_ComponentType, _DerivedType>(
              static_cast<ComponentType>(0)) {}
    constexpr _base_vector4<_ComponentType, _DerivedType>(_ComponentType _X,
                                                          ComponentType _Y,
                                                          ComponentType _Z,
                                                          ComponentType _W)
        : X(_X), Y(_Y), Z(_Z), W(_W) {}
    constexpr explicit _base_vector4<_ComponentType, _DerivedType>(
        ComponentType Value)
        : _base_vector4<_ComponentType, _DerivedType>(Value,
                                                      Value,
                                                      Value,
                                                      Value) {}

    union {
        ComponentType X, R, S;
    };
    union {
        ComponentType Y, G, T;
    };
    union {
        ComponentType Z, B, P;
    };
    union {
        ComponentType W, A, Q;
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
        return _DerivedType(this->X + B.X, this->Y + B.Y, this->Z + B.Z,
                            this->W + B.W);
    }

    constexpr _DerivedType operator+(ComponentType B) const {
        return _DerivedType(this->X + B, this->Y + B, this->Z + B, this->W + B);
    }

    constexpr void operator+=(const _DerivedType& B) { *this = *this + B; }

    constexpr void operator+=(ComponentType B) { *this = *this + B; }

    constexpr _DerivedType operator-(const _DerivedType& B) const {
        return _DerivedType(this->X - B.X, this->Y - B.Y, this->Z - B.Z,
                            this->W - B.W);
    }
    

    constexpr _DerivedType operator-(ComponentType B) const {
        return _DerivedType(this->X - B, this->Y - B, this->Z - B, this->W - B);
    }

    constexpr _DerivedType operator-() const {
        return _DerivedType(-this->X, -this->Y, -this->Z, -this->W);
    }

    constexpr void operator-=(const _DerivedType& B) { *this = *this - B; }

    constexpr void operator-=(ComponentType B) { *this = *this - B; }

    constexpr _DerivedType operator*(const _DerivedType& B) const {
        return _DerivedType(this->X * B.X, this->Y * B.Y, this->Z * B.Z,
                            this->W * B.W);
    }

    constexpr _DerivedType operator*(ComponentType B) const {
        return _DerivedType(this->X * B, this->Y * B, this->Z * B, this->W * B);
    }

    constexpr void operator*=(const _DerivedType& B) { *this = *this * B; }

    constexpr void operator*=(ComponentType B) { *this = *this * B; }

    constexpr _DerivedType operator/(const _DerivedType& B) const {
        return _DerivedType(this->X / B.X, this->Y / B.Y, this->Z / B.Z,
                            this->W / B.W);
    }

    constexpr _DerivedType operator/(ComponentType B) const {
        if CONSTEXPR_IF (std::is_integral_v<ComponentType>) {
            return _DerivedType(this->X / B, this->Y / B, this->Z / B,
                                this->W / B);
        } else {
            return this->operator*(static_cast<ComponentType>(1) / B);
        }
    }

    constexpr void operator/=(const _DerivedType& B) { *this = *this / B; }

    constexpr void operator/=(ComponentType B) { *this = *this / B; }

    constexpr _DerivedType operator%(const _DerivedType& B) const {
        if CONSTEXPR_IF (std::is_integral_v<ComponentType>) {
            return _DerivedType(this->X % B.X, this->Y % B.Y, this->Z % B.Z,
                                this->W % B.W);
        } else {
            return _DerivedType(
                std::fmod(this->X, B.X), std::fmod(this->Y, B.Y),
                std::fmod(this->Z, B.Z), std::fmod(this->W, B.W));
        }
    }

    constexpr _DerivedType operator%(ComponentType B) const {
        if CONSTEXPR_IF (std::is_integral_v<ComponentType>) {
            return _DerivedType(this->X % B, this->Y % B, this->Z % B,
                                this->W % B);
        } else {
            return _DerivedType(std::fmod(this->X, B), std::fmod(this->Y, B),
                                std::fmod(this->Z, B), std::fmod(this->W, B));
        }
    }

    constexpr void operator%=(const _DerivedType& B) { *this = *this % B; }

    constexpr void operator%=(ComponentType B) { *this = *this % B; }

    constexpr bool operator==(const _DerivedType& B) const {
        return this->X == B.X && this->Y == B.Y && this->Z == B.Z &&
               this->W == B.W;
    }
    constexpr bool operator!=(const _DerivedType& B) const {
        return this->X != B.X || this->Y != B.Y || this->Z != B.Z ||
               this->W != B.W;
    }

    constexpr ComponentType Dot(const _DerivedType& B) const {
        return (this->X * B.X) + (this->Y * B.Y) + (this->Z * B.Z) +
               (this->W * B.W);
    }

    constexpr ComponentType Magnitude() const {
        if CONSTEXPR_IF (std::is_integral_v<ComponentType>) {
            return static_cast<ComponentType>(
                std::sqrtl(static_cast<long double>(
                    (this->X * this->X) + (this->Y * this->Y) +
                    (this->Z * this->Z) + (this->W * this->W))));
        } else {
            return std::sqrt((this->X * this->X) + (this->Y * this->Y) +
                             (this->Z * this->Z) + (this->W * this->W));
        }
    }

    constexpr _DerivedType Lerp(const _DerivedType& B, float Alpha) const {
        return *(_DerivedType*)this + (B - *(_DerivedType*)this) * Alpha;
    }

    constexpr _DerivedType Lerp(const _DerivedType& B,
                                const _DerivedType& Alpha) const {
        return *(_DerivedType*)this + (B - *(_DerivedType*)this) * Alpha;
    }
};

struct Math::Vector4 : public Math::etc::_base_vector4<float, Vector4> {

    using _base_vector4::_base_vector4;

    SSE3_FUNCTION
    constexpr Math::Vector4 Normalize() const {
#ifdef USE_SIMD_INTRINSICS
        return this->operator*(_mm_rsqrt_ss(_mm_set_ss(this->Dot(*this)))[0]);
#else
        return this->operator/(std::sqrt(this->Dot(*this)));
#endif
    }

    constexpr Math::Vector4 Floor() const {
        return Math::Vector4(std::floor(this->X), std::floor(this->Y),
                             std::floor(this->Z), std::floor(this->W));
    }

    constexpr Math::Vector4 Round() const {
        return Math::Vector4(std::round(this->X), std::round(this->Y),
                             std::round(this->Z), std::round(this->W));
    }

    constexpr Math::Vector4 Ceil() const {
        return Math::Vector4(std::ceil(this->X), std::ceil(this->Y),
                             std::ceil(this->Z), std::ceil(this->W));
    }

    constexpr operator Math::IVector4() const;
    constexpr operator Math::UVector4() const;
};

constexpr Math::Vector4 operator+(Math::Vector4::ComponentType A,
                                  const Math::Vector4& B) {
    return Math::Vector4(A + B.X, A + B.Y, A + B.Z, A + B.W);
}

constexpr Math::Vector4 operator-(Math::Vector4::ComponentType A,
                                  const Math::Vector4& B) {
    return Math::Vector4(A - B.X, A - B.Y, A - B.Z, A - B.W);
}

constexpr Math::Vector4 operator*(Math::Vector4::ComponentType A,
                                  const Math::Vector4& B) {
    return Math::Vector4(A * B.X, A * B.Y, A * B.Z, A * B.W);
}

constexpr Math::Vector4 operator/(Math::Vector4::ComponentType A,
                                  const Math::Vector4& B) {
    return Math::Vector4(A / B.X, A / B.Y, A / B.Z, A / B.W);
}

constexpr Math::Vector4 operator%(Math::Vector4::ComponentType A,
                                  const Math::Vector4& B) {
    return Math::Vector4(std::fmod(A, B.X), std::fmod(A, B.Y),
                         std::fmod(A, B.Z), std::fmod(A, B.W));
}

struct Math::IVector4 : Math::etc::_base_vector4<Sint32, IVector4> {
    using _base_vector4<Sint32, IVector4>::_base_vector4;

    constexpr operator Math::Vector4() const;
    constexpr operator Math::UVector4() const;
};

constexpr Math::IVector4 operator+(Math::IVector4::ComponentType A,
                                   const Math::IVector4& B) {
    return Math::IVector4(A + B.X, A + B.Y, A + B.Z, A + B.W);
}

constexpr Math::IVector4 operator-(Math::IVector4::ComponentType A,
                                   const Math::IVector4& B) {
    return Math::IVector4(A - B.X, A - B.Y, A - B.Z, A - B.W);
}

constexpr Math::IVector4 operator*(Math::IVector4::ComponentType A,
                                   const Math::IVector4& B) {
    return Math::IVector4(A * B.X, A * B.Y, A * B.Z, A * B.W);
}

constexpr Math::IVector4 operator/(Math::IVector4::ComponentType A,
                                   const Math::IVector4& B) {
    return Math::IVector4(A / B.X, A / B.Y, A / B.Z, A / B.W);
}

constexpr Math::IVector4 operator%(Math::IVector4::ComponentType A,
                                   const Math::IVector4& B) {
    return Math::IVector4(A % B.X, A % B.Y, A % B.Z, A % B.W);
}

struct Math::UVector4 : Math::etc::_base_vector4<Uint32, UVector4> {
    using _base_vector4::_base_vector4;

    constexpr operator Math::Vector4() const;
    constexpr operator Math::IVector4() const;
};

constexpr Math::UVector4 operator+(Math::UVector4::ComponentType A,
                                   const Math::UVector4& B) {
    return Math::UVector4(A + B.X, A + B.Y, A + B.Z, A + B.W);
}

constexpr Math::UVector4 operator-(Math::UVector4::ComponentType A,
                                   const Math::UVector4& B) {
    return Math::UVector4(A - B.X, A - B.Y, A - B.Z, A - B.W);
}

constexpr Math::UVector4 operator*(Math::UVector4::ComponentType A,
                                   const Math::UVector4& B) {
    return Math::UVector4(A * B.X, A * B.Y, A * B.Z, A * B.W);
}

constexpr Math::UVector4 operator/(Math::UVector4::ComponentType A,
                                   const Math::UVector4& B) {
    return Math::UVector4(A / B.X, A / B.Y, A / B.Z, A / B.W);
}

constexpr Math::UVector4 operator%(Math::UVector4::ComponentType A,
                                   const Math::UVector4& B) {
    return Math::UVector4(A % B.X, A % B.Y, A % B.Z, A % B.W);
}

constexpr Math::Vector4::operator Math::IVector4() const {
    return Math::IVector4(static_cast<Math::IVector4::ComponentType>(this->X),
                          static_cast<Math::IVector4::ComponentType>(this->Y),
                          static_cast<Math::IVector4::ComponentType>(this->Z),
                          static_cast<Math::IVector4::ComponentType>(this->W));
}

constexpr Math::Vector4::operator Math::UVector4() const {
    return Math::UVector4(static_cast<Math::UVector4::ComponentType>(this->X),
                          static_cast<Math::UVector4::ComponentType>(this->Y),
                          static_cast<Math::UVector4::ComponentType>(this->Z),
                          static_cast<Math::UVector4::ComponentType>(this->W));
}

constexpr Math::IVector4::operator Math::Vector4() const {
    return Math::Vector4(static_cast<Math::Vector4::ComponentType>(this->X),
                         static_cast<Math::Vector4::ComponentType>(this->Y),
                         static_cast<Math::Vector4::ComponentType>(this->Z),
                         static_cast<Math::Vector4::ComponentType>(this->W));
}

constexpr Math::IVector4::operator Math::UVector4() const {
    return Math::UVector4(static_cast<Math::UVector4::ComponentType>(this->X),
                          static_cast<Math::UVector4::ComponentType>(this->Y),
                          static_cast<Math::UVector4::ComponentType>(this->Z),
                          static_cast<Math::UVector4::ComponentType>(this->W));
}

constexpr Math::UVector4::operator Math::Vector4() const {
    return Math::Vector4(static_cast<Math::Vector4::ComponentType>(this->X),
                         static_cast<Math::Vector4::ComponentType>(this->Y),
                         static_cast<Math::Vector4::ComponentType>(this->Z),
                         static_cast<Math::Vector4::ComponentType>(this->W));
}

constexpr Math::UVector4::operator Math::IVector4() const {
    return Math::IVector4(static_cast<Math::IVector4::ComponentType>(this->X),
                          static_cast<Math::IVector4::ComponentType>(this->Y),
                          static_cast<Math::IVector4::ComponentType>(this->Z),
                          static_cast<Math::IVector4::ComponentType>(this->W));
}
#endif