#ifndef V8WORLD_CONTROLLER_H
#define V8WORLD_CONTROLLER_H

#include "decomp.h"

#include <G3D/Color3.h>
#include <G3D/ReferenceCount.h>

namespace RBX {

using G3D::Color3;

// SIZE 0x0c
class __declspec(novtable) Controller : public G3D::ReferenceCountedObject
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

	// FUNCTION: WEBSERVICE 0x10030b80 FOLDED
	virtual ~Controller() {} // vtable+0x00

	virtual float getValue(InputType inputType) const;    // vtable+0x04
	virtual ControllerType getControllerType() const = 0; // vtable+0x08
	virtual bool hasIntelligence() const = 0;             // vtable+0x0c
	virtual bool isUserController() const = 0;            // vtable+0x10

	bool isNullController();

	static bool isControllableInput(InputType inputType);
	static Color3 controllerTypeToColor(ControllerType controllerType);
};

DECOMP_SIZE_ASSERT(Controller, 0x0c)

// VTABLE: WEBSERVICE 0x10236688
// SIZE 0x0c
class NullController : public Controller
{
public:
	// SYNTHETIC: WEBSERVICE 0x10099080
	// RBX::NullController::`scalar deleting destructor'

	virtual ControllerType getControllerType() const; // vtable+0x08
	virtual bool hasIntelligence() const;             // vtable+0x0c
	virtual bool isUserController() const;            // vtable+0x10

	static NullController* getStaticNullController();
};

DECOMP_SIZE_ASSERT(NullController, 0x0c)

// SIZE 0x8
class ControllerTypeArray
{
private:
	bool values[Controller::NUM_CONTROLLER_TYPES]; // 0x00

public:
	ControllerTypeArray() { clear(); }

	void clear()
	{
		for (int i = 0; i < Controller::NUM_CONTROLLER_TYPES; ++i) {
			values[i] = false;
		}
	}

	void setController(Controller::ControllerType controllerType, bool value);
	bool hasController(Controller::ControllerType controllerType) const;

	ControllerTypeArray& operator|=(const ControllerTypeArray& other);
};

DECOMP_SIZE_ASSERT(ControllerTypeArray, 0x8)

// FUNCTION: WEBSERVICE 0x10099400
inline NullController* NullController::getStaticNullController()
{
	static NullController n;
	return &n;
}

// SYNTHETIC: WEBSERVICE 0x10222ff0
// `RBX::NullController::getStaticNullController'::`2'::`dynamic atexit destructor for 'n''

} // namespace RBX

#endif // V8WORLD_CONTROLLER_H
