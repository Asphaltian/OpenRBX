#ifndef V8WORLD_SPATIALHASH_H
#define V8WORLD_SPATIALHASH_H

#include "decomp.h"
#include "util/Extents.h"
#include "util/Vector3int32.h"

#include <G3D/Array.h>
#include <vector>

namespace RBX {

class ContactManager;
class Primitive;
class SpatialNode;
class World;

// SIZE 0x24
class SpatialHash
{
public:
	static Vector3int32 realToHashGrid(const Vector3& realPoint);
	static Extents hashGridToRealExtents(const Vector3& hashGrid);

	void getPrimitivesTouchingExtents(const Extents& extents, const Primitive* ignore, G3D::Array<Primitive*>& found);

	void onPrimitiveAdded(Primitive* primitive);
	void onPrimitiveRemoved(Primitive* primitive);
	void onPrimitiveExtentsChanged(Primitive* primitive);

	void onAllPrimitivesMoved();

	int getNodesOut() const { return nodesOut; }
	int getMaxBucket() const { return maxBucket; }

private:
	static float hashGridSize() { return 8.0f; }
	static float hashGridRecip() { return 1.0f / hashGridSize(); }

	void destroyNode(SpatialNode* node);
	void primitiveExtentsChanged(Primitive* primitive);

	World* world;                    // 0x00
	ContactManager* contactManager;  // 0x04
	std::vector<SpatialNode*> nodes; // 0x08
	SpatialNode* extraNodes;         // 0x18
	int nodesOut;                    // 0x1c
	int maxBucket;                   // 0x20
};

DECOMP_SIZE_ASSERT(SpatialHash, 0x24)

// SIZE 0x08
class ContactManager
{
public:
	void getPrimitivesTouchingExtents(const Extents& extents, const Primitive* ignore, G3D::Array<Primitive*>& found);

	void onPrimitiveAdded(Primitive* primitive);
	void onPrimitiveRemoved(Primitive* primitive);
	void onPrimitiveExtentsChanged(Primitive* primitive);

	void stepWorld();

	SpatialHash* getSpatialHash() const { return spatialHash; }

private:
	SpatialHash* spatialHash; // 0x00
	World* world;             // 0x04
};

DECOMP_SIZE_ASSERT(ContactManager, 0x08)

} // namespace RBX

#endif // V8WORLD_SPATIALHASH_H
