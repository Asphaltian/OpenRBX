#include "humanoid/Humanoid.h"

#include "reflection/property.h"
#include "v8datamodel/Workspace.h"
#include "v8kernel/Body.h"
#include "v8world/World.h"

namespace RBX {

char sHumanoid[] = "Humanoid";

using namespace Reflection;

static PropDescriptor<Humanoid, float> propMaxHealth(
	"MaxHealth",
	"Game",
	&RBX::Humanoid::getMaxHealth,
	&RBX::Humanoid::setMaxHealth
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

static PropDescriptor<Humanoid, bool> propSit(
	"Sit",
	"Control",
	&RBX::Humanoid::getSit,
	&RBX::Humanoid::setSit,
	Reflection::PropertyDescriptor::STREAMING
);

// FUNCTION: WEBSERVICE 0x100a0990
ContactManager* Humanoid::getContactManager()
{
	World* world = Workspace::getWorldIfInWorkspace(this);

	if (world != NULL) {
		return world->getContactManager();
	}

	return NULL;
}

// STUB: WEBSERVICE 0x100a0d40
void Humanoid::computeForce(float dt, bool throttling)
{
	STUB(0x100a0d40);
}

// STUB: WEBSERVICE 0x100a1100
Humanoid::~Humanoid()
{
}

// FUNCTION: WEBSERVICE 0x100a1480
float Humanoid::getIntendedRotationAboutYAxis()
{
	if (currentState.get() != NULL) {
		return currentState->getIntendedRotationAboutYAxis();
	}

	return 0;
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

// FUNCTION: WEBSERVICE 0x100a27c0
const CoordinateFrame Humanoid::getLocation() const
{
	return getHead() != NULL ? getHead()->getCoordinateFrame() : CoordinateFrame();
}

// STUB: WEBSERVICE 0x100a2f70
DECOMP_NOINLINE G3D::Vector3 Humanoid::getIntendedMovementVector(bool ignoreSit)
{
	STUB(0x100a2f70);
	return G3D::Vector3::zero();
}

// STUB: WEBSERVICE 0x100a30d0
G3D::Vector3 Humanoid::getIntendedMovementVector()
{
	return getIntendedMovementVector(false);
}

// STUB: WEBSERVICE 0x100a30f0
G3D::Vector3 Humanoid::updateWalkDirection()
{
	STUB(0x100a30f0);
	return G3D::Vector3::zero();
}

// STUB: WEBSERVICE 0x100a31b0
void Humanoid::tellCameraNear(float distance)
{
	STUB(0x100a31b0);
}

// STUB: WEBSERVICE 0x100a4290
Humanoid::Humanoid()
{
	STUB(0x100a4290);
}

// STUB: WEBSERVICE 0x100a4960
void Humanoid::render2d(Adorn* adorn)
{
	STUB(0x100a4960);
}

// STUB: WEBSERVICE 0x100a4bf0
void Humanoid::getIgnorePrims(std::vector<const Primitive*>& prims)
{
	STUB(0x100a4bf0);
}

// FUNCTION: WEBSERVICE 0x100a4e80
void Humanoid::setMaxHealth(float value)
{
	if (maxHealth != value) {
		maxHealth = value;
		raisePropertyChanged(propMaxHealth);
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

// STUB: WEBSERVICE 0x100a56c0
void Humanoid::onEvent(const RunService* source, Stepped event)
{
	STUB(0x100a56c0);
}

// STUB: WEBSERVICE 0x100a5770
void Humanoid::cameraSetWalkOrientation(float angle, bool value)
{
	STUB(0x100a5770);
}

bool Humanoid::isControllable() const
{
	STUB(0x100e6150);
	return false;
}

bool Humanoid::shouldRender2d() const
{
	STUB(0x100e6151);
	return false;
}

bool Humanoid::shouldRender3dAdorn() const
{
	STUB(0x100e6152);
	return false;
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::Humanoid, RBX::Instance, RBX::sHumanoid>;
template class RBX::Reflection::
	Described<RBX::Humanoid, RBX::sHumanoid, RBX::FactoryProduct<RBX::Humanoid, RBX::Instance, RBX::sHumanoid> >;
template class RBX::DescribedCreatable<RBX::Humanoid, RBX::Instance, RBX::sHumanoid>;
