#include "humanoid/Humanoid.h"

#include "humanoid/FallingDown.h"
#include "network/Players.h"
#include "rbxgraphics/Adorn.h"
#include "reflection/property.h"
#include "util/Color.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/ForceField.h"
#include "v8datamodel/JointInstance.h"
#include "v8datamodel/ModelInstance.h"
#include "v8datamodel/Teams.h"
#include "v8datamodel/Tool.h"
#include "v8datamodel/Workspace.h"
#include "v8kernel/Body.h"
#include "v8tree/Service.h"
#include "v8world/Joint.h"
#include "v8world/Primitive.h"
#include "v8world/World.h"

#include <algorithm>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <stdexcept>
#include <vector>

namespace RBX {

char sHumanoid[] = "Humanoid";

using namespace Reflection;

static RefPropDescriptor<Humanoid, PartInstance> propTorso(
	"Torso",
	"Data",
	&RBX::Humanoid::getTorso,
	&RBX::Humanoid::setTorso,
	Reflection::PropertyDescriptor::UI
);

static RefPropDescriptor<Humanoid, PartInstance> propLeftLeg(
	"LeftLeg",
	"Data",
	&RBX::Humanoid::getLeftLeg,
	&RBX::Humanoid::setLeftLeg,
	Reflection::PropertyDescriptor::UI
);

static RefPropDescriptor<Humanoid, PartInstance> propRightLeg(
	"RightLeg",
	"Data",
	&RBX::Humanoid::getRightLeg,
	&RBX::Humanoid::setRightLeg,
	Reflection::PropertyDescriptor::UI
);

static RefPropDescriptor<Humanoid, PartInstance> propWalkToPart(
	"WalkToPart",
	"Control",
	&RBX::Humanoid::getWalkToPart,
	&RBX::Humanoid::setWalkToPart,
	Reflection::PropertyDescriptor::STREAMING
);

static PropDescriptor<Humanoid, G3D::Vector3> propWalkToPoint(
	"WalkToPoint",
	"Control",
	&RBX::Humanoid::getWalkToPoint,
	&RBX::Humanoid::setWalkToPoint,
	Reflection::PropertyDescriptor::STREAMING
);

static PropDescriptor<Humanoid, G3D::Vector3> propWalkDirection(
	"WalkDirection",
	"Control",
	&RBX::Humanoid::getWalkDirection,
	&RBX::Humanoid::setWalkDirection,
	Reflection::PropertyDescriptor::STREAMING
);

static PropDescriptor<Humanoid, float> propWalkRotationalVelocity(
	"WalkRotationalVelocity",
	"Control",
	&RBX::Humanoid::getWalkRotationalVelocity,
	&RBX::Humanoid::setWalkRotationalVelocity,
	Reflection::PropertyDescriptor::STREAMING
);

static PropDescriptor<Humanoid, bool> propJump(
	"Jump",
	"Control",
	&RBX::Humanoid::getJump,
	&RBX::Humanoid::setJump,
	Reflection::PropertyDescriptor::STREAMING
);

static PropDescriptor<Humanoid, G3D::Vector3> propTargetPoint(
	"TargetPoint",
	"Control",
	&RBX::Humanoid::getTargetPoint,
	&RBX::Humanoid::setTargetPoint,
	Reflection::PropertyDescriptor::STREAMING
);

static PropDescriptor<Humanoid, bool> propSit(
	"Sit",
	"Control",
	&RBX::Humanoid::getSit,
	&RBX::Humanoid::setSit,
	Reflection::PropertyDescriptor::STREAMING
);

static PropDescriptor<Humanoid, float> propHealth(
	"Health",
	"Game",
	&RBX::Humanoid::getHealth,
	&RBX::Humanoid::setHealth
);

static PropDescriptor<Humanoid, float> propMaxHealth(
	"MaxHealth",
	"Game",
	&RBX::Humanoid::getMaxHealth,
	&RBX::Humanoid::setMaxHealth
);

static BoundFuncDesc<Humanoid, void(float), 1> func_TakeDamage(
	&RBX::Humanoid::takeDamage,
	"TakeDamage",
	"amount",
	Reflection::FunctionDescriptor::AnyCaller
);

static BoundFuncDesc<Humanoid, void(G3D::Vector3, boost::shared_ptr<Instance>), 2> func_MoveTo(
	&RBX::Humanoid::moveTo2,
	"MoveTo",
	"location",
	"part",
	Reflection::FunctionDescriptor::AnyCaller
);

SignalDesc<Humanoid, void()> Humanoid::event_Died("Died");
SignalDesc<Humanoid, void(float)> Humanoid::event_Running("Running", "speed");
SignalDesc<Humanoid, void(float)> Humanoid::event_Climbing("Climbing", "speed");
SignalDesc<Humanoid, void(bool)> Humanoid::event_Jumping("Jumping", "active");
SignalDesc<Humanoid, void(bool)> Humanoid::event_FreeFalling("FreeFalling", "active");
SignalDesc<Humanoid, void(bool)> Humanoid::event_GettingUp("GettingUp", "active");
SignalDesc<Humanoid, void(bool)> Humanoid::event_FallingDown("FallingDown", "active");
SignalDesc<Humanoid, void(bool)> Humanoid::event_Seated("Seated", "active");

// FUNCTION: WEBSERVICE 0x100a0830
PartInstance* Humanoid::getHeadFromCharacter(const ModelInstance* character)
{
	return character ? dynamic_cast<PartInstance*>(character->findFirstChildByName("Head")) : NULL;
}

// FUNCTION: WEBSERVICE 0x100a08d0
ModelInstance* Humanoid::getCharacterFromHumanoid(Humanoid* humanoid)
{
	if (humanoid != NULL) {
		Instance* parent = humanoid->getParent();
		ModelInstance* model = dynamic_cast<ModelInstance*>(parent);
		if (model != NULL) {
			return model;
		}
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x100a0900
G3D::CoordinateFrame Humanoid::getTopOfHead() const
{
	return G3D::CoordinateFrame(G3D::Matrix3::identity(), G3D::Vector3(0, 0.5f, 0));
}

// FUNCTION: WEBSERVICE 0x100a0990
ContactManager* Humanoid::getContactManager()
{
	World* world = Workspace::getWorldIfInWorkspace(this);

	if (world != NULL) {
		return world->getContactManager();
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x100a0bb0
PartInstance* Humanoid::getLocalHeadFromContext(const Instance* context)
{
	ModelInstance* character = Network::Players::findLocalCharacter(context);

	return getHeadFromCharacter(character);
}

// FUNCTION: WEBSERVICE 0x100a0bd0
bool Humanoid::hasWalkToPoint(G3D::Vector3& worldPosition) const
{
	if (walkToPart != NULL && Workspace::findWorkspace(this) != NULL &&
		Workspace::contextInWorkspace(walkToPart.get())) {
		worldPosition = walkToPart->getCoordinateFrame().pointToWorldSpace(walkToPoint);

		return true;
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x100a0c50
G3D::CoordinateFrame Humanoid::getRightArmGrip() const
{
	G3D::CoordinateFrame grip;
	grip.lookAt(G3D::Vector3(0, -1, 0), G3D::Vector3(0, 0, -1));
	grip.translation = G3D::Vector3(0, -1, 0);

	return grip;
}

// FUNCTION: WEBSERVICE 0x100a0cb0
PartInstance* Humanoid::findRightArm() const
{
	return static_cast<PartInstance*>(getParent() ? getParent()->findFirstChildByName("Right Arm") : NULL);
}

// FUNCTION: WEBSERVICE 0x100a0d40
void Humanoid::computeForce(float dt, bool throttling)
{
	if (currentState.get() != NULL) {
		currentState->onComputeForce(dt);
	}
}

// FUNCTION: WEBSERVICE 0x100a0d60
void Humanoid::setState(State* state)
{
	if (state != currentState.get()) {
		currentState.reset(state);
	}
}

// FUNCTION: WEBSERVICE 0x100a1100
Humanoid::~Humanoid()
{
}

// FUNCTION: WEBSERVICE 0x100a13b0
void Humanoid::getCameraIgnorePrimitives(std::vector<const Primitive*>& primitives)
{
	getIgnorePrims(primitives);
}

// FUNCTION: WEBSERVICE 0x100a1480
float Humanoid::getIntendedRotationAboutYAxis()
{
	if (currentState.get() != NULL) {
		return currentState->getIntendedRotationAboutYAxis();
	}

	return 0;
}

// FUNCTION: WEBSERVICE 0x100a1840
Humanoid* Humanoid::modelIsCharacter(const Instance* instance)
{
	return instance ? instance->findFirstChildOfType<Humanoid>() : NULL;
}

// FUNCTION: WEBSERVICE 0x100a19e0
Humanoid* Humanoid::getLocalHumanoidFromContext(const Instance* context)
{
	return modelIsCharacter(Network::Players::findLocalCharacter(context));
}

// STUB: WEBSERVICE 0x100a1b00
void Humanoid::render3dAdorn(Adorn* adorn)
{
	STUB(0x100a1b00);
}

// FUNCTION: WEBSERVICE 0x100a2010
PartInstance* Humanoid::getHead() const
{
	if (head == NULL && getParent() != NULL) {
		head = shared_from_dynamic_cast<PartInstance>(getParent()->findFirstChildByName("Head"));
	}

	return head.get();
}

// FUNCTION: WEBSERVICE 0x100a2110
PartInstance* Humanoid::getTorso() const
{
	if (torso == NULL && getParent() != NULL) {
		torso = shared_from_dynamic_cast<PartInstance>(getParent()->findFirstChildByName("Torso"));
	}

	return torso.get();
}

// FUNCTION: WEBSERVICE 0x100a2210
PartInstance* Humanoid::getLeftLeg() const
{
	if (leftLeg == NULL && getParent() != NULL) {
		leftLeg = shared_from_dynamic_cast<PartInstance>(getParent()->findFirstChildByName("Left Leg"));
	}

	return leftLeg.get();
}

// FUNCTION: WEBSERVICE 0x100a2310
PartInstance* Humanoid::getRightLeg() const
{
	if (rightLeg == NULL && getParent() != NULL) {
		rightLeg = shared_from_dynamic_cast<PartInstance>(getParent()->findFirstChildByName("Right Leg"));
	}

	return rightLeg.get();
}

// FUNCTION: WEBSERVICE 0x100a2410
PartInstance* Humanoid::getLeftArm() const
{
	if (leftArm == NULL && getParent() != NULL) {
		leftArm = shared_from_dynamic_cast<PartInstance>(getParent()->findFirstChildByName("Left Arm"));
	}

	return leftArm.get();
}

// FUNCTION: WEBSERVICE 0x100a2510
PartInstance* Humanoid::getRightArm() const
{
	if (rightArm == NULL && getParent() != NULL) {
		rightArm = shared_from_dynamic_cast<PartInstance>(getParent()->findFirstChildByName("Right Arm"));
	}

	return rightArm.get();
}

// FUNCTION: WEBSERVICE 0x100a2610
JointInstance* Humanoid::findRightShoulder() const
{
	PartInstance* torso = getTorso();
	return static_cast<JointInstance*>(torso ? torso->findFirstChildByName("Right Shoulder") : NULL);
}

// FUNCTION: WEBSERVICE 0x100a26a0
Primitive* Humanoid::getTorsoPrimitive() const
{
	PartInstance* part = getTorso();

	return part != NULL ? part->getPrimitive() : NULL;
}

// FUNCTION: WEBSERVICE 0x100a26c0
Primitive* Humanoid::getLeftLegPrimitive()
{
	PartInstance* part = getLeftLeg();

	return part != NULL ? part->getPrimitive() : NULL;
}

// FUNCTION: WEBSERVICE 0x100a26e0
Primitive* Humanoid::getRightLegPrimitive()
{
	PartInstance* part = getRightLeg();

	return part != NULL ? part->getPrimitive() : NULL;
}

// FUNCTION: WEBSERVICE 0x100a2700
Body* Humanoid::getTorsoBody()
{
	PartInstance* part = getTorso();

	return part != NULL ? part->getPrimitive()->getBody() : NULL;
}

// FUNCTION: WEBSERVICE 0x100a2720
Body* Humanoid::getRootBody()
{
	Body* body = getTorsoBody();

	return body != NULL ? body->getRoot() : NULL;
}

// FUNCTION: WEBSERVICE 0x100a2750
float Humanoid::getTorsoHeading() const
{
	Primitive* torso = getTorsoPrimitive();

	if (torso != NULL) {
		return Math::getHeading(torso->getCoordinateFrame().lookVector());
	}

	return 0.0f;
}

// FUNCTION: WEBSERVICE 0x100a27c0
const CoordinateFrame Humanoid::getLocation() const
{
	return getHead() != NULL ? getHead()->getCoordinateFrame() : CoordinateFrame();
}

// FUNCTION: WEBSERVICE 0x100a2f70
G3D::Vector3 Humanoid::getIntendedMovementVector(bool setWalkMode)
{
	G3D::Vector3 result = G3D::Vector3::zero();

	if (walkMode == DIRECTION_MOVE) {
		result = walkDirection;
	}
	else if (walkMode == CLICK_TO_MOVE) {
		PartInstance* torso = getTorso();
		G3D::Vector3 worldPosition;

		if (torso != NULL && hasWalkToPoint(worldPosition)) {
			const G3D::Vector3& translation = torso->getCoordinateFrame().translation;

			G3D::Vector3 delta = worldPosition - translation;

			worldPosition.x = delta.x;
			worldPosition.y = delta.y + 3.0f;
			worldPosition.z = delta.z;

			float distance = (worldPosition.z * worldPosition.z + worldPosition.x * worldPosition.x) +
							 worldPosition.y * worldPosition.y;

			worldPosition.y = 0;

			if (sqrtf(distance) < 2.0f) {
				if (setWalkMode) {
					walkMode = DIRECTION_MOVE;
				}

				result = G3D::Vector3::zero();
			}
			else {
				result = worldPosition.unit();
			}
		}
	}

	return result;
}

// FUNCTION: WEBSERVICE 0x100a30d0
G3D::Vector3 Humanoid::getIntendedMovementVector()
{
	return getIntendedMovementVector(false);
}

// FUNCTION: WEBSERVICE 0x100a30f0
G3D::Vector3 Humanoid::updateWalkDirection()
{
	return getIntendedMovementVector(true);
}

// FUNCTION: WEBSERVICE 0x100a3110
static void collectAllDescendantCharacterPrims(
	boost::shared_ptr<Instance> descendant,
	std::vector<Primitive*>& primitives
)
{
	PartInstance* part = dynamic_cast<PartInstance*>(descendant.get());
	if (part) {
		primitives.push_back(part->getPrimitive());
	}
}

// FUNCTION: WEBSERVICE 0x100a31b0
void Humanoid::tellCameraNear(float distance)
{
	std::vector<Primitive*> primitives;
	if (ModelInstance* character = getCharacterFromHumanoid(this)) {
		character->visitDescendents(boost::bind(&collectAllDescendantCharacterPrims, _1, boost::ref(primitives)));
	}

	float alphaModifier = 1.0f;
	if (distance < 3.0f) {
		alphaModifier = 0.0f;
	}
	else if (distance < 6.0f) {
		alphaModifier = 0.5f;
	}

	for (size_t i = 0; i < primitives.size(); ++i) {
		PartInstance* part = PartInstance::fromPrimitive(primitives[i]);
		if (!dynamic_cast<Tool*>(part->getParent())) {
			part->setAlphaModifier(alphaModifier);
		}
	}
}

// FUNCTION: WEBSERVICE 0x100a4290
Humanoid::Humanoid()
	: health(100), maxHealth(100), walkRotationalVelocity(0), walkTimer(0), walkMode(DIRECTION_MOVE), jump(false),
	  imDead(false), hadHeadJoint(false), sit(false), world(NULL)
{
	setName("Humanoid");
}

// FUNCTION: WEBSERVICE 0x100a44c0
void Humanoid::onLocalHumanoidEnteringWorkspace()
{
	Workspace* workspace = ServiceProvider::find<Workspace>(this);

	if (workspace) {
		if (this == getLocalHumanoidFromContext(this)) {
			if (workspace->getCamera()->getCameraType() == Camera::CUSTOM_CAMERA) {
				if (workspace->getCamera()->getCameraSubject() != this) {
					workspace->getCamera()->setCameraSubject(this);
				}
			}

			if (getHead()) {
				getHead()->setRenderImportance(1.0f);
			}
			if (getLeftLeg()) {
				getLeftLeg()->setRenderImportance(1.0f);
			}
			if (getRightLeg()) {
				getRightLeg()->setRenderImportance(1.0f);
			}
			if (getLeftArm()) {
				getLeftArm()->setRenderImportance(1.0f);
			}
			if (getRightArm()) {
				getRightArm()->setRenderImportance(1.0f);
			}
			if (getTorso()) {
				getTorso()->setRenderImportance(1.0f);
			}
		}
	}
}

// FUNCTION: WEBSERVICE 0x100a4610
void Humanoid::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	stopListening(oldProvider ? oldProvider->create<RunService>() : NULL, this);

	Instance::onServiceProvider(oldProvider, newProvider);

	startListening(newProvider ? newProvider->create<RunService>() : NULL, this);

	if (newProvider && !oldProvider) {
		onLocalHumanoidEnteringWorkspace();
	}
}

// STUB: WEBSERVICE 0x100a4690
void Humanoid::renderMultiplayer(Adorn* adorn, const G3D::GCamera& camera)
{
	if (getHead() != NULL) {
		const CoordinateFrame& headCoord = head->getCoordinateFrame();

		Vector3 headPosition(headCoord.translation.x, headCoord.translation.y + 1.5f, headCoord.translation.z);

		Vector3 projected = camera.project(headPosition, adorn->getViewport());

		float distance = (camera.getCoordinateFrame().translation - headPosition).magnitude();

		if (distance < 100.0f) {
			int fontSize = 8;

			if (20.0f <= distance) {
				if (distance < 50.0f) {
					fontSize = 10;
				}
			}
			else {
				fontSize = 12;
			}

			Teams* teams = ServiceProvider::find<Teams>(this);

			G3D::Color3 nameColor = teams == NULL ? G3D::Color3::white() : teams->getTeamColorForHumanoid(this);

			adorn->drawFont2D(
				getName(),
				projected.xy(),
				fontSize,
				G3D::Color4(nameColor, 1.0f),
				G3D::Color4(G3D::Color3::black(), 1.0f),
				Adorn::XALIGN_CENTER,
				Adorn::YALIGN_CENTER,
				Adorn::PROPORTIONAL_SPACING
			);

			float width = (float) (fontSize * 4);

			headPosition.x -= 0.5f * width;

			Vector2 barLow(headPosition.x, headPosition.y);
			Vector2 barHigh(headPosition.x + width, headPosition.y + (float) (fontSize / 2));

			Vector2 healthHigh(headPosition.x + (health / maxHealth) * width, headPosition.y);

			adorn->rect2d(G3D::Rect2D::xyxy(barLow, healthHigh), G3D::Color4(Color::lightGreen(), 1.0f));
			adorn->rect2d(G3D::Rect2D::xyxy(healthHigh, barHigh), G3D::Color4(G3D::Color3::red(), 1.0f));
		}
	}
}

// FUNCTION: WEBSERVICE 0x100a4960
void Humanoid::render2d(Adorn* adorn)
{
	Humanoid* self = this;
	ModelInstance* character = Network::Players::findLocalCharacter(self);
	Humanoid* localHumanoid = character != NULL ? character->findFirstChildOfType<Humanoid>() : NULL;

	if (this == localHumanoid) {
		return;
	}

	Workspace* workspace = Workspace::findWorkspace(self);

	if (workspace != NULL) {
		renderMultiplayer(adorn, workspace->getGCamera());
	}
}

// STUB: WEBSERVICE 0x100a4bf0
void Humanoid::getIgnorePrims(std::vector<const Primitive*>& ignore)
{
	STUB(0x100a4bf0);
}

// FUNCTION: WEBSERVICE 0x100a4d50
void Humanoid::setHealth(float value)
{
	float newHealth = G3D::clamp(value, 0.0f, maxHealth);

	if (health != newHealth) {
		health = newHealth;
		raisePropertyChanged(propHealth);

		if (health == 0.0f && !imDead) {
			imDead = true;
			setState(new FallingDown(this, Math::inf()));

			if (ModelInstance* character = getCharacterFromHumanoid(this)) {
				character->breakJoints();
			}

			event_Died.fire(this);
		}
	}
}

// FUNCTION: WEBSERVICE 0x100a4e80
void Humanoid::setMaxHealth(float value)
{
	if (maxHealth != value) {
		maxHealth = value;
		raisePropertyChanged(propMaxHealth);
	}
}

// FUNCTION: WEBSERVICE 0x100a4eb0
void Humanoid::takeDamage(float value)
{
	if (getTorso() && !ForceField::partInForceField(getTorso())) {
		setHealth(health - value);
	}
}

// FUNCTION: WEBSERVICE 0x100a4ef0
void Humanoid::setWalkDirection(const G3D::Vector3& value)
{
	if (walkDirection != value) {
		if (value == G3D::Vector3::zero()) {
			walkDirection = value;
		}
		else {
			walkDirection = G3D::Vector3(value.x, 0.0f, value.z).direction();
			walkMode = DIRECTION_MOVE;
		}

		raisePropertyChanged(propWalkDirection);
	}
}

// FUNCTION: WEBSERVICE 0x100a5000
void Humanoid::setWalkRotationalVelocity(const float& value)
{
	if (walkRotationalVelocity != value) {
		walkRotationalVelocity = value;
		raisePropertyChanged(propWalkRotationalVelocity);
	}
}

// FUNCTION: WEBSERVICE 0x100a5030
void Humanoid::setWalkToPoint(const G3D::Vector3& value)
{
	if (walkToPoint != value) {
		walkToPoint = value;
		raisePropertyChanged(propWalkToPoint);
	}

	walkMode = walkMode == HAS_PART ? CLICK_TO_MOVE : HAS_POINT;
	walkTimer = walkMode == CLICK_TO_MOVE ? 240 : 0;
}

// FUNCTION: WEBSERVICE 0x100a50c0
void Humanoid::setWalkToPart(PartInstance* value)
{
	if (walkToPart.get() != value) {
		walkToPart = shared_from(value);
		raisePropertyChanged(propWalkToPart);
	}

	walkMode = walkMode == HAS_POINT ? CLICK_TO_MOVE : HAS_PART;
	walkTimer = walkMode == CLICK_TO_MOVE ? 240 : 0;
}

// FUNCTION: WEBSERVICE 0x100a51b0
void Humanoid::setJump(bool value)
{
	if (jump != value) {
		jump = value;
		raisePropertyChanged(propJump);
	}
}

// FUNCTION: WEBSERVICE 0x100a51f0
void Humanoid::setSit(bool value)
{
	if (sit != value) {
		sit = value;
		raisePropertyChanged(propSit);
	}
}

// FUNCTION: WEBSERVICE 0x100a5230
void Humanoid::setTargetPoint(const G3D::Vector3& value)
{
	float tolerance = 1.0f;
	if (getTorso()) {
		float torsoToTarget = Math::taxiCabMagnitude(getTorso()->getCoordinateFrame().translation - value);
		tolerance = std::max(2.0f, torsoToTarget) * 0.01f;
	}

	if (Math::taxiCabMagnitude(value - targetPoint) > tolerance) {
		targetPoint = value;
		raisePropertyChanged(propTargetPoint);
	}
}

// FUNCTION: WEBSERVICE 0x100a5310
void Humanoid::moveTo(const G3D::Vector3& worldPosition, PartInstance* part)
{
	if (part) {
		G3D::Vector3 local = part->getCoordinateFrame().pointToObjectSpace(worldPosition);
		walkToPoint = local + G3D::Vector3::unitX();
		walkToPart.reset();

		setWalkToPart(part);
		setWalkToPoint(local);
	}
}

// FUNCTION: WEBSERVICE 0x100a5400
void Humanoid::setLeftLeg(PartInstance* value)
{
	if (leftLeg.get() != value) {
		leftLeg = shared_from(value);
		raisePropertyChanged(propLeftLeg);
	}
}

// FUNCTION: WEBSERVICE 0x100a54c0
void Humanoid::setRightLeg(PartInstance* value)
{
	if (rightLeg.get() != value) {
		rightLeg = shared_from(value);
		raisePropertyChanged(propRightLeg);
	}
}

// FUNCTION: WEBSERVICE 0x100a5580
void Humanoid::setTorso(PartInstance* value)
{
	if (torso.get() != value) {
		torso = shared_from(value);
		raisePropertyChanged(propTorso);
	}
}

// FUNCTION: WEBSERVICE 0x100a5640
void Humanoid::checkForJointDeath()
{
	if (Primitive* headPrim = getHeadPrimitive()) {
		if (Primitive* torsoPrim = getTorsoPrimitive()) {
			Joint* joint = headPrim->getFirstJoint();
			while (joint) {
				if (joint->otherPrimitive(headPrim) == torsoPrim) {
					hadHeadJoint = true;
					return;
				}

				joint = headPrim->getNextJoint(joint);
			}
		}
	}

	if (hadHeadJoint) {
		setHealth(0.0f);
	}
}

// STUB: WEBSERVICE 0x100a56c0
void Humanoid::onEvent(const RunService* source, Stepped event)
{
	State* state = currentState.get();

	if (state != NULL && getParent() != NULL) {
		State* next = state->onStep(event.step, *static_cast<PVInstance*>(getParent())->getTopPVController());

		if (next != state) {
			currentState.reset(next);
		}
	}
}

// FUNCTION: WEBSERVICE 0x100a5770
void Humanoid::cameraSetWalkOrientation(float rad, bool noAdjust)
{
	if (noAdjust) {
		setWalkRotationalVelocity(0.0f);
	}
	else if (Body* torsoBody = getTorsoBody()) {
		if (Body* root = torsoBody->getRoot()) {
			G3D::Vector3 look = root->getPV().position.lookVector();
			look.y = 0.0f;
			look.unitize();
			float lookAng = Math::radWrap(Math::angleToE0(G3D::Vector2(look.z, look.x)));
			float delta = Math::radWrap(Math::radWrap(rad) - lookAng);
			if (fabs(delta) < 0.1f) {
				delta = 0.0f;
			}
			setWalkRotationalVelocity(delta);
		}
	}
}

// FUNCTION: WEBSERVICE 0x100a58a0
void Humanoid::moveTo2(G3D::Vector3 worldPosition, boost::shared_ptr<Instance> part)
{
	PartInstance* p = dynamic_cast<PartInstance*>(part.get());
	if (!p) {
		throw std::runtime_error("Argument error: A Part is required");
	}

	moveTo(worldPosition, p);
}

// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
bool Humanoid::isControllable() const
{
	return true;
}

// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
bool Humanoid::shouldRender2d() const
{
	return true;
}

// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
bool Humanoid::shouldRender3dAdorn() const
{
	return true;
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::Humanoid, RBX::Instance, RBX::sHumanoid>;
template class RBX::Reflection::
	Described<RBX::Humanoid, RBX::sHumanoid, RBX::FactoryProduct<RBX::Humanoid, RBX::Instance, RBX::sHumanoid> >;
template class RBX::DescribedCreatable<RBX::Humanoid, RBX::Instance, RBX::sHumanoid>;
