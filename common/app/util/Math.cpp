#include "util/Math.h"

#include "decomp.h"

#include <G3D/g3dmath.h>
#include <algorithm>

namespace RBX {

static float rotationStep()
{
	static const float step = static_cast<float>(2.0 * G3D::pi() / 256.0);
	return step;
}

// FUNCTION: WEBSERVICE 0x100de860
bool Math::isDenormal(float value)
{
	const int bits = *reinterpret_cast<const int*>(&value);
	return (bits & 0x7f800000) == 0 && (bits & 0x7fffff) != 0;
}

// FUNCTION: WEBSERVICE 0x100de930
bool Math::fixDenorm(Vector3& value)
{
	bool fixed = false;

	if (isDenormal(value.x)) {
		value.x = 0.0f;
		fixed = true;
	}
	if (isDenormal(value.y)) {
		value.y = 0.0f;
		fixed = true;
	}
	if (isDenormal(value.z)) {
		value.z = 0.0f;
		fixed = true;
	}

	return fixed;
}

// STUB: WEBSERVICE 0x100de9a0
bool Math::legalCameraCoord(const CoordinateFrame& coordinateFrame)
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (coordinateFrame.rotation[i][j] <= -1.2f) {
				return false;
			}
			if (coordinateFrame.rotation[i][j] >= 1.2f) {
				return false;
			}
		}
		if (coordinateFrame.translation[i] <= -1000000.0f) {
			return false;
		}
		if (coordinateFrame.translation[i] >= 1000000.0f) {
			return false;
		}
	}

	return true;
}

// STUB: WEBSERVICE 0x100dea30
float Math::rotationFromByte(unsigned char byte)
{
	const float value = byte;

	return value * rotationStep() - 3.14159274f;
}

// FUNCTION: WEBSERVICE 0x100debb0
Vector3 Math::toDiagonal(const Matrix3& matrix)
{
	return Vector3(matrix[0][0], matrix[1][1], matrix[2][2]);
}

// FUNCTION: WEBSERVICE 0x100debd0
Matrix3 Math::fromDiagonal(const Vector3& diagonal)
{
	return Matrix3(diagonal.x, 0.0f, 0.0f, 0.0f, diagonal.y, 0.0f, 0.0f, 0.0f, diagonal.z);
}

// FUNCTION: WEBSERVICE 0x100dec20
bool Math::isAxisAligned(const Matrix3& matrix)
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			const float value = matrix[i][j];
			if (value != 0.0f && value != 1.0f && value != -1.0f) {
				return false;
			}
		}
	}

	return true;
}

// FUNCTION: WEBSERVICE 0x100df0a0
Vector3 Math::iRoundVector3(const Vector3& value)
{
	return Vector3(
		static_cast<float>(G3D::iRound(value.x)),
		static_cast<float>(G3D::iRound(value.y)),
		static_cast<float>(G3D::iRound(value.z))
	);
}

// FUNCTION: WEBSERVICE 0x100df150
float Math::angle(const Vector3& v0, const Vector3& v1)
{
	const float dot = v0.dot(v1);

	if (dot >= 1.0f) {
		return 0.0f;
	}
	if (dot <= -1.0f) {
		return 3.14159274f;
	}

	return acosf(dot);
}

// FUNCTION: WEBSERVICE 0x100df1a0
bool Math::fuzzyEq(const Vector3& v0, const Vector3& v1, float epsilon)
{
	for (int i = 0; i < 3; ++i) {
		const float a = v0[i];
		const float b = v1[i];

		if (a != b && !(fabsf(a - b) <= (fabsf(a) + 1.0f) * epsilon)) {
			return false;
		}
	}

	return true;
}

// FUNCTION: WEBSERVICE 0x100df200
bool Math::fuzzyEq(const Matrix3& m0, const Matrix3& m1, float epsilon)
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			const float a = m0[i][j];
			const float b = m1[i][j];

			if (a != b && !(fabsf(a - b) <= (fabsf(a) + 1.0f) * epsilon)) {
				return false;
			}
		}
	}

	return true;
}

// STUB: WEBSERVICE 0x100df530
NormalId Math::getClosestObjectNormalId(const Vector3& worldNormal, const Matrix3& objectRotation)
{
	const Vector3 objectNormal = worldNormal * objectRotation;

	const float x = fabsf(objectNormal.x);
	const float y = fabsf(objectNormal.y);
	const float z = fabsf(objectNormal.z);

	if (x > y) {
		if (x > z) {
			return objectNormal.x > 0.0f ? NORM_X : NORM_X_NEG;
		}
	}
	else if (y > z) {
		return objectNormal.y > 0.0f ? NORM_Y : NORM_Y_NEG;
	}

	return objectNormal.z > 0.0f ? NORM_Z : NORM_Z_NEG;
}

// STUB: WEBSERVICE 0x100df600
Vector3 Math::vector3Abs(const Vector3& value)
{
	STUB(0x100df600);
	return Vector3::zero();
}

// FUNCTION: WEBSERVICE 0x100df620
float Math::getHeading(const Vector3& look)
{
	return atan2f(-look.x, -look.z);
}

// STUB: WEBSERVICE 0x100dfaf0
unsigned char rotationToByteBase(float rotation)
{
	const int index = G3D::iRound((rotation + 3.14159274f) / rotationStep());

	return static_cast<unsigned char>(index < 1 ? 0 : (index > 254 ? 255 : index));
}

// STUB: WEBSERVICE 0x100dfb90
Vector3 Math::toGrid(const Vector3& value, const Vector3& grid)
{
	return Vector3(
		grid.x * G3D::iRound(value.x / grid.x),
		grid.y * G3D::iRound(value.y / grid.y),
		grid.z * G3D::iRound(value.z / grid.z)
	);
}

// FUNCTION: WEBSERVICE 0x100dfe40
Vector3 Math::sortVector3(const Vector3& value)
{
	Vector3 sorted = value;

	if (sorted.z < sorted.y) {
		std::swap(sorted.z, sorted.y);
	}
	if (sorted.y < sorted.x) {
		std::swap(sorted.y, sorted.x);
	}
	if (sorted.z < sorted.y) {
		std::swap(sorted.z, sorted.y);
	}

	return sorted;
}

// FUNCTION: WEBSERVICE 0x100dfea0
float Math::maxAxisLength(const Vector3& value)
{
	const float lengths[3] = {fabsf(value.x), fabsf(value.y), fabsf(value.z)};

	return std::max(lengths[0], std::max(lengths[1], lengths[2]));
}

} // namespace RBX
