#include "humanoid/Freefall.h"

#include "util/Name.h"

namespace RBX {

const char sFreefall[] = "Freefall";

// STUB: WEBSERVICE 0x1012a530
Humanoid::State* Freefall::onStep(const float stepDt, Controller& controller)
{
	STUB(0x1012a530);
	return NULL;
}

// FUNCTION: WEBSERVICE 0x1012a670
Freefall::Freefall(Humanoid* humanoid) : Named<Flying, sFreefall>(humanoid)
{
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sFreefall>();
template void RBX::Name::callDoDeclare<RBX::sFreefall>();
