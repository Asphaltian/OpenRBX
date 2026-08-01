#ifndef UTIL_MATH_H
#define UTIL_MATH_H

#include "util/NormalId.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/Matrix3.h>
#include <G3D/Vector3.h>
#include <limits>

namespace RBX {

using G3D::CoordinateFrame;
using G3D::Matrix3;
using G3D::Vector3;

unsigned char rotationToByteBase(float rotation);

class Math
{
public:
	// FUNCTION: WEBSERVICE 0x100a0670
	static const float& inf()
	{
		static const float i = std::numeric_limits<float>::infinity();

		return i;
	}

	static bool isDenormal(float value);
	static bool fixDenorm(Vector3& value);
	static bool legalCameraCoord(const CoordinateFrame& coordinateFrame);
	static float rotationFromByte(unsigned char byte);
	static Vector3 toDiagonal(const Matrix3& matrix);
	static Matrix3 fromDiagonal(const Vector3& diagonal);
	static bool isAxisAligned(const Matrix3& matrix);
	static Vector3 iRoundVector3(const Vector3& value);
	static float angle(const Vector3& v0, const Vector3& v1);
	static bool fuzzyEq(const Vector3& v0, const Vector3& v1, float epsilon);
	static bool fuzzyEq(const Matrix3& m0, const Matrix3& m1, float epsilon);
	static NormalId getClosestObjectNormalId(const Vector3& worldNormal, const Matrix3& objectRotation);
	static Vector3 vector3Abs(const Vector3& value);
	static float getHeading(const Vector3& look);
	static Vector3 toGrid(const Vector3& value, const Vector3& grid);
	static Vector3 sortVector3(const Vector3& value);
	static float maxAxisLength(const Vector3& value);
};
} // namespace RBX

#endif // UTIL_MATH_H
