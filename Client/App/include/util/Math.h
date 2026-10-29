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

namespace G3D {
class GCamera;
class Line;
} // namespace G3D

namespace RBX {

using G3D::CoordinateFrame;
using G3D::Matrix3;
using G3D::Plane;
using G3D::Ray;
using G3D::Vector2;
using G3D::Vector3;

class Velocity;

unsigned char rotationToByteBase(float rotation);

class Math
{
public:
	static const float pi();
	static const float piHalf();
	static const float twoPi();

	// FUNCTION: WEBSERVICE 0x100a0670
	static const float& inf()
	{
		static const float i = std::numeric_limits<float>::infinity();

		return i;
	}

	static int iRound(float);
	static int iFloor(float);
	static float polarity(float);

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
	static bool isNanInfDenormMatrix3(const Matrix3&);
	static bool fixDenorm(float&);
	static bool fixDenorm(Vector3& value);

	static bool fuzzyEq(float f0, float f1, float epsilon)
	{
		return f0 == f1 || fabsf(f0 - f1) <= (fabsf(f0) + 1.0f) * epsilon;
	}

	static bool fuzzyEq(const Vector3& v0, const Vector3& v1, float epsilon);
	static bool fuzzyEq(const Matrix3& m0, const Matrix3& m1, float epsilon);
	static bool fuzzyEq(
		const CoordinateFrame& c0,
		const CoordinateFrame& c1,
		float translationEpsilon,
		float rotationEpsilon
	);
	static bool fuzzyAxisAligned(const Matrix3& m0, const Matrix3& m1, float epsilon);
	static bool isEven(int);
	static bool isOdd(int);
	static int nextEven(int);
	static int nextOdd(int);
	static float angleToE0(const Vector2& value);
	static unsigned int hash(const Vector3&);
	static bool isIntegerVector3(const Vector3&);
	static Vector3 iRoundVector3(const Vector3& value);
	static float angle(const Vector3& v0, const Vector3& v1);
	static float smallAngle(const Vector3&, const Vector3&);
	static float elevationAngle(const Vector3&);
	static Vector3 vector3Abs(const Vector3& value);
	static float volume(const Vector3&);
	static float maxAxisLength(const Vector3& value);
	static Vector3 sortVector3(const Vector3& value);
	static Velocity calcTrajectory(const Vector3&, const Vector3&, float);
	static Vector3 toGrid(const Vector3& value, const Vector3& grid);
	static Vector3 toGrid(const Vector3& value, float grid);
	static bool lessThan(const Vector3&, const Vector3&);
	static float longestVector3Component(const Vector3&);
	static float planarSize(const Vector3&);

	static float taxiCabMagnitude(const Vector3& value) { return fabs(value.x) + fabs(value.y) + fabs(value.z); }

	static const Plane& yPlane();
	static Vector3 closestPointOnRay(const Ray&, const Ray&);
	static bool cameraSeesPoint(const Vector3&, const G3D::GCamera&);
	static bool legalCameraCoord(const CoordinateFrame& coordinateFrame);
	static Vector3 toSmallAngles(const Matrix3&);
	static Matrix3 snapToAxes(const Matrix3& matrix);
	static bool isOrthonormal(const Matrix3&);
	static bool orthonormalizeIfNecessary(Matrix3& matrix);
	static Vector3 toFocusSpace(const Vector3&, const CoordinateFrame&);
	static Vector3 fromFocusSpace(const Vector3&, const CoordinateFrame&);
	static Vector3 toDiagonal(const Matrix3& matrix);
	static Matrix3 fromDiagonal(const Vector3& diagonal);

	static Vector3 getColumn(const Matrix3& matrix, int column)
	{
		return Vector3(matrix[0][column], matrix[1][column], matrix[2][column]);
	}

	static unsigned char rotationToByte(float rotation);
	static float rotationFromByte(unsigned char byte);
	static bool isAxisAligned(const Matrix3& matrix);
	static int getOrientId(const Matrix3& matrix);
	static void idToMatrix3(int orientId, Matrix3& matrix);
	static const Matrix3& matrixRotateY();
	static const Matrix3& matrixRotateNegativeY();
	static const Matrix3& matrixTiltZ();
	static const Matrix3& matrixTiltNegativeZ();
	static const Matrix3 matrixTiltQuadrant(int);
	static void rotateMatrixAboutX90(Matrix3&, int);
	static void rotateMatrixAboutY90(Matrix3& matrix, int count);
	static void rotateMatrixAboutZ90(Matrix3&);
	static Matrix3 rotateAboutZ(const Matrix3& matrix, float radians);
	static Matrix3 momentToObjectSpace(const Matrix3& moment, const Matrix3& rotation);
	static Matrix3 momentToWorldSpace(const Matrix3& moment, const Matrix3& rotation);
	static Matrix3 getIWorldAtPoint(const Vector3& cofm, const Vector3& point, const Matrix3& iWorld, float mass);
	static void rotateAboutYLocal(CoordinateFrame&, float);
	static void rotateAboutYGlobal(CoordinateFrame&, float);
	static CoordinateFrame snapToGrid(const CoordinateFrame& coordinateFrame, float grid);
	static CoordinateFrame snapToGrid(const CoordinateFrame& coordinateFrame, const Vector3& grid);
	static float zAxisAngle(const CoordinateFrame&);
	static void pan(const Vector3&, CoordinateFrame&, float);
	static int radiansToQuadrant(float);
	static int radiansToOctant(float);
	static float radiansToDegrees(float);
	static float degreesToRadians(float);
	static float getHeading(const Vector3& look);
	static float getElevation(const Vector3&);
	static void getHeadingElevation(const CoordinateFrame& coordinateFrame, float& heading, float& elevation);
	static void setHeadingElevation(CoordinateFrame& coordinateFrame, float heading, float elevation);
	static CoordinateFrame getFocusSpace(const CoordinateFrame& focus);
	static int toYAxisQuadrant(const CoordinateFrame&);
	static Matrix3 alignAxesClosest(const Matrix3& matrix, const Matrix3& target);
	static NormalId getClosestObjectNormalId(const Vector3& worldNormal, const Matrix3& objectRotation);

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

	// FUNCTION: WEBSERVICE 0x100a06a0
	static float radWrap(float rad)
	{
		if (rad >= -3.1415927f && rad < 3.1415927f) {
			return rad;
		}
		return rad - G3D::iRound(floor((rad + 3.1415927f) / 6.283185f)) * 6.283185f;
	}

	static const Matrix3& getAxisRotationMatrix(int face);
	static Vector3 vectorToObjectSpace(const Vector3& vector, const Matrix3& rotation) { return vector * rotation; }
	static bool clipRay(Vector3&, Vector3&, Vector3*, Vector3&);
	static bool intersectLinePlane(const G3D::Line&, const Plane&, Vector3&);
	static bool intersectRayPlane(const Ray& ray, const Plane& plane, Vector3& hitPoint);
	static float computeLaunchAngle(float, float, float, float);
};
} // namespace RBX

#endif // UTIL_MATH_H
