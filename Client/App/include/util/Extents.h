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
private:
	Vector3 low;  // 0x00
	Vector3 high; // 0x0c

	static float epsilon();

public:
	Extents() : low(Vector3::inf()), high(-Vector3::inf()) {}
	Extents(const Vector3& low, const Vector3& high) : low(low), high(high) {}

	Extents(const Vector3int32& low, const Vector3int32& high)
		: low((float) low.x, (float) low.y, (float) low.z), high((float) high.x, (float) high.y, (float) high.z)
	{
	}

	static Extents fromCenterCorner(const Vector3& center, const Vector3& corner);
	static Extents fromCenterRadius(const Vector3&, float);

	bool operator==(const Extents&) const;
	bool operator!=(const Extents&) const;

	Extents& operator=(const Extents& other)
	{
		low = other.low;
		high = other.high;
		return *this;
	}

	static Extents vv(const Vector3& v0, const Vector3& v1);

	const Vector3& min() const { return low; }
	const Vector3& max() const { return high; }

	Vector3 getCorner(int index) const;

	Vector3 size() const { return high - low; }

	Vector3 center() const { return (high + low) * 0.5f; }

	Vector3 bottomCenter() const
	{
		Vector3 answer = center();
		answer.y = low.y;

		return answer;
	}

	Vector3 topCenter() const
	{
		Vector3 answer = center();
		answer.y = high.y;

		return answer;
	}

	float longestSide() const;
	float volume() const;
	float areaXZ() const;

	Extents toWorldSpace(const CoordinateFrame& coordinateFrame);
	Extents express(const CoordinateFrame& from, const CoordinateFrame& to);

	Vector3 faceCenter(NormalId normalId) const;
	void getFaceCorners(NormalId normalId, Vector3& c0, Vector3& c1, Vector3& c2, Vector3& c3) const;
	Plane getPlane(NormalId normalId) const;

	Vector3 clip(const Vector3& point) const { return point.clamp(low, high); }

	Vector3 clamp(const Extents&) const;
	NormalId closestFace(const Vector3& point);

	void unionWith(const Extents& other);
	void shift(const Vector3&);
	void scale(float value);
	void expand(float distance);

	const Vector3& operator[](int) const;
	Vector3& operator[](int);
	operator Vector3*();
	operator const Vector3*() const;

	bool contains(const Vector3& point) const;
	bool overlapsOrTouches(const Extents& other) const;
	static bool overlapsOrTouches(const Extents&, const Extents&);
	bool fuzzyContains(const Vector3& point, float epsilon) const;
	bool containedByFrustum(const G3D::GCamera::Frustum& frustum) const;
	bool partiallyContainedByFrustum(const G3D::GCamera::Frustum&) const;
	bool separatedByMoreThan(const Extents& other, float distance) const;

	static const Extents& zero();
	static const Extents& unit();
	static const Extents& infiniteExtents();
	static const Extents& negativeInfiniteExtents();
};

DECOMP_SIZE_ASSERT(Extents, 0x18)

inline Extents Extents::fromCenterCorner(const Vector3& center, const Vector3& corner)
{
	return Extents(center - corner, center + corner);
}

// FUNCTION: WEBSERVICE 0x1005a750
// FUNCTION: ROBLOX 0x005654c0
inline void Extents::unionWith(const Extents& other)
{
	low = low.min(other.low);
	high = high.max(other.high);
}

inline void Extents::scale(float value)
{
	low *= value;
	high *= value;
}

inline void Extents::expand(float distance)
{
	low -= Vector3(distance, distance, distance);
	high += Vector3(distance, distance, distance);
}

} // namespace RBX

#endif // UTIL_EXTENTS_H
