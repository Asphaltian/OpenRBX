#ifndef V8DATAMODEL_USERCONTROLLER_H
#define V8DATAMODEL_USERCONTROLLER_H

#include "decomp.h"
#include "util/Events.h"
#include "util/Handle.h"
#include "util/RunStateOwner.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"
#include "v8world/Controller.h"

#include <G3D/ReferenceCount.h>
#include <G3D/Vector3.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace RBX {

class PVInstance;
class UserInputBase;

extern char sControllerService[];
// SIZE 0x10c
class ControllerService : public DescribedCreatable<ControllerService, Instance, sControllerService>, public Service
{
private:
	UserInputBase* hardwareDevice;                                        // 0x0f8
	G3D::ReferenceCountedPointer<Controller> playerController;            // 0x0fc
	G3D::ReferenceCountedPointer<Controller> keyboardPrimaryController;   // 0x100
	G3D::ReferenceCountedPointer<Controller> keyboardSecondaryController; // 0x104
	G3D::ReferenceCountedPointer<Controller> nullController;              // 0x108

public:
	ControllerService();

	G3D::ReferenceCountedPointer<Controller> getController(
		Controller::ControllerType type, const PVInstance* controlledInstance
	);

	const UserInputBase* getHardwareDevice() const { return hardwareDevice; }
	void setHardwareDevice(UserInputBase*);
	UserInputBase* getHardwareDevice() { return hardwareDevice; }
};

DECOMP_SIZE_ASSERT(ControllerService, 0x10c)

// SIZE 0x20
class SteppingController : public Controller, public Listener<RunService, Heartbeat>
{
private:
	boost::shared_ptr<RunService> runService; // 0x10

protected:
	boost::weak_ptr<const PVInstance> controlledInstance; // 0x18

	SteppingController(const PVInstance* _controlledInstance);
	virtual ~SteppingController();
};

DECOMP_SIZE_ASSERT(SteppingController, 0x20)

// SIZE 0x64
class AIController : public SteppingController
{
private:
	float retargetTime; // 0x20

protected:
	float buffer[NUM_INPUT_TYPES];            // 0x24
	boost::weak_ptr<const PVInstance> target; // 0x5c

	AIController(const PVInstance* controlledInstance);
	virtual ~AIController();

	void updateTarget(ControllerType excludeType, float time);
	G3D::Vector3 getLocalTargetPosition() const;
	void chaseTarget(const G3D::Vector3& localChasePoint);

	virtual bool hasIntelligence() const { return true; }
	virtual bool isUserController() const { return false; }
	virtual float getValue(InputType inputType) const;
	virtual void updateBuffer(float time) = 0; // vtable+0x14

	virtual void onEvent(const RunService* source, Heartbeat event) { updateBuffer(event.time); }
};

DECOMP_SIZE_ASSERT(AIController, 0x64)

// SIZE 0x64
class AIChaseController : public AIController
{
protected:
	virtual void updateBuffer(float time);

public:
	AIChaseController(const PVInstance* controlledInstance) : AIController(controlledInstance) {}

	virtual ControllerType getControllerType() const { return AI_CHASE_CONTROLLER; }
};

DECOMP_SIZE_ASSERT(AIChaseController, 0x64)

// SIZE 0x64
class AIFleeController : public AIController
{
protected:
	virtual void updateBuffer(float time);

public:
	AIFleeController(const PVInstance* controlledInstance) : AIController(controlledInstance) {}

	virtual ControllerType getControllerType() const { return AI_FLEE_CONTROLLER; }
};

DECOMP_SIZE_ASSERT(AIFleeController, 0x64)

// SIZE 0x10
class __declspec(novtable) HardwareController : public Controller
{
private:
	ControllerService* controllerService; // 0x0c

protected:
	HardwareController(ControllerService* _controllerService) : controllerService(_controllerService) {}

	const UserInputBase* getHardwareDevice() const
	{
		return controllerService ? controllerService->getHardwareDevice() : NULL;
	}

	virtual bool hasIntelligence() const { return true; }
	virtual bool isUserController() const { return true; }
};

DECOMP_SIZE_ASSERT(HardwareController, 0x10)

// SIZE 0x10
class KeyboardPrimaryController : public HardwareController
{
public:
	KeyboardPrimaryController(ControllerService* _controllerService) : HardwareController(_controllerService) {}

	virtual float getValue(InputType inputType) const;
	virtual ControllerType getControllerType() const { return PRIMARY_CONTROLLER; }
};

DECOMP_SIZE_ASSERT(KeyboardPrimaryController, 0x10)

// SIZE 0x10
class KeyboardSecondaryController : public HardwareController
{
public:
	KeyboardSecondaryController(ControllerService* _controllerService) : HardwareController(_controllerService) {}

	virtual float getValue(InputType inputType) const;
	virtual ControllerType getControllerType() const { return SECONDARY_CONTROLLER; }
};

DECOMP_SIZE_ASSERT(KeyboardSecondaryController, 0x10)

// SIZE 0x2c
class PlayerController : public SteppingController
{
private:
	boost::weak_ptr<ControllerService> controllerService; // 0x20
	int stepsRotating;                                    // 0x28

	const UserInputBase* getHardwareDevice() const;

	virtual bool hasIntelligence() const { return true; }
	virtual bool isUserController() const { return true; }
	virtual ControllerType getControllerType() const { return PLAYER_CONTROLLER; }
	virtual void onEvent(const RunService* source, Heartbeat event);

public:
	PlayerController(ControllerService* _controllerService, const PVInstance* _controlledInstance);
};

DECOMP_SIZE_ASSERT(PlayerController, 0x2c)

// clang-format off
// FUNCTION: WEBSERVICE 0x100c53e0
// FUNCTION: ROBLOX 0x005d44d0
// RBX::HardwareController::getHardwareDevice
// FUNCTION: WEBSERVICE 0x100c6480
// FUNCTION: ROBLOX 0x005d5330
// RBX::AIController::onEvent
// SYNTHETIC: WEBSERVICE 0x100c64a0
// SYNTHETIC: ROBLOX 0x005d5350
// RBX::SteppingController::`scalar deleting destructor'
// SYNTHETIC: WEBSERVICE 0x100c66e0
// SYNTHETIC: ROBLOX 0x005d5530
// RBX::PlayerController::~PlayerController
// SYNTHETIC: WEBSERVICE 0x100c6740
// SYNTHETIC: ROBLOX 0x005d5590
// RBX::PlayerController::`scalar deleting destructor'
// FUNCTION: WEBSERVICE 0x100c6b60
// FUNCTION: ROBLOX 0x005d59c0
// RBX::AIChaseController::AIChaseController
// SYNTHETIC: WEBSERVICE 0x100c6b90 FOLDED
// SYNTHETIC: ROBLOX 0x005d5a10 FOLDED
// RBX::AIController::`scalar deleting destructor'
// SYNTHETIC: WEBSERVICE 0x100c6b90 FOLDED
// SYNTHETIC: ROBLOX 0x005d5a10 FOLDED
// RBX::AIChaseController::`scalar deleting destructor'
// SYNTHETIC: WEBSERVICE 0x100c6b90 FOLDED
// SYNTHETIC: ROBLOX 0x005d5a10 FOLDED
// RBX::AIFleeController::`scalar deleting destructor'
// FUNCTION: WEBSERVICE 0x100c6bb0
// FUNCTION: ROBLOX 0x005d59e0
// RBX::AIFleeController::AIFleeController
// SYNTHETIC: WEBSERVICE 0x100c6da0
// SYNTHETIC: ROBLOX 0x005d5bf0
// RBX::ControllerService::~ControllerService
// SYNTHETIC: WEBSERVICE 0x100c6ee0
// SYNTHETIC: ROBLOX 0x005d5d30
// RBX::ControllerService::`scalar deleting destructor'
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_USERCONTROLLER_H
