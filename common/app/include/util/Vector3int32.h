#ifndef UTIL_VECTOR3INT32_H
#define UTIL_VECTOR3INT32_H

#include "decomp.h"

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

class Vector3int32
{
public:
	static Vector3int32 floor(const Vector3& value);

	Vector3int32 operator+(const Vector3int32& other) const;
	bool operator==(const Vector3int32& other) const;
	bool operator!=(const Vector3int32& other) const;

	Vector3 toVector3() const;

	int x; // 0x00
	int y; // 0x04
	int z; // 0x08
};

DECOMP_SIZE_ASSERT(Vector3int32, 0x0c)

} // namespace RBX

#endif // UTIL_VECTOR3INT32_H
