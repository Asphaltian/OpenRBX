#include "humanoid/FallingDown.h"

#include "humanoid/GettingUp.h"
#include "util/Name.h"

namespace RBX {

const char sFallingDown[] = "FallingDown";

Humanoid::State* FallingDown::onStep(const float stepDt, Controller& controller)
{
	time += stepDt;

	if (time > timeToGetUp) {
		return new GettingUp(humanoid);
	}

	return this;
}

FallingDown::FallingDown(Humanoid* humanoid, float timeToGetUp)
	: Named<Humanoid::State, sFallingDown>(humanoid), time(0.0f), timeToGetUp(timeToGetUp)
{
	Humanoid::event_FallingDown.fire(humanoid, true);
}

FallingDown::~FallingDown()
{
	Humanoid::event_FallingDown.fire(humanoid, false);
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sFallingDown>();
template void RBX::Name::callDoDeclare<RBX::sFallingDown>();
