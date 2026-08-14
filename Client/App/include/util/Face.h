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
public:
	static Face fromExtentsSide(const Extents& extents, NormalId normalId);

	Vector3 getU() const { return c1 - c0; }
	Vector3 getV() const { return c3 - c0; }

	Vector2 size() const { return Vector2((c1 - c0).magnitude(), (c3 - c0).magnitude()); }

	Vector3 center() const { return (c2 + c0) * 0.5f; }

	void snapToGrid(float grid);

	Face toWorldSpace(const CoordinateFrame& coord) const;
	Face toObjectSpace(const CoordinateFrame& coord) const;

	Face projectOverlapOnMe(const Face& other) const;

	bool fuzzyContainsInExtrusion(const Vector3& point, float tolerance) const;

	static bool cornersAligned(const Face& f0, const Face& f1, float tolerance);
	static bool hasOverlap(const Face& f0, const Face& f1, float tolerance);
	static bool overlapWithinPlanes(const Face& f0, const Face& f1, float tolerance);

	Vector3& operator[](int index);

private:
	Vector3 c0; // 0x00
	Vector3 c1; // 0x0c
	Vector3 c2; // 0x18
	Vector3 c3; // 0x24
};

DECOMP_SIZE_ASSERT(Face, 0x30)

} // namespace RBX

#endif // UTIL_FACE_H
