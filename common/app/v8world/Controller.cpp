#include "v8world/Controller.h"

namespace RBX {

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
