#include "v8world/SpatialHash.h"

#include "decomp.h"
#include "v8world/Assembly2.h"
#include "v8world/Primitive.h"
#include "v8world/World.h"

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

void SpatialHash::removeNodeFromPrimitive(SpatialNode* remove)
{
	SpatialNode* next = remove->nextPrimitiveLink;
	SpatialNode* prev = remove->prevPrimitiveLink;

	if (next != NULL) {
		next->prevPrimitiveLink = prev;
	}

	if (prev != NULL) {
		prev->nextPrimitiveLink = next;
	}
	else {
		remove->primitive->spatialNodes = next;
	}
}

// FUNCTION: WEBSERVICE 0x10123840
void SpatialHash::removeNodeFromHash(SpatialNode* remove)
{
	SpatialNode** link = &nodes[remove->hashId];

	while (*link != remove) {
		link = &(*link)->nextHashLink;
	}
	*link = remove->nextHashLink;
}

int SpatialHash::getHash(const Vector3int32& grid)
{
	return (grid.x * -2979 ^ grid.y * 16543 ^ grid.z * -73) & (numBuckets() - 1);
}

// FUNCTION: WEBSERVICE 0x10123890
SpatialNode* SpatialHash::findNode(Primitive* p, const Vector3int32& grid)
{
	int hash = getHash(grid);

	SpatialNode* node = nodes[hash];

	while (node->primitive != p || node->gridId != grid) {
		node = node->nextHashLink;
	}

	return node;
}

// FUNCTION: WEBSERVICE 0x10123910
bool SpatialHash::shareCommonGrid(Primitive* me, Primitive* other)
{
	for (SpatialNode* node = me->spatialNodes; node != NULL; node = node->nextPrimitiveLink) {
		for (SpatialNode* hashNode = nodes[node->hashId]; hashNode != NULL; hashNode = hashNode->nextHashLink) {
			if (hashNode->primitive == other && hashNode->gridId == node->gridId) {
				return true;
			}
		}
	}

	return false;
}

void SpatialHash::returnNode(SpatialNode* node)
{
	node->nextHashLink = extraNodes;
	nodesOut--;
	extraNodes = node;
}

// FUNCTION: WEBSERVICE 0x10123990
void SpatialHash::destroyNode(SpatialNode* destroy)
{
	removeNodeFromPrimitive(destroy);

	removeNodeFromHash(destroy);

	Vector3int32 destroyGrid = destroy->gridId;

	SpatialNode* node = nodes[destroy->hashId];

	while (node != NULL) {
		if (node->gridId == destroyGrid) {
			Primitive* me = destroy->primitive;
			Primitive* other = node->primitive;

			if (Primitive::getContact(me, other) != NULL) {
				if (!shareCommonGrid(me, other)) {
					contactManager->onReleasePair(me, other);
				}
			}
		}

		node = node->nextHashLink;
	}

	returnNode(destroy);
}

// STUB: WEBSERVICE 0x10123bd0
void SpatialHash::onPrimitiveAdded(Primitive* primitive)
{
	STUB(0x10123bd0);
}

// STUB: WEBSERVICE 0x10123e20
DECOMP_NOINLINE void SpatialHash::primitiveExtentsChanged(Primitive* primitive)
{
	STUB(0x10123e20);
}

// FUNCTION: WEBSERVICE 0x10123fb0
void SpatialHash::onPrimitiveRemoved(Primitive* p)
{
	while (p->spatialNodes != NULL) {
		destroyNode(p->spatialNodes);
	}
}

// STUB: WEBSERVICE 0x10123fe0
void SpatialHash::onPrimitiveExtentsChanged(Primitive* primitive)
{
	STUB(0x10123fe0);
}

// STUB: WEBSERVICE 0x10123ff0
void SpatialHash::onAllPrimitivesMoved()
{
	const World::PrimitiveArray& primitives = world->getPrimitives();

	for (int i = 0; i < primitives.size(); i++) {
		Primitive* p = primitives[i];

		if (p->getAssembly()->getSleepStatus() == Sim::AWAKE) {
			primitiveExtentsChanged(p);
		}
	}
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
