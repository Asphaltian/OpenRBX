#include "humanoid/Freefall.h"

#include "humanoid/Running.h"
#include "util/Name.h"

namespace RBX {

const char sFreefall[] = "Freefall";

Humanoid::State* Freefall::onStep(const float stepDt, Controller& controller)
{
	State* answer = Flying::onStep(stepDt, controller);

	if (answer != this) {
		return answer;
	}

	if (floorPart) {
		return new Running(humanoid);
	}

	return this;
}

Freefall::Freefall(Humanoid* humanoid) : Named<Flying, sFreefall>(humanoid)
{
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sFreefall>();
template void RBX::Name::callDoDeclare<RBX::sFreefall>();
