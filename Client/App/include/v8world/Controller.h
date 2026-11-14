#ifndef V8WORLD_CONTROLLER_H
#define V8WORLD_CONTROLLER_H

#include "placeholder.h"

#include <G3D/Color3.h>
#include <G3D/ReferenceCount.h>

namespace RBX {

using G3D::Color3;

class __declspec(novtable) Controller : public G3D::ReferenceCountedObject
{
public:
	Controller() {}

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

	virtual ~Controller() {}

	virtual float getValue(InputType inputType) const;
	virtual ControllerType getControllerType() const = 0;
	virtual bool hasIntelligence() const = 0;
	virtual bool isUserController() const = 0;

	bool isNullController();

	static bool isControllableInput(InputType inputType);
	static Color3 controllerTypeToColor(ControllerType controllerType);
};

class NullController : public Controller
{
public:
	virtual ControllerType getControllerType() const;
	virtual bool hasIntelligence() const;
	virtual bool isUserController() const;

	static NullController* getStaticNullController();
};

class ControllerTypeArray
{
private:
	bool values[Controller::NUM_CONTROLLER_TYPES];

public:
	ControllerTypeArray() { clear(); }

	void clear()
	{
		for (int i = 0; i < Controller::NUM_CONTROLLER_TYPES; ++i) {
			values[i] = false;
		}
	}

	void setController(Controller::ControllerType controllerType, bool value) { values[controllerType] = value; }
	bool hasController(Controller::ControllerType controllerType) const { return values[controllerType]; }

	ControllerTypeArray& operator|=(const ControllerTypeArray& other);
};

inline NullController* NullController::getStaticNullController()
{
	static NullController n;
	return &n;
}

} // namespace RBX

#endif // V8WORLD_CONTROLLER_H
