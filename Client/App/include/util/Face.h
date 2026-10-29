#ifndef UTIL_FACE_H
#define UTIL_FACE_H

#include "decomp.h"
#include "util/Extents.h"
#include "util/NormalId.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/Vector2.h>
#include <G3D/Vector3.h>

namespace RBX {

using G3D::CoordinateFrame;
using G3D::Vector2;
using G3D::Vector3;

// SIZE 0x30
class Face
{
private:
	Vector3 c0; // 0x00
	Vector3 c1; // 0x0c
	Vector3 c2; // 0x18
	Vector3 c3; // 0x24

	Face(const Vector3& c0, const Vector3& c1, const Vector3& c2, const Vector3& c3) : c0(c0), c1(c1), c2(c2), c3(c3) {}

public:
	Face() {}
	Face(const Face& other) : c0(other.c0), c1(other.c1), c2(other.c2), c3(other.c3) {}

private:
	// STUB: WEBSERVICE 0x1010b320
	// STUB: ROBLOX 0x0064a960
	Vector3 getAxis(int i) const { return i == 0 ? (c1 - c0).direction() : (c3 - c0).direction(); }

	void minMax(const Vector3& point, const Vector3& normal, float& min, float& max) const;

	Face operator*(const Vector3& scale) const;
	Face operator*(float scale) const;

public:
	static Face fromExtentsSide(const Extents& e, NormalId faceId);

	void snapToGrid(float grid);

	const Vector3& operator[](int i) const { return (&c0)[i]; }
	Vector3& operator[](int i);

	Vector3 getU() const { return c1 - c0; }
	Vector3 getV() const { return c3 - c0; }
	Vector3 getNormal() const;

	Vector2 size() const { return Vector2((c1 - c0).magnitude(), (c3 - c0).magnitude()); }

	Vector3 center() const { return (c2 + c0) * 0.5f; }

	Face toWorldSpace(const CoordinateFrame& objectCoord) const;
	Face toObjectSpace(const CoordinateFrame& objectCoord) const;

	Face projectOverlapOnMe(const Face& other) const;

	bool fuzzyContainsInExtrusion(const Vector3& point, float tolerance) const;

	static bool cornersAligned(const Face& f0, const Face& f1, float tolerance);
	static bool hasOverlap(const Face& f0, const Face& f1, float byAtLeast);
	static bool overlapWithinPlanes(const Face& f0, const Face& f1, float tolerance);
};

DECOMP_SIZE_ASSERT(Face, 0x30)

} // namespace RBX

#endif // UTIL_FACE_H
