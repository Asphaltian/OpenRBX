#include "util/Face.h"

#include "decomp.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x1010ae60
Vector3& Face::operator[](int index)
{
	return (&c0)[index];
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

} // namespace RBX
