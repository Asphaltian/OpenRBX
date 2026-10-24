#include "humanoid/Balancing.h"

#include "util/object.h"
#include "v8datamodel/PartInstance.h"
#include "v8kernel/Body.h"
#include "v8world/Geometry.h"
#include "v8world/Primitive.h"
#include "v8world/SpatialHash.h"
#include "v8world/World.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x10121f90
HitTestFilter::Result Balancing::filterResult(const Primitive* testMe) const
{
	Primitive* torsoPrimitive = humanoid->getTorsoPrimitive();

	Result result = INCLUDE_PRIM;

	// clang-format off
	if (testMe->getDragging() || !testMe->getCanCollide() || (torsoPrimitive != NULL && torsoPrimitive->getAssembly() == testMe->getAssembly())) {
		result = IGNORE_PRIM;
	}
	// clang-format on

	return result;
}

// FUNCTION: WEBSERVICE 0x10121fe0
PartInstance* Balancing::tryFloor(const G3D::Ray& ray, float maxDistance)
{
	float distance;
	Primitive* hit = humanoid->getWorld()
						 ->getContactManager()
						 ->getHitLegacy(ray, NULL, this, floorTouchInWorld, distance, maxDistance);

	return hit ? PartInstance::fromPrimitive(hit) : NULL;
}

// FUNCTION: WEBSERVICE 0x10122030
G3D::Vector3 Balancing::getYAxis() const
{
	Body* body = humanoid->getTorsoBody();

	if (body != NULL) {
		G3D::Vector3 yAxis = body->getPV().position.vectorToObjectSpace(G3D::Vector3::unitY());
		return yAxis;
	}

	return G3D::Vector3::unitY();
}

// STUB: WEBSERVICE 0x10122140
void Balancing::onComputeForce(const float dt)
{
	STUB(0x10122140);
}

// STUB: WEBSERVICE 0x10122520
PartInstance* Balancing::findFloor(bool existingFloor)
{
	if (humanoid->getTorsoBody()) {
		const CoordinateFrame& torsoCoord = humanoid->getTorsoPrimitive()->getCoordinateFrame();
		G3D::Vector3 halfSize = humanoid->getTorsoPrimitive()->getGridSize() * 0.5f;

		float maxDistance = 1.0f;
		const float hysteresis = existingFloor ? 1.5f : 1.1f;

		if (humanoid->getLeftLegPrimitive()) {
			maxDistance = humanoid->getLeftLegPrimitive()->getGridSize().y * hysteresis + 1.0f;
		}
		else if (humanoid->getRightLegPrimitive()) {
			maxDistance = humanoid->getRightLegPrimitive()->getGridSize().y * hysteresis + 1.0f;
		}

		G3D::Vector3 offset(0.0f, -halfSize.y, 0.0f);
		G3D::Vector3 pointOnTorso = torsoCoord.pointToWorldSpace(offset);
		G3D::Ray tryRay = G3D::Ray::fromOriginAndDirection(pointOnTorso, -G3D::Vector3::unitY());

		if (PartInstance* floor = tryFloor(tryRay, maxDistance)) {
			return floor;
		}

		for (int x = -1; x < 2; x += 2) {
			for (int z = -1; z < 2; z += 2) {
				offset.x = x * halfSize.x;
				offset.z = z * halfSize.z;
				pointOnTorso = torsoCoord.pointToWorldSpace(offset);
				tryRay = G3D::Ray::fromOriginAndDirection(pointOnTorso, -G3D::Vector3::unitY());

				if (PartInstance* floor = tryFloor(tryRay, maxDistance)) {
					return floor;
				}
			}
		}
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x10122860
Balancing::Balancing(Humanoid* humanoid, const float kP, const float kD)
	: Humanoid::State(humanoid), kP(kP), kD(kD), floorTouchInWorld(0, 0, 0)
{
}

// FUNCTION: WEBSERVICE 0x101228d0
Humanoid::State* Balancing::onStep(const float stepDt, Controller& controller)
{
	if (humanoid->getTorsoBody() != NULL) {
		floorPart = shared_from(findFloor(floorPart));

		return this;
	}

	return NULL;
}

} // namespace RBX
