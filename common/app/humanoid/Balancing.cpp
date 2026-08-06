#include "humanoid/Balancing.h"

#include "util/object.h"
#include "v8datamodel/PartInstance.h"
#include "v8world/Primitive.h"

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

// STUB: WEBSERVICE 0x10122140
void Balancing::onComputeForce(const float stepDt)
{
	STUB(0x10122140);
}

// STUB: WEBSERVICE 0x10122520
DECOMP_NOINLINE PartInstance* Balancing::findFloor(bool hadFloor)
{
	STUB(0x10122520);
	return NULL;
}

// FUNCTION: WEBSERVICE 0x10122860
Balancing::Balancing(Humanoid* humanoid, const float kP, const float kD)
	: Humanoid::State(humanoid), kP(kP), kD(kD), floorTouchInWorld(0, 0, 0)
{
}

// STUB: WEBSERVICE 0x101228d0
Humanoid::State* Balancing::onStep(const float stepDt, Controller& controller)
{
	if (humanoid->getTorsoBody() != NULL) {
		floorPart = shared_from(findFloor(floorPart));

		return this;
	}

	return NULL;
}

DECOMP_NOINLINE G3D::Vector3 Balancing::getYAxis() const
{
	return G3D::Vector3(0, 0, 0);
}

} // namespace RBX
