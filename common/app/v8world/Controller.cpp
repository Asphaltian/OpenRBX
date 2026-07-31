#include "v8world/Controller.h"

#include "decomp.h"

#include <cstddef>

namespace RBX {

// STUB: WEBSERVICE 0x10099400
NullController* NullController::getStaticNullController()
{
	STUB(0x10099400);
	return NULL;
}

// FUNCTION: WEBSERVICE 0x101004d0
Color3 Controller::controllerTypeToColor(ControllerType controllerType)
{
	switch (controllerType) {
	case PRIMARY_CONTROLLER:
		return Color3::blue();
	case SECONDARY_CONTROLLER:
		return Color3::red();
	case AI_CHASE_CONTROLLER:
		return Color3::black();
	case AI_FLEE_CONTROLLER:
		return Color3::yellow();
	case PLAYER_CONTROLLER:
		return Color3::purple();
	default:
		return Color3::gray();
	}
}

} // namespace RBX
