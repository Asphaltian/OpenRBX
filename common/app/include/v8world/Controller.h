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

	enum InputType
	{
		NO_INPUT = 0,
		LEFT_TRACK_INPUT = 1,
		RIGHT_TRACK_INPUT = 2,
		RIGHT_LEFT_INPUT = 3,
		BACK_FORWARD_INPUT = 4,
		STRAFE_INPUT = 5,
		UP_DOWN_INPUT = 6,
		BUTTON_1_INPUT = 7,
		BUTTON_2_INPUT = 8,
		BUTTON_3_INPUT = 9,
		BUTTON_4_INPUT = 10,
		BUTTON_3_4_INPUT = 11,
		CONSTANT_INPUT = 12,
		SIN_INPUT = 13,
		NUM_INPUT_TYPES = 14,
	};

	static Color3 controllerTypeToColor(ControllerType controllerType);
};

class NullController : public Controller
{
public:
	static NullController* getStaticNullController();
};

} // namespace RBX

#endif // V8WORLD_CONTROLLER_H
