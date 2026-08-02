#include "humanoid/Humanoid.h"

#include "reflection/Property.h"

namespace RBX {

char sHumanoid[] = "Humanoid";

using namespace Reflection;

static PropDescriptor<Humanoid, float> propMaxHealth;
static PropDescriptor<Humanoid, float> propWalkRotationalVelocity;
static PropDescriptor<Humanoid, bool> propJump;
static PropDescriptor<Humanoid, bool> propSit;

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
