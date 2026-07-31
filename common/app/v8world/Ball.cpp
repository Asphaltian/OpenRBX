#include "v8world/Ball.h"

#include "decomp.h"
#include "util/Math.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x1010a780
Matrix3 Ball::getMomentSolid(float mass) const
{
	Vector3 diagonal;
	diagonal.x = realRadius * realRadius * mass * 0.4f;
	diagonal.y = diagonal.x;
	diagonal.z = diagonal.x;

	return Math::fromDiagonal(diagonal);
}

// FUNCTION: WEBSERVICE 0x1010a7c0
float Ball::getGridVolume() const
{
	float radius = gridSize.x * 0.5f;
	return radius * radius * radius * 4.1887903f;
}

// FUNCTION: WEBSERVICE 0x1010a7e0
void Ball::onSetSize()
{
	realRadius = gridSize.x * 0.5f;
}

// STUB: WEBSERVICE 0x1010a7f0
bool Ball::hitTest(const Ray& ray, Vector3& hitPoint, bool& inside)
{
	STUB(0x1010a7f0);
	return false;
}

} // namespace RBX
