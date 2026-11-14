#include "appdraw/HitTest.h"

#include "appdraw/Part.h"

#include <G3D/AABox.h>
#include <G3D/Capsule.h>
#include <G3D/CollisionDetection.h>
#include <G3D/Sphere.h>

namespace RBX {

float HitTest::distanceToHandleRadius(const G3D::Vector3& p_unk0x04, const G3D::Vector3& p_unk0x08)
{
	float distance = (p_unk0x04 - p_unk0x08).magnitude();
	float radius = 0.5f;
	if (distance > 28.66242f) {
		radius = distance * 0.017444445f;
	}
	return radius;
}

bool HitTest::hitTestBall(
	const Part& part, G3D::Ray& rayInPartCoords, G3D::Vector3& hitPointInPartCoords, float gridToReal
)
{
	float radius = part.gridSize.x * gridToReal * 0.5;

	if (G3D::CollisionDetection::collisionTimeForMovingPointFixedSphere(
			rayInPartCoords.origin,
			rayInPartCoords.direction,
			G3D::Sphere(G3D::Vector3::ZERO3, radius),
			hitPointInPartCoords
		) != G3D::inf()) {
		return true;
	}

	return false;
}

bool HitTest::hitTestCylinder(
	const Part& part, G3D::Ray& rayInPartCoords, G3D::Vector3& hitPointInPartCoords, float gridToReal
)
{
	float radius = part.gridSize.z * gridToReal * 0.5;

	return G3D::CollisionDetection::collisionTimeForMovingPointFixedCapsule(
			   rayInPartCoords.origin,
			   rayInPartCoords.direction,
			   G3D::Capsule(G3D::Vector3::ZERO3, G3D::Vector3(part.gridSize.x * gridToReal * 0.5, 0, 0), radius),
			   hitPointInPartCoords
		   ) != G3D::inf();
}

bool HitTest::hitTestBox(
	const Part& part, G3D::Ray& rayInPartCoords, G3D::Vector3& hitPointInPartCoords, float gridToReal
)
{
	G3D::Vector3 halfRealSize = part.gridSize * gridToReal * 0.5;

	return G3D::CollisionDetection::collisionLocationForMovingPointFixedAABox(
		rayInPartCoords.origin, rayInPartCoords.direction, G3D::AABox(-halfRealSize, halfRealSize), hitPointInPartCoords
	);
}

bool HitTest::hitTest(const Part& part, G3D::Ray& rayInPartCoords, G3D::Vector3& hitPointInPartCoords, float gridToReal)
{
	switch (part.type) {
	case Part::BLOCK_PART:
		return hitTestBox(part, rayInPartCoords, hitPointInPartCoords, gridToReal);
	case Part::BALL_PART:
		return hitTestBall(part, rayInPartCoords, hitPointInPartCoords, gridToReal);
	case Part::CYLINDER_PART:
		return hitTestCylinder(part, rayInPartCoords, hitPointInPartCoords, gridToReal);
	}

	return false;
}

} // namespace RBX
