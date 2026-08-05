#ifndef V8WORLD_SPATIALHASH_H
#define V8WORLD_SPATIALHASH_H

#include "decomp.h"
#include "util/Extents.h"
#include "util/Vector3int32.h"

#include <G3D/Array.h>
#include <set>
#include <vector>

namespace RBX {

class Contact;
class ContactManager;
class Primitive;
class World;

// SIZE 0x20
class SpatialNode
{
public:
	Primitive* primitive;           // 0x00
	SpatialNode* nextHashLink;      // 0x04
	SpatialNode* nextPrimitiveLink; // 0x08
	SpatialNode* prevPrimitiveLink; // 0x0c
	int hashId;                     // 0x10
	Vector3int32 gridId;            // 0x14
};

DECOMP_SIZE_ASSERT(SpatialNode, 0x20)

// SIZE 0x24
class SpatialHash
{
public:
	SpatialHash(World* world, ContactManager* contactManager);
	~SpatialHash();

	static Vector3int32 realToHashGrid(const Vector3& realPoint);
	static Extents hashGridToRealExtents(const Vector3& hashGrid);

	bool getNextGrid(Vector3int32& grid, const G3D::Ray& unitRay, float maxDistance);

	void getPrimitivesTouchingExtents(const Extents& extents, const Primitive* ignore, G3D::Array<Primitive*>& answer);

	void onPrimitiveAdded(Primitive* p);
	void onPrimitiveRemoved(Primitive* p);
	void onPrimitiveExtentsChanged(Primitive* p);

	void onAllPrimitivesMoved();

	int getNodesOut() const { return nodesOut; }
	int getMaxBucket() const { return maxBucket; }

private:
	static unsigned int numBits() { return 16; }
	static unsigned int numBuckets() { return 1 << numBits(); }

	static float hashGridSize() { return 8.0f; }
	static float hashGridRecip() { return 1.0f / hashGridSize(); }

	static int getHash(const Vector3int32& grid);

	SpatialNode* findNode(Primitive* p, const Vector3int32& grid);
	SpatialNode* newNode();
	void insertNodeToPrimitive(SpatialNode* node, Primitive* p, const Vector3int32& grid, int hash);
	void addNode(Primitive* p, const Vector3int32& grid);
	void getPrimitivesInGrid(const Vector3int32& grid, G3D::Array<Primitive*>& found);

	void removeNodeFromPrimitive(SpatialNode* remove);
	void removeNodeFromHash(SpatialNode* remove);
	void returnNode(SpatialNode* node);
	void destroyNode(SpatialNode* destroy);
	bool shareCommonGrid(Primitive* me, Primitive* other);
	static void computeMinMax(const Extents& extents, Vector3int32& min, Vector3int32& max);

	void changeMinMax(Primitive* p, const Extents& change, const Extents& oldBox, const Extents& newBox);
	void primitiveExtentsChanged(Primitive* p);

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
	ContactManager(World* world);
	~ContactManager();

	void getPrimitivesTouchingExtents(const Extents& extents, const Primitive* ignore, G3D::Array<Primitive*>& found);

	void onPrimitiveAdded(Primitive* primitive);
	void onPrimitiveRemoved(Primitive* p);
	void onPrimitiveExtentsChanged(Primitive* p);
	void onPrimitiveGeometryTypeChanged(Primitive* p);

	void onNewPair(Primitive* p0, Primitive* p1);
	void onReleasePair(Primitive* p0, Primitive* p1);

	bool intersectingOthers(Primitive* check, const std::set<Primitive*>& checkSet, float overlapIgnored);
	bool intersectingOthers(const G3D::Array<Primitive*>& check, float overlapIgnored);
	bool intersectingOthers(Primitive* check, float overlapIgnored);

	void stepWorld();

	SpatialHash* getSpatialHash() const { return spatialHash; }

private:
	Contact* createContact(Primitive* p0, Primitive* p1);

	SpatialHash* spatialHash; // 0x00
	World* world;             // 0x04
};

DECOMP_SIZE_ASSERT(ContactManager, 0x08)

} // namespace RBX

#endif // V8WORLD_SPATIALHASH_H
