#include "v8datamodel/UserController.h"

#include "humanoid/Humanoid.h"
#include "network/Players.h"
#include "util/NavKeys.h"
#include "util/UserInputBase.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/PVInstance.h"
#include "v8datamodel/Workspace.h"

#include <math.h>
#include <stdlib.h>

namespace RBX {

char sControllerService[] = "ControllerService";

// FUNCTION: WEBSERVICE 0x100c5480
float AIController::getValue(InputType inputType) const
{
	return buffer[inputType];
}

// FUNCTION: WEBSERVICE 0x100c5490
// FUNCTION: ROBLOX 0x005d4590
void AIController::chaseTarget(const G3D::Vector3& localChasePoint)
{
	float angle = atan2(-localChasePoint.x, -localChasePoint.z);

	float left;
	float right;

	if (angle > 1.2566371f) {
		left = -0.9f;
		right = 0.9f;
	}
	else if (angle < -1.2566371f) {
		left = 0.9f;
		right = -0.9f;
	}
	else if (angle > 0.471238911151886) {
		left = 0.0f;
		right = 0.9f;
	}
	else if (angle < -0.471238911151886) {
		left = 0.9f;
		right = 0.0f;
	}
	else {
		left = 0.9f;
		right = 0.9f;
	}

	buffer[LEFT_TRACK_INPUT] = left;
	buffer[RIGHT_TRACK_INPUT] = right;
	buffer[RIGHT_LEFT_INPUT] = angle * 0.31830987f;
	buffer[BACK_FORWARD_INPUT] = localChasePoint.z < 0.0f ? 0.9f : 0.0f;
}

// FUNCTION: WEBSERVICE 0x100c5560
// FUNCTION: ROBLOX 0x005d4660
float KeyboardPrimaryController::getValue(InputType inputType) const
{
	if (!getHardwareDevice()) {
		return 0.0f;
	}

	switch (inputType) {
	case UP_DOWN_INPUT:
		return getHardwareDevice()->keyDown(SDLK_SPACE) ? 1.0f : 0.0f;
	case BUTTON_1_INPUT:
		return getHardwareDevice()->keyDown(SDLK_1) ? 1.0f : 0.0f;
	case BUTTON_2_INPUT:
		return getHardwareDevice()->keyDown(SDLK_2) ? 1.0f : 0.0f;
	case BUTTON_3_INPUT:
		return getHardwareDevice()->keyDown(SDLK_3) ? 1.0f : 0.0f;
	case BUTTON_4_INPUT:
		return getHardwareDevice()->keyDown(SDLK_4) ? 1.0f : 0.0f;
	case BUTTON_3_4_INPUT:
		return getHardwareDevice()->keyDown(SDLK_3) ? 1.0f : (getHardwareDevice()->keyDown(SDLK_4) ? -1.0f : 0.0f);
	}

	if (getHardwareDevice()->altKeyDown()) {
		return 0.0f;
	}
	if (getHardwareDevice()->ctrlKeyDown()) {
		return 0.0f;
	}

	bool F = getHardwareDevice()->keyDown(SDLK_u);
	bool B = getHardwareDevice()->keyDown(SDLK_j);
	bool L = getHardwareDevice()->keyDown(SDLK_h);
	bool R = getHardwareDevice()->keyDown(SDLK_k);

	switch (inputType) {
	default:
		return 0.0f;

	case RIGHT_LEFT_INPUT:
		return L ? 1.0f : (R ? -1.0f : 0.0f);

	case BACK_FORWARD_INPUT:
		return F ? 1.0f : (B ? -1.0f : 0.0f);

	case LEFT_TRACK_INPUT:
		if ((F && !L) || (R && !B) || (F && R)) {
			return 1.0f;
		}
		if ((B && !R) || (L && !F) || (B && L)) {
			return -1.0f;
		}
		return 0.0f;

	case RIGHT_TRACK_INPUT:
		if ((F && !R) || (L && !B) || (F && L)) {
			return 1.0f;
		}
		if ((B && !L) || (R && !F) || (B && R)) {
			return -1.0f;
		}
		return 0.0f;
	}
}

// FUNCTION: WEBSERVICE 0x100c57c0
// FUNCTION: ROBLOX 0x005d48c0
float KeyboardSecondaryController::getValue(InputType inputType) const
{
	if (!getHardwareDevice()) {
		return 0.0f;
	}

	switch (inputType) {
	case UP_DOWN_INPUT:
		return getHardwareDevice()->keyDown(SDLK_RSHIFT) ? 1.0f : 0.0f;
	}

	if (getHardwareDevice()->altKeyDown() || getHardwareDevice()->ctrlKeyDown()) {
		return 0.0f;
	}

	bool F = getHardwareDevice()->keyDown(SDLK_w);
	bool B = getHardwareDevice()->keyDown(SDLK_s);
	bool L = getHardwareDevice()->keyDown(SDLK_a);
	bool R = getHardwareDevice()->keyDown(SDLK_d);

	switch (inputType) {
	default:
		return 0.0f;

	case RIGHT_LEFT_INPUT:
		return L ? 1.0f : (R ? -1.0f : 0.0f);

	case BACK_FORWARD_INPUT:
		return F ? 1.0f : (B ? -1.0f : 0.0f);

	case LEFT_TRACK_INPUT:
		if ((F && !L) || (R && !B) || (F && R)) {
			return 1.0f;
		}
		if ((B && !R) || (L && !F) || (B && L)) {
			return -1.0f;
		}
		return 0.0f;

	case RIGHT_TRACK_INPUT:
		if ((F && !R) || (L && !B) || (F && L)) {
			return 1.0f;
		}
		if ((B && !L) || (R && !F) || (B && R)) {
			return -1.0f;
		}
		return 0.0f;
	}
}

// FUNCTION: WEBSERVICE 0x100c5c20
// FUNCTION: ROBLOX 0x005d4c40
const UserInputBase* PlayerController::getHardwareDevice() const
{
	return controllerService.expired() ? NULL : controllerService.lock()->getHardwareDevice();
}

// FUNCTION: WEBSERVICE 0x100c5ca0
// FUNCTION: ROBLOX 0x005d4cc0
G3D::Vector3 AIController::getLocalTargetPosition() const
{
	G3D::Vector3 answer(0, 0, 0);

	boost::shared_ptr<const PVInstance> lockPV = controlledInstance.lock();
	boost::shared_ptr<const PVInstance> lockTarget = target.lock();

	if (lockPV && lockTarget) {
		G3D::CoordinateFrame controlledCoord = lockPV->getLocation();
		G3D::Vector3 targetLoc = lockTarget->getLocation().translation;
		answer = controlledCoord.pointToObjectSpace(targetLoc);
	}

	return answer;
}

// FUNCTION: WEBSERVICE 0x100c5fb0
// FUNCTION: ROBLOX 0x005d4e60
void AIController::updateTarget(ControllerType excludeType, float time)
{
	if (!(time < retargetTime)) {
		int random = rand();
		retargetTime = random * (2.0f / RAND_MAX) + time + 1.0f;

		boost::shared_ptr<const PVInstance> pvLock = controlledInstance.lock();
		if (!pvLock) {
			return;
		}

		const PVInstance* root = pvLock->getTypedRoot<PVInstance>();
		if (!root) {
			return;
		}

		float bestDistance = 1e20f;
		G3D::CoordinateFrame controlledCoord = pvLock->getLocation();

		for (unsigned int i = 0; i < root->numChildren(); ++i) {
			const PVInstance* pv = dynamic_cast<const PVInstance*>((*root->getChildren())[i].get());

			if (pv && pv != pvLock.get() && pv->isChaseable() &&
				pv->getTopPVController()->getControllerType() != excludeType) {
				float distance = (pv->getLocation().translation - controlledCoord.translation).magnitude();
				if (distance < bestDistance) {
					bestDistance = distance;
					target = shared_from(pv);
				}
			}
		}
	}
}

// FUNCTION: WEBSERVICE 0x100c6280
// FUNCTION: ROBLOX 0x005d5130
void AIChaseController::updateBuffer(float time)
{
	updateTarget(AI_CHASE_CONTROLLER, time);
	if (!target.expired()) {
		chaseTarget(getLocalTargetPosition());
	}
	else {
		buffer[LEFT_TRACK_INPUT] = -1.0f;
		buffer[RIGHT_TRACK_INPUT] = 1.0f;
		buffer[RIGHT_LEFT_INPUT] = 1.0f;
	}
}

// FUNCTION: WEBSERVICE 0x100c62e0
// FUNCTION: ROBLOX 0x005d5190
void AIFleeController::updateBuffer(float time)
{
	updateTarget(AI_FLEE_CONTROLLER, time);
	if (!target.expired()) {
		chaseTarget(-getLocalTargetPosition());
	}
}

// FUNCTION: WEBSERVICE 0x100c6340
// FUNCTION: ROBLOX 0x005d51f0
SteppingController::~SteppingController()
{
	stopListening(runService, this);
}

// FUNCTION: WEBSERVICE 0x100c6410
// FUNCTION: ROBLOX 0x005d52c0
AIController::~AIController()
{
}

// FUNCTION: WEBSERVICE 0x100c6510
// FUNCTION: ROBLOX 0x005d5370
SteppingController::SteppingController(const PVInstance* _controlledInstance)
	: runService(shared_from(ServiceProvider::create<RunService>(_controlledInstance))),
	  controlledInstance(shared_from(_controlledInstance))
{
	startListening(runService, this);
}

// FUNCTION: WEBSERVICE 0x100c6600
// FUNCTION: ROBLOX 0x005d5460
PlayerController::PlayerController(ControllerService* _controllerService, const PVInstance* _controlledInstance)
	: SteppingController(_controlledInstance), controllerService(shared_from(_controllerService)), stepsRotating(0)
{
}

// FUNCTION: WEBSERVICE 0x100c6760
// FUNCTION: ROBLOX 0x005d55b0
void PlayerController::onEvent(const RunService* source, Heartbeat event)
{
	const UserInputBase* hardware = getHardwareDevice();

	Humanoid* localHumanoid = NULL;
	const Instance* controlled = controlledInstance.lock().get();
	ModelInstance* character = Network::Players::findLocalCharacter(controlled);
	if (controlled == character) {
		localHumanoid = Humanoid::modelIsCharacter(character);
	}

	Workspace* workspace = ServiceProvider::find<Workspace>(character);

	if (hardware && localHumanoid && workspace) {
		Camera* camera = workspace->getCamera();

		NavKeys nav;
		hardware->getNavKeys(nav);

		if (nav.arrowKeyDown()) {
			if (nav.left_arrow || nav.right_arrow) {
				camera->panRadians(nav.leftRightArrow() * event.step * 1.8f);
				stepsRotating++;
			}
			else {
				stepsRotating = 0;
			}

			float forwardBackward = nav.forwardBackwardArrow();

			G3D::Vector3 look = camera->getCameraCoordinateFrame().lookVector();
			look.y = 0.0f;
			look.unitize();

			G3D::Vector3 move = G3D::Vector3(look.x, 0.0f, look.z) * forwardBackward;
			localHumanoid->setWalkDirection(move);
		}
		else if (nav.asdwKeyDown()) {
			float forwardBackward = nav.forwardBackwardASDW();
			float leftRight = nav.leftRightASDW();

			G3D::Vector3 look = camera->getCameraCoordinateFrame().lookVector();
			look.y = 0.0f;
			look.unitize();

			G3D::Vector3 moveForward = G3D::Vector3(look.x, 0.0f, look.z) * forwardBackward;
			moveForward += G3D::Vector3(look.z, 0.0f, -look.x) * leftRight;
			if (moveForward.squaredMagnitude() > 1.0f) {
				moveForward.unitize();
			}

			localHumanoid->setWalkDirection(moveForward);
		}
		else {
			localHumanoid->setWalkDirection(G3D::Vector3::zero());
		}

		if (nav.space) {
			localHumanoid->setJump(true);
		}
	}
}

// FUNCTION: WEBSERVICE 0x100c6a30
// FUNCTION: ROBLOX 0x005d5880
AIController::AIController(const PVInstance* controlledInstance)
	: SteppingController(controlledInstance), retargetTime(0.0f)
{
	for (int i = 0; i < NUM_INPUT_TYPES; ++i) {
		buffer[i] = 0.0f;
	}
	buffer[CONSTANT_INPUT] = 1.0f;
}

// FUNCTION: WEBSERVICE 0x100c6be0
// FUNCTION: ROBLOX 0x005d5a30
ControllerService::ControllerService()
	: hardwareDevice(NULL), keyboardPrimaryController(new KeyboardPrimaryController(this)),
	  keyboardSecondaryController(new KeyboardSecondaryController(this)), nullController(new NullController())
{
}

// FUNCTION: WEBSERVICE 0x100c6f00
// FUNCTION: ROBLOX 0x005d5d50
G3D::ReferenceCountedPointer<Controller> ControllerService::getController(
	Controller::ControllerType type, const PVInstance* controlledInstance
)
{
	switch (type) {
	case Controller::PLAYER_CONTROLLER:
		return new PlayerController(this, controlledInstance);

	case Controller::PRIMARY_CONTROLLER:
		return keyboardPrimaryController;

	case Controller::SECONDARY_CONTROLLER:
		return keyboardSecondaryController;

	case Controller::AI_CHASE_CONTROLLER:
		return new AIChaseController(controlledInstance);

	case Controller::AI_FLEE_CONTROLLER:
		return new AIFleeController(controlledInstance);

	default:
		return nullController;
	}
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::ControllerService, RBX::Instance, RBX::sControllerService>;
template class RBX::Reflection::Described<
	RBX::ControllerService,
	RBX::sControllerService,
	RBX::FactoryProduct<RBX::ControllerService, RBX::Instance, RBX::sControllerService> >;
template class RBX::DescribedCreatable<RBX::ControllerService, RBX::Instance, RBX::sControllerService>;
