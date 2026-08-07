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

} // namespace RBX

template class RBX::FactoryProduct<RBX::Humanoid, RBX::Instance, RBX::sHumanoid>;
template class RBX::Reflection::
	Described<RBX::Humanoid, RBX::sHumanoid, RBX::FactoryProduct<RBX::Humanoid, RBX::Instance, RBX::sHumanoid> >;
template class RBX::DescribedCreatable<RBX::Humanoid, RBX::Instance, RBX::sHumanoid>;
