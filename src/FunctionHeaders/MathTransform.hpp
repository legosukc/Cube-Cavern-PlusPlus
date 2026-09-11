#pragma once

#include "../MathClasses/Matrix.hpp"
#include "../MathClasses/Vector3.hpp"

namespace Math::Transform {

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
                                float Angle,
                                const Math::Vector3& V);
}