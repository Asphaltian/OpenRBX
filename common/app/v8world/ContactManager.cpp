#include "decomp.h"
#include "v8world/SpatialHash.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x10108130
void ContactManager::getPrimitivesTouchingExtents(
	const Extents& extents,
	const Primitive* ignore,
	G3D::Array<Primitive*>& found
)
{
	spatialHash->getPrimitivesTouchingExtents(extents, ignore, found);
}

// STUB: WEBSERVICE 0x10108140
DECOMP_NOINLINE void ContactManager::onReleasePair(Primitive* p0, Primitive* p1)
{
	STUB(0x10108140);
}

// FUNCTION: WEBSERVICE 0x10108170
void ContactManager::onPrimitiveAdded(Primitive* primitive)
{
	spatialHash->onPrimitiveAdded(primitive);
}

// FUNCTION: WEBSERVICE 0x10108180
void ContactManager::onPrimitiveRemoved(Primitive* primitive)
{
	spatialHash->onPrimitiveRemoved(primitive);
}

// FUNCTION: WEBSERVICE 0x10108190
void ContactManager::onPrimitiveExtentsChanged(Primitive* primitive)
{
	spatialHash->onPrimitiveExtentsChanged(primitive);
}

// FUNCTION: WEBSERVICE 0x101081a0
void ContactManager::stepWorld()
{
	spatialHash->onAllPrimitivesMoved();
}

} // namespace RBX
