#ifndef UTIL_EXTENTS_H
#define UTIL_EXTENTS_H

#include "decomp.h"
#include "util/NormalId.h"
#include "util/Vector3int32.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/GCamera.h>
#include <G3D/Plane.h>
#include <G3D/Vector3.h>

namespace RBX {

using G3D::CoordinateFrame;
using G3D::Plane;
using G3D::Vector3;

class Extents
{
public:
	Extents() : low(Vector3::inf()), high(-Vector3::inf()) {}
	Extents(const Vector3& low, const Vector3& high) : low(low), high(high) {}

	Extents(const Vector3int32& low, const Vector3int32& high)
		: low((float) low.x, (float) low.y, (float) low.z), high((float) high.x, (float) high.y, (float) high.z)
	{
	}

	static Extents fromCenterCorner(const Vector3& center, const Vector3& corner);
	static Extents vv(const Vector3& v0, const Vector3& v1);

	static const Extents& zero();
	static const Extents& negativeInfiniteExtents();

	const Vector3& min() const { return low; }
	const Vector3& max() const { return high; }

	Vector3 getCorner(int index) const;
	Vector3 faceCenter(NormalId normalId) const;
	Plane getPlane(NormalId normalId) const;

	void getFaceCorners(NormalId normalId, Vector3& c0, Vector3& c1, Vector3& c2, Vector3& c3) const;

	float areaXZ() const;

	NormalId closestFace(const Vector3& point);

	bool contains(const Vector3& point) const;
	bool fuzzyContains(const Vector3& point, float epsilon) const;
	bool overlapsOrTouches(const Extents& other) const;
	bool separatedByMoreThan(const Extents& other, float distance) const;
	bool containedByFrustum(const G3D::GCamera::Frustum& frustum) const;

	Extents toWorldSpace(const CoordinateFrame& coordinateFrame);
	Extents express(const CoordinateFrame& from, const CoordinateFrame& to);

	void expand(float distance);
	void unionWith(const Extents& other);

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

// STUB: WEBSERVICE 0x1005a750
inline void Extents::unionWith(const Extents& other)
{
	low = Vector3(G3D::min(other.low.x, low.x), G3D::min(other.low.y, low.y), G3D::min(other.low.z, low.z));
	high = Vector3(G3D::max(other.high.x, high.x), G3D::max(other.high.y, high.y), G3D::max(other.high.z, high.z));
}

inline void Extents::expand(float distance)
{
	low -= Vector3(distance, distance, distance);
	high += Vector3(distance, distance, distance);
}

} // namespace RBX

#endif // UTIL_EXTENTS_H
