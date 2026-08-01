#ifndef V8WORLD_BLOCK_H
#define V8WORLD_BLOCK_H

#include "decomp.h"
#include "v8world/Geometry.h"

#include <cstddef>

namespace RBX {

// SIZE 0x60
class BlockTemplate
{
public:
	static const Vector3* getVertices(const Vector3& corner);

private:
	Vector3 vertices[8]; // 0x00
};

DECOMP_SIZE_ASSERT(BlockTemplate, 0x60)

// VTABLE: WEBSERVICE 0x10237654
// SIZE 0x18
class Block : public Geometry
{
public:
	Block() : vertices(NULL), cornerRadius(0) {}

	// SYNTHETIC: WEBSERVICE 0x100a76b0 FOLDED
	// RBX::Block::`scalar deleting destructor'

	virtual void onSetSize();
	virtual bool hitTest(const Ray& ray, Vector3& hitPoint, bool& inside);

	// FUNCTION: WEBSERVICE 0x100a7740 FOLDED
	virtual GeometryType getGeometryType() const { return GEOMETRY_BLOCK; }

	// FUNCTION: WEBSERVICE 0x100a7750
	virtual float getRadius() const { return cornerRadius; }

	// FUNCTION: WEBSERVICE 0x100a7760
	virtual Matrix3 getMoment(float mass) const { return getMomentHollow(mass); }

	virtual Vector3 getCenterToCorner(const Matrix3& rotation) const;
	virtual float getGridVolume() const;

	Matrix3 getMomentHollow(float mass) const;

private:
	const Vector3* vertices; // 0x10
	float cornerRadius;      // 0x14
};

DECOMP_SIZE_ASSERT(Block, 0x18)

} // namespace RBX

#endif // V8WORLD_BLOCK_H
