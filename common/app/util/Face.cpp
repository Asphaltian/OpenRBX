#include "util/Face.h"

#include "decomp.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x1010ae60
Vector3& Face::operator[](int index)
{
	return (&c0)[index];
}

// STUB: WEBSERVICE 0x1010ae70
DECOMP_NOINLINE void Face::snapToGrid(float grid)
{
	STUB(0x1010ae70);
}

// STUB: WEBSERVICE 0x1010aec0
DECOMP_NOINLINE bool Face::fuzzyContainsInExtrusion(const Vector3& point, float tolerance) const
{
	STUB(0x1010aec0);
	return false;
}

// STUB: WEBSERVICE 0x1010af40
DECOMP_NOINLINE bool Face::cornersAligned(const Face& f0, const Face& f1, float tolerance)
{
	STUB(0x1010af40);
	return false;
}

// STUB: WEBSERVICE 0x1010afb0
Face Face::fromExtentsSide(const Extents& extents, NormalId normalId)
{
	STUB(0x1010afb0);
	return Face();
}

// STUB: WEBSERVICE 0x1010b000
Face Face::toWorldSpace(const CoordinateFrame& coord) const
{
	STUB(0x1010b000);
	return Face();
}

// STUB: WEBSERVICE 0x1010b180
Face Face::toObjectSpace(const CoordinateFrame& coord) const
{
	STUB(0x1010b180);
	return Face();
}

// STUB: WEBSERVICE 0x1010b520
DECOMP_NOINLINE bool Face::hasOverlap(const Face& f0, const Face& f1, float tolerance)
{
	STUB(0x1010b520);
	return false;
}

// STUB: WEBSERVICE 0x1010b5b0
Face Face::projectOverlapOnMe(const Face& other) const
{
	STUB(0x1010b5b0);
	return Face();
}

// STUB: WEBSERVICE 0x1010b890
DECOMP_NOINLINE bool Face::overlapWithinPlanes(const Face& f0, const Face& f1, float tolerance)
{
	STUB(0x1010b890);
	return false;
}

} // namespace RBX
