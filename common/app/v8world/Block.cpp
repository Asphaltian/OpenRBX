#include "v8world/Block.h"

#include "decomp.h"

#include <cstddef>

namespace RBX {

// STUB: WEBSERVICE 0x101090b0
Matrix3 Block::getMomentHollow(float mass) const
{
	STUB(0x101090b0);
	return Matrix3::zero();
}

// FUNCTION: WEBSERVICE 0x101091f0
float Block::getGridVolume() const
{
	return gridSize.z * gridSize.y * gridSize.x;
}

// STUB: WEBSERVICE 0x10109840
bool Block::hitTest(const Ray& ray, Vector3& hitPoint, bool& inside)
{
	STUB(0x10109840);
	return false;
}

// STUB: WEBSERVICE 0x10109a70
Vector3 Block::getCenterToCorner(const Matrix3& rotation) const
{
	STUB(0x10109a70);
	return Vector3::zero();
}

// STUB: WEBSERVICE 0x1010a5e0
const Vector3* BlockTemplate::getVertices(const Vector3& corner)
{
	STUB(0x1010a5e0);
	return NULL;
}

// STUB: WEBSERVICE 0x1010a680
void Block::onSetSize()
{
	vertices = BlockTemplate::getVertices(gridSize * -0.5f);
	cornerRadius = vertices[0].magnitude();
}

} // namespace RBX
