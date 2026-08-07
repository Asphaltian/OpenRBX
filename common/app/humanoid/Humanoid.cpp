#include "humanoid/Humanoid.h"

#include "reflection/property.h"
#include "v8kernel/Body.h"

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

// STUB: WEBSERVICE 0x100a0990
ContactManager* Humanoid::getContactManager()
{
	STUB(0x100a0990);
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
	STUB(0x100a1100);
}

// STUB: WEBSERVICE 0x100a1480
float Humanoid::getIntendedRotationAboutYAxis()
{
	STUB(0x100a1480);
	return 0;
}

// STUB: WEBSERVICE 0x100a1b00
void Humanoid::render3dAdorn(Adorn* adorn)
{
	STUB(0x100a1b00);
}

// STUB: WEBSERVICE 0x100a2110
DECOMP_NOINLINE PartInstance* Humanoid::getTorso() const
{
	STUB(0x100a2110);
	return NULL;
}

// STUB: WEBSERVICE 0x100a2210
DECOMP_NOINLINE PartInstance* Humanoid::getLeftLeg() const
{
	STUB(0x100a2210);
	return NULL;
}

// STUB: WEBSERVICE 0x100a2310
DECOMP_NOINLINE PartInstance* Humanoid::getRightLeg() const
{
	STUB(0x100a2310);
	return NULL;
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

// STUB: WEBSERVICE 0x100a27c0
const CoordinateFrame Humanoid::getLocation() const
{
	STUB(0x100a27c0);
	return CoordinateFrame();
}

// STUB: WEBSERVICE 0x100a30d0
G3D::Vector3 Humanoid::getIntendedMovementVector()
{
	STUB(0x100a30d0);
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
