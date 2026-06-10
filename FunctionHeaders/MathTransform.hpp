#pragma once

#include <cmath>

#include "../MathClasses/Vector3.hpp"
#include "../MathClasses/Matrix.hpp"


namespace Math::Transform {

	constexpr Math::Mat4 LookAt(const Math::Vector3& Eye, const Math::Vector3& Center, const Math::Vector3& Up) {

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

	constexpr Math::Mat4 Perspective(float FOV, float aspect, float zNear, float zFar) {

		//assert(abs(aspect - std::numeric_limits<T>::epsilon()) > static_cast<T>(0));

		Math::Mat4 Result(0.f);
		const float tanHalfFovy = std::tan(FOV / 2.f);

		Result[0][0] = 1.f / (aspect * tanHalfFovy);
		Result[1][1] = 1.f / (tanHalfFovy);
		Result[2][2] = -(zFar + zNear) / (zFar - zNear);
		Result[2][3] = -1.f;
		Result[3][2] = -(2.f * zFar * zNear) / (zFar - zNear);

		return Result;
	}

	constexpr Math::Mat4 Rotate(const Math::Mat4& Matrix, float Angle, const Math::Vector3& V) {

		Math::Mat4 Result, Rotate;

		const float AngleCosine = std::cos(Angle);
		const float AngleSine = std::sin(Angle);

		const Math::Vector3 Axis = V.Normalize();
		const Math::Vector3 Temp = (1.f - AngleCosine) * Axis;

		Rotate[0].X = AngleCosine + Temp.X * Axis.X;
		Rotate[0].Y = Temp.X * Axis.Y + AngleSine * Axis.Z;
		Rotate[0].Z = Temp.X * Axis.Z - AngleSine * Axis.Y;

		Rotate[1].X = Temp.Y * Axis.X - AngleSine * Axis.Z;
		Rotate[1].Y = AngleCosine + Temp.Y * Axis.Y;
		Rotate[1].Z = Temp.Y * Axis.Z + AngleSine * Axis.X;

		Rotate[2].X = Temp.Z * Axis.X + AngleSine * Axis.Y;
		Rotate[2].Y = Temp.Z * Axis.Y - AngleSine * Axis.X;
		Rotate[2].Z = AngleCosine + Temp.Z * Axis.Z;

		Result[0] = Matrix[0] * Rotate[0].X + Matrix[1] * Rotate[0].Y + Matrix[2] * Rotate[0].Z;
		Result[1] = Matrix[0] * Rotate[1].X + Matrix[1] * Rotate[1].Y + Matrix[2] * Rotate[1].Z;
		Result[2] = Matrix[0] * Rotate[2].X + Matrix[1] * Rotate[2].Y + Matrix[2] * Rotate[2].Z;
		Result[3] = Matrix[3];
		return Result;
	}
}