#include "v8world/SpatialHash.h"

#include "decomp.h"

namespace RBX {

// STUB: WEBSERVICE 0x10123bd0
void SpatialHash::onPrimitiveAdded(Primitive* primitive)
{
	STUB(0x10123bd0);
}

// STUB: WEBSERVICE 0x10123fb0
void SpatialHash::onPrimitiveRemoved(Primitive* primitive)
{
	STUB(0x10123fb0);
}

// STUB: WEBSERVICE 0x10123fe0
void SpatialHash::onPrimitiveExtentsChanged(Primitive* primitive)
{
	STUB(0x10123fe0);
}

// STUB: WEBSERVICE 0x10123ff0
void SpatialHash::onAllPrimitivesMoved()
{
	STUB(0x10123ff0);
}

// STUB: WEBSERVICE 0x10124100
void SpatialHash::getPrimitivesTouchingExtents(
	const Extents& extents,
	const Primitive* ignore,
	G3D::Array<Primitive*>& found
)
{
	STUB(0x10124100);
}

} // namespace RBX
