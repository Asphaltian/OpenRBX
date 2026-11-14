#include "humanoid/Seated.h"

#include "humanoid/Jumping.h"
#include "util/Name.h"

namespace RBX {

const char sSeated[] = "Seated";

Humanoid::State* Seated::onStep(const float stepDt, Controller& controller)
{
	if (humanoid->getJump()) {
		humanoid->setJump(false);
		return new Jumping(humanoid);
	}

	return this;
}

Seated::Seated(Humanoid* humanoid) : Named<Humanoid::State, sSeated>(humanoid)
{
	Humanoid::event_Seated.fire(humanoid, true);
}

Seated::~Seated()
{
	Humanoid::event_Seated.fire(humanoid, false);
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sSeated>();
template void RBX::Name::callDoDeclare<RBX::sSeated>();
