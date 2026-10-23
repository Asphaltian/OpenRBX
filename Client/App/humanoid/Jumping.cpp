#include "humanoid/Jumping.h"

#include "humanoid/Freefall.h"
#include "util/Name.h"
#include "v8world/World.h"

namespace RBX {

const char sJumping[] = "Jumping";

const float Jumping::kJumpP = 5000.0f;
const float Jumping::jumpVelocityGrid = 50.0f;

// FUNCTION: WEBSERVICE 0x10122be0
Humanoid::State* Jumping::onStep(const float stepDt, Controller& controller)
{
	if (done) {
		return new Freefall(humanoid);
	}

	State* answer = Flying::onStep(stepDt, controller);
	if (answer != this) {
		return answer;
	}

	if (floorPart) {
		humanoid->getWorld()->ticklePrimitive(floorPart->getPrimitive(), false);
		return this;
	}

	return new Freefall(humanoid);
}

// STUB: WEBSERVICE 0x10122cd0
void Jumping::onComputeForce(const float dt)
{
	STUB(0x10122cd0);
}

// FUNCTION: WEBSERVICE 0x10122ec0
Jumping::Jumping(Humanoid* humanoid) : Named<Flying, sJumping>(humanoid), done(false), timeout(0.5f)
{
	Humanoid::event_Jumping.fire(humanoid, true);
}

// FUNCTION: WEBSERVICE 0x10122f50
Jumping::~Jumping()
{
	Humanoid::event_Jumping.fire(humanoid, false);
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sJumping>();
template void RBX::Name::callDoDeclare<RBX::sJumping>();
