#ifndef UTIL_VECTOR3INT32_H
#define UTIL_VECTOR3INT32_H

#include "decomp.h"

#include <G3D/Vector3.h>
#include <cmath>

namespace RBX {

using G3D::Vector3;

class Vector3int32
{
public:
	Vector3int32() : x(0), y(0), z(0) {}

	Vector3int32(int x, int y, int z) : x(x), y(y), z(z) {}

	static Vector3int32 floor(const Vector3& value);

	Vector3int32 operator+(const Vector3int32& other) const;

	bool operator==(const Vector3int32& other) const { return x == other.x && y == other.y && z == other.z; }
	bool operator!=(const Vector3int32& other) const { return !(*this == other); }

	Vector3 toVector3() const;

	int x; // 0x00
	int y; // 0x04
	int z; // 0x08
};

DECOMP_SIZE_ASSERT(Vector3int32, 0x0c)

// FUNCTION: WEBSERVICE 0x10123340
inline Vector3int32 Vector3int32::floor(const Vector3& value)
{
	return Vector3int32(G3D::iRound(floorf(value.x)), G3D::iRound(floorf(value.y)), G3D::iRound(floorf(value.z)));
}

} // namespace RBX

#endif // UTIL_VECTOR3INT32_H
