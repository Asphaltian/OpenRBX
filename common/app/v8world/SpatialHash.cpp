#include "v8world/SpatialHash.h"

#include "decomp.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x101233c0
Vector3int32 SpatialHash::realToHashGrid(const Vector3& realPoint)
{
	Vector3 gridPoint = realPoint * hashGridRecip();

	return Vector3int32::floor(gridPoint);
}

// FUNCTION: WEBSERVICE 0x10123410
Extents SpatialHash::hashGridToRealExtents(const Vector3& hashGrid)
{
	return Extents(
		hashGrid * hashGridSize(),
		Vector3(hashGrid.x + 1.0f, hashGrid.y + 1.0f, hashGrid.z + 1.0f) * hashGridSize()
	);
}

// STUB: WEBSERVICE 0x10123990
void SpatialHash::destroyNode(SpatialNode* node)
{
	STUB(0x10123990);
}

// STUB: WEBSERVICE 0x10123bd0
void SpatialHash::onPrimitiveAdded(Primitive* primitive)
{
	STUB(0x10123bd0);
}

// STUB: WEBSERVICE 0x10123e20
void SpatialHash::primitiveExtentsChanged(Primitive* primitive)
{
	STUB(0x10123e20);
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
