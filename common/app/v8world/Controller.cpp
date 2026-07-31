#include "v8world/Controller.h"

#include "decomp.h"

#include <cstddef>

namespace RBX {

// FUNCTION: WEBSERVICE 0x10059d20 FOLDED
Controller::ControllerType NullController::getControllerType() const
{
	return NO_CONTROLLER;
}

// FUNCTION: WEBSERVICE 0x10099070 FOLDED
float Controller::getValue(InputType inputType) const
{
	return 0;
}

// SYNTHETIC: WEBSERVICE 0x10099080
// RBX::NullController::`scalar deleting destructor'

// SYNTHETIC: WEBSERVICE 0x10222ff0
// `RBX::NullController::getStaticNullController'::`2'::`dynamic atexit destructor for 'n''

bool NullController::hasIntelligence() const
{
	return false;
}

// FUNCTION: WEBSERVICE 0x100eb790 FOLDED
bool NullController::isUserController() const
{
	return false;
}

// FUNCTION: WEBSERVICE 0x101004d0
Color3 Controller::controllerTypeToColor(ControllerType controllerType)
{
	switch (controllerType) {
	case PLAYER_CONTROLLER:
		return Color3::purple();
	case PRIMARY_CONTROLLER:
		return Color3::blue();
	case SECONDARY_CONTROLLER:
		return Color3::red();
	case AI_CHASE_CONTROLLER:
		return Color3::black();
	case AI_FLEE_CONTROLLER:
		return Color3::yellow();
	default:
		return Color3::gray();
	}
}

} // namespace RBX
