#ifndef V8WORLD_CONTROLLER_H
#define V8WORLD_CONTROLLER_H

#include <G3D/Color3.h>

namespace RBX {

using G3D::Color3;

class Controller
{
public:
	enum ControllerType
	{
		NO_CONTROLLER = 0,
		PRIMARY_CONTROLLER = 1,
		SECONDARY_CONTROLLER = 2,
		PAD_ONE_CONTROLLER = 3,
		PAD_TWO_CONTROLLER = 4,
		AI_CHASE_CONTROLLER = 5,
		AI_FLEE_CONTROLLER = 6,
		PLAYER_CONTROLLER = 7,
		NUM_CONTROLLER_TYPES = 8,
	};

	static Color3 controllerTypeToColor(ControllerType controllerType);
};

} // namespace RBX

#endif // V8WORLD_CONTROLLER_H
