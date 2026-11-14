#ifndef UTIL_VECTOR3INT32_H
#define UTIL_VECTOR3INT32_H

#include "placeholder.h"

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

	int& operator[](int index) { return (&x)[index]; }
	const int& operator[](int index) const { return (&x)[index]; }

	Vector3 toVector3() const { return Vector3((float) x, (float) y, (float) z); }

	int x;
	int y;
	int z;
};

inline Vector3int32 Vector3int32::floor(const Vector3& v)
{
	return Vector3int32(G3D::iRound(floorf(v.x)), G3D::iRound(floorf(v.y)), G3D::iRound(floorf(v.z)));
}

inline bool operator<(const Vector3int32& a, const Vector3int32& b)
{
	for (int i = 0; i < 3; i++) {
		if (a[i] < b[i]) {
			return true;
		}
		if (a[i] > b[i]) {
			return false;
		}
	}

	return false;
}

} // namespace RBX

#endif // UTIL_VECTOR3INT32_H
