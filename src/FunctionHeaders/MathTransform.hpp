#pragma once

#include <cmath>

#include "../MathClasses/Matrix.hpp"
#include "../MathClasses/Vector3.hpp"

namespace Math::Transform {

    constexpr Math::Mat4 Translate(const Math::Mat4& Mat,
                                   const Math::Vector3& Translate);

    constexpr Math::Mat4 LookAt(const Math::Vector3& Eye,
                                const Math::Vector3& Center,
                                const Math::Vector3& Up);

    constexpr Math::Mat4 Perspective(float FOV,
                                     float aspect,
                                     float zNear,
                                     float zFar);

    constexpr Math::Mat4 Orthographic(float Left,
                                      float Right,
                                      float Bottom,
                                      float Top);

    constexpr Math::Mat4 Rotate(const Math::Mat4& Matrix,
                                const Math::Vector3& Rotate);

    constexpr Math::Mat4 RotateAxis(const Math::Mat4& Matrix,
                                    float Angle,
                                    const Math::Vector3& V);
}

constexpr Math::Mat4 Math::Transform::Translate(
    const Math::Mat4& Mat,
    const Math::Vector3& Translate) {
    return Math::Mat4(Mat[0], Mat[1], Mat[2],
                      Mat[0] * Translate[0] + Mat[1] * Translate[1] +
                          Mat[2] * Translate[2] + Mat[3]);
}

constexpr Math::Mat4 Math::Transform::LookAt(const Math::Vector3& Eye,
                                             const Math::Vector3& Center,
                                             const Math::Vector3& Up) {
    Math::Mat4 Result(1.f);
    Math::Vector3 F, S, U;

    F = (Center - Eye).Normalize();
    S = F.Cross(Up).Normalize();
    U = S.Cross(F);

    Result[0][0] = S.X;
    Result[1][0] = S.Y;
    Result[2][0] = S.Z;

    Result[0][1] = U.X;
    Result[1][1] = U.Y;
    Result[2][1] = U.Z;

    Result[0][2] = -F.X;
    Result[1][2] = -F.Y;
    Result[2][2] = -F.Z;

    Result[3][0] = -S.Dot(Eye);
    Result[3][1] = -U.Dot(Eye);
    Result[3][2] = F.Dot(Eye);

    return Result;
}

constexpr Math::Mat4 Math::Transform::Perspective(float FOV,
                                                  float aspect,
                                                  float zNear,
                                                  float zFar) {
    // assert(abs(aspect - std::numeric_limits<T>::epsilon()) >
    // static_cast<T>(0));

    Math::Mat4 Result(0.f);
    const float tanHalfFovy = std::tan(FOV / 2.f);

    Result.R0.X = 1.f / (aspect * tanHalfFovy);
    Result.R1.Y = 1.f / (tanHalfFovy);
    Result.R2.Z = -(zFar + zNear) / (zFar - zNear);
    Result.R2.W = -1.f;
    Result.R3.Z = -(2.f * zFar * zNear) / (zFar - zNear);

    return Result;
}

constexpr Math::Mat4 Math::Transform::Orthographic(float Left,
                                                   float Right,
                                                   float Bottom,
                                                   float Top) {
    Math::Mat4 Result(1.f);
    Result.R0.X = 2.f / (Right - Left);
    Result.R1.Y = 2.f / (Top - Bottom);
    Result.R2.Z = -1.f;
    Result.R3.X = -(Right + Left) / (Right - Left);
    Result.R3.Y = -(Top + Bottom) / (Top - Bottom);
    return Result;
}

constexpr Math::Mat4 Math::Transform::RotateAxis(const Math::Mat4& Matrix,
                                                 float Angle,
                                                 const Math::Vector3& V) {
    Math::Mat4 Result, Rotate;

    const float AngleCosine = std::cos(Angle);
    const float AngleSine = std::sin(Angle);

    const Math::Vector3 Axis = V.Normalize();
    const Math::Vector3 Temp = (1.f - AngleCosine) * Axis;

    Rotate.R0.X = AngleCosine + Temp.X * Axis.X;
    Rotate.R0.Y = Temp.X * Axis.Y + AngleSine * Axis.Z;
    Rotate.R0.Z = Temp.X * Axis.Z - AngleSine * Axis.Y;

    Rotate.R1.X = Temp.Y * Axis.X - AngleSine * Axis.Z;
    Rotate.R1.Y = AngleCosine + Temp.Y * Axis.Y;
    Rotate.R1.Z = Temp.Y * Axis.Z + AngleSine * Axis.X;

    Rotate.R2.X = Temp.Z * Axis.X + AngleSine * Axis.Y;
    Rotate.R2.Y = Temp.Z * Axis.Y - AngleSine * Axis.X;
    Rotate.R2.Z = AngleCosine + Temp.Z * Axis.Z;

    Result.R0 = Matrix.R0 * Rotate.R0.X + Matrix.R1 * Rotate.R0.Y +
                Matrix.R2 * Rotate.R0.Z;
    Result.R1 = Matrix.R0 * Rotate.R1.X + Matrix.R1 * Rotate.R1.Y +
                Matrix.R2 * Rotate.R1.Z;
    Result.R2 = Matrix.R0 * Rotate.R2.X + Matrix.R1 * Rotate.R2.Y +
                Matrix.R2 * Rotate.R2.Z;
    Result.R3 = Matrix.R3;
    return Result;
}

constexpr Math::Mat4 Math::Transform::Rotate(const Math::Mat4& Mat4,
                                             const Math::Vector3& Rotate) {
    constexpr float degToRad = 3.14159265359f / 180.f;
    const float radX = Rotate.X * degToRad;
    const float radY = Rotate.Y * degToRad;
    const float radZ = Rotate.Z * degToRad;

    const float cx = std::cos(radX), sx = std::sin(radX);
    const float cy = std::cos(radY), sy = std::sin(radY);
    const float cz = std::cos(radZ), sz = std::sin(radZ);

    // i have assign the columns using vectors
    // because for some reason doing it component by component gives me an error :(
    const Math::Mat4 rotX(
        Math::Vector4(1.f, 0.f, 0.f, 0.f), Math::Vector4(0.f, cx, sx, 0.f),
        Math::Vector4(0.f, -sx, cx, 0.f), Math::Vector4(0.f, 0.f, 0.f, 1.f));

    const Math::Mat4 rotY(
        Math::Vector4(cy, 0.f, -sy, 0.f), Math::Vector4(0.f, 1.f, 0.f, 0.f),
        Math::Vector4(sy, 0.f, cy, 0.f), Math::Vector4(0.f, 0.f, 0.f, 1.f));

    const Math::Mat4 rotZ(
        Math::Vector4(cz, sz, 0.f, 0.f), Math::Vector4(-sz, cz, 0.f, 0.f),
        Math::Vector4(0.f, 0.f, 1.f, 0.f), Math::Vector4(0.f, 0.f, 0.f, 1.f));

    return Mat4 * (rotZ * rotY * rotX);
}