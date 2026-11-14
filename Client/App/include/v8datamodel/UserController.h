#ifndef V8DATAMODEL_USERCONTROLLER_H
#define V8DATAMODEL_USERCONTROLLER_H

#include "placeholder.h"
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
class ControllerService : public DescribedCreatable<ControllerService, Instance, sControllerService>, public Service
{
private:
	UserInputBase* hardwareDevice;
	G3D::ReferenceCountedPointer<Controller> playerController;
	G3D::ReferenceCountedPointer<Controller> keyboardPrimaryController;
	G3D::ReferenceCountedPointer<Controller> keyboardSecondaryController;
	G3D::ReferenceCountedPointer<Controller> nullController;

public:
	ControllerService();

	G3D::ReferenceCountedPointer<Controller> getController(
		Controller::ControllerType type, const PVInstance* controlledInstance
	);

	const UserInputBase* getHardwareDevice() const { return hardwareDevice; }
	void setHardwareDevice(UserInputBase* device) { hardwareDevice = device; }
	UserInputBase* getHardwareDevice() { return hardwareDevice; }
};

class SteppingController : public Controller, public Listener<RunService, Heartbeat>
{
private:
	boost::shared_ptr<RunService> runService;

protected:
	boost::weak_ptr<const PVInstance> controlledInstance;

	SteppingController(const PVInstance* _controlledInstance);
	virtual ~SteppingController();
};

class AIController : public SteppingController
{
private:
	float retargetTime;

protected:
	float buffer[NUM_INPUT_TYPES];
	boost::weak_ptr<const PVInstance> target;

	AIController(const PVInstance* controlledInstance);
	virtual ~AIController();

	void updateTarget(ControllerType excludeType, float time);
	G3D::Vector3 getLocalTargetPosition() const;
	void chaseTarget(const G3D::Vector3& localChasePoint);

	virtual bool hasIntelligence() const { return true; }
	virtual bool isUserController() const { return false; }
	virtual float getValue(InputType inputType) const;
	virtual void updateBuffer(float time) = 0;

	virtual void onEvent(const RunService* source, Heartbeat event) { updateBuffer(event.time); }
};

class AIChaseController : public AIController
{
protected:
	virtual void updateBuffer(float time);

public:
	AIChaseController(const PVInstance* controlledInstance) : AIController(controlledInstance) {}

	virtual ControllerType getControllerType() const { return AI_CHASE_CONTROLLER; }
};

class AIFleeController : public AIController
{
protected:
	virtual void updateBuffer(float time);

public:
	AIFleeController(const PVInstance* controlledInstance) : AIController(controlledInstance) {}

	virtual ControllerType getControllerType() const { return AI_FLEE_CONTROLLER; }
};

class __declspec(novtable) HardwareController : public Controller
{
private:
	ControllerService* controllerService;

protected:
	HardwareController(ControllerService* _controllerService) : controllerService(_controllerService) {}

	const UserInputBase* getHardwareDevice() const
	{
		return controllerService ? controllerService->getHardwareDevice() : NULL;
	}

	virtual bool hasIntelligence() const { return true; }
	virtual bool isUserController() const { return true; }
};

class KeyboardPrimaryController : public HardwareController
{
public:
	KeyboardPrimaryController(ControllerService* _controllerService) : HardwareController(_controllerService) {}

	virtual float getValue(InputType inputType) const;
	virtual ControllerType getControllerType() const { return PRIMARY_CONTROLLER; }
};

class KeyboardSecondaryController : public HardwareController
{
public:
	KeyboardSecondaryController(ControllerService* _controllerService) : HardwareController(_controllerService) {}

	virtual float getValue(InputType inputType) const;
	virtual ControllerType getControllerType() const { return SECONDARY_CONTROLLER; }
};

class PlayerController : public SteppingController
{
private:
	boost::weak_ptr<ControllerService> controllerService;
	int stepsRotating;

	const UserInputBase* getHardwareDevice() const;

	virtual bool hasIntelligence() const { return true; }
	virtual bool isUserController() const { return true; }
	virtual ControllerType getControllerType() const { return PLAYER_CONTROLLER; }
	virtual void onEvent(const RunService* source, Heartbeat event);

public:
	PlayerController(ControllerService* _controllerService, const PVInstance* _controlledInstance);
};

} // namespace RBX

#endif // V8DATAMODEL_USERCONTROLLER_H
