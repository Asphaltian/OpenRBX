#ifndef V8WORLD_SPATIALHASH_H
#define V8WORLD_SPATIALHASH_H

#include "decomp.h"
#include "util/Extents.h"

#include <G3D/Array.h>

namespace RBX {

class ContactManager;
class Primitive;
class World;

// SIZE 0x24
class SpatialHash
{
public:
	void getPrimitivesTouchingExtents(const Extents& extents, const Primitive* ignore, G3D::Array<Primitive*>& found);

	void onPrimitiveAdded(Primitive* primitive);
	void onPrimitiveRemoved(Primitive* primitive);
	void onPrimitiveExtentsChanged(Primitive* primitive);

	void onAllPrimitivesMoved();

	int getNumHashNodes() const { return nodesOut; }
	int getMaxBucketSize() const { return maxBucket; }

private:
	World* world;                     // 0x00
	ContactManager* contactManager;   // 0x04
	undefined m_unk0x08[0x1c - 0x08]; // 0x08
	int nodesOut;                     // 0x1c
	int maxBucket;                    // 0x20
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
