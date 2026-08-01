#ifndef UTIL_EXTENTS_H
#define UTIL_EXTENTS_H

#include "decomp.h"
#include "util/NormalId.h"

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

class Extents
{
public:
	Extents() {}
	Extents(const Vector3& low, const Vector3& high) : low(low), high(high) {}

	static Extents fromCenterCorner(const Vector3& center, const Vector3& corner);
	static const Extents& zero();

	const Vector3& min() const { return low; }
	const Vector3& max() const { return high; }

	Vector3 getCorner(int index) const;
	Vector3 faceCenter(NormalId normalId) const;

	float areaXZ() const;

	NormalId closestFace(const Vector3& point) const;

	bool contains(const Vector3& point) const;
	bool fuzzyContains(const Vector3& point, float epsilon) const;
	bool overlapsOrTouches(const Extents& other) const;

	void expand(float distance);

private:
	static float epsilon();

	Vector3 low;  // 0x00
	Vector3 high; // 0x0c
};

DECOMP_SIZE_ASSERT(Extents, 0x18)

inline Extents Extents::fromCenterCorner(const Vector3& center, const Vector3& corner)
{
	return Extents(center - corner, center + corner);
}

inline void Extents::expand(float distance)
{
	low -= Vector3(distance, distance, distance);
	high += Vector3(distance, distance, distance);
}

} // namespace RBX

#endif // UTIL_EXTENTS_H
