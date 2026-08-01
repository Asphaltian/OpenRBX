#ifndef UTIL_MATH_H
#define UTIL_MATH_H

#include "util/NormalId.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/Matrix3.h>
#include <G3D/Vector3.h>

namespace RBX {

using G3D::CoordinateFrame;
using G3D::Matrix3;
using G3D::Vector3;

unsigned char rotationToByteBase(float rotation);

namespace Math {

bool isDenormal(float value);
bool fixDenorm(Vector3& value);
bool legalCameraCoord(const CoordinateFrame& coordinateFrame);
float rotationFromByte(unsigned char byte);
Vector3 toDiagonal(const Matrix3& matrix);
Matrix3 fromDiagonal(const Vector3& diagonal);
bool isAxisAligned(const Matrix3& matrix);
Vector3 iRoundVector3(const Vector3& value);
float angle(const Vector3& v0, const Vector3& v1);
bool fuzzyEq(const Vector3& v0, const Vector3& v1, float epsilon);
bool fuzzyEq(const Matrix3& m0, const Matrix3& m1, float epsilon);
NormalId getClosestObjectNormalId(const Vector3& worldNormal, const Matrix3& objectRotation);
Vector3 vector3Abs(const Vector3& value);
float getHeading(const Vector3& look);
Vector3 toGrid(const Vector3& value, const Vector3& grid);
Vector3 sortVector3(const Vector3& value);
float maxAxisLength(const Vector3& value);

} // namespace Math
} // namespace RBX

#endif // UTIL_MATH_H
