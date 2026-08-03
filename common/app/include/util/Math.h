#ifndef UTIL_MATH_H
#define UTIL_MATH_H

#include "util/NormalId.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/Matrix3.h>
#include <G3D/Plane.h>
#include <G3D/Ray.h>
#include <G3D/Vector2.h>
#include <G3D/Vector3.h>
#include <cmath>
#include <limits>

namespace RBX {

using G3D::CoordinateFrame;
using G3D::Matrix3;
using G3D::Plane;
using G3D::Ray;
using G3D::Vector2;
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

	static Vector3 getColumn(const Matrix3& matrix, int column)
	{
		return Vector3(matrix[0][column], matrix[1][column], matrix[2][column]);
	}

	static Vector3 getWorldNormal(NormalId normalId, const Matrix3& rotation)
	{
		int face = normalId / 3;
		int axis = normalId - face * 3;

		return Vector3(rotation[0][axis], rotation[1][axis], rotation[2][axis]) * (float) (1 - face * 2);
	}

	static Vector3 getWorldNormal(NormalId normalId, const CoordinateFrame& coord)
	{
		return getWorldNormal(normalId, coord.rotation);
	}

	static float taxiCabMagnitude(const Vector3& value) { return fabs(value.x) + fabs(value.y) + fabs(value.z); }

	static float sign(float value)
	{
		if (value > 0.0f) {
			return 1.0f;
		}

		if (value < 0.0f) {
			return -1.0f;
		}

		return 0.0f;
	}

	static bool isDenormal(float value);
	static bool isNanInfDenorm(float value);
	static bool isNanInfDenormVector3(const Vector3& value);
	static Matrix3 momentToObjectSpace(const Matrix3& moment, const Matrix3& rotation);
	static Matrix3 momentToWorldSpace(const Matrix3& moment, const Matrix3& rotation);
	static float angleToE0(const Vector2& value);
	static unsigned char rotationToByte(float rotation);
	static Vector3 toGrid(const Vector3& value, float grid);
	static bool orthonormalizeIfNecessary(Matrix3& matrix);
	static int getOrientId(const Matrix3& matrix);
	static const Matrix3& matrixTiltZ();
	static const Matrix3& matrixRotateY();
	static void getHeadingElevation(const CoordinateFrame& coordinateFrame, float& heading, float& elevation);
	static void setHeadingElevation(CoordinateFrame& coordinateFrame, float heading, float elevation);
	static Matrix3 snapToAxes(const Matrix3& matrix);
	static CoordinateFrame snapToGrid(const CoordinateFrame& coordinateFrame, const Vector3& grid);
	static CoordinateFrame snapToGrid(const CoordinateFrame& coordinateFrame, float grid);
	static bool fuzzyEq(
		const CoordinateFrame& c0,
		const CoordinateFrame& c1,
		float translationEpsilon,
		float rotationEpsilon
	);
	static void idToMatrix3(int orientId, Matrix3& matrix);
	static Matrix3 rotateAboutZ(const Matrix3& matrix, float angle);
	static void rotateMatrixAboutY90(Matrix3& matrix, int count);
	static CoordinateFrame getFocusSpace(const CoordinateFrame& coordinateFrame);
	static Matrix3 getIWorldAtPoint(const Vector3& cofm, const Vector3& point, const Matrix3& iWorld, float mass);
	static bool fuzzyAxisAligned(const Matrix3& m0, const Matrix3& m1, float epsilon);
	static const Matrix3& getAxisRotationMatrix(int normalId);
	static bool intersectRayPlane(const Ray& ray, const Plane& plane, Vector3& hitPoint);
	static Matrix3 alignAxesClosest(const Matrix3& matrix, const Matrix3& target);
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
