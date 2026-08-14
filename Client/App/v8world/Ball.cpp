#include "v8world/Ball.h"

#include "decomp.h"
#include "util/Math.h"

#include <G3D/CollisionDetection.h>
#include <G3D/Sphere.h>

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

// FUNCTION: WEBSERVICE 0x1010a7f0
bool Ball::hitTest(const Ray& rayInMe, Vector3& localHitPoint, bool& inside)
{
	float distanceSquared = rayInMe.origin.squaredMagnitude();

	inside = distanceSquared <= realRadius * realRadius;

	G3D::Sphere sphere(Vector3::zero(), realRadius);

	return G3D::CollisionDetection::collisionTimeForMovingPointFixedSphere(
			   rayInMe.origin,
			   rayInMe.direction,
			   sphere,
			   localHitPoint
		   ) != G3D::inf();
}

} // namespace RBX
