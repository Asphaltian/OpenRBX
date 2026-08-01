#include "v8datamodel/JointInstance.h"

#include "reflection/Property.h"

namespace RBX {

static Reflection::PropDescriptor<Motor, float> prop_MaxVelocity;
static Reflection::PropDescriptor<Motor, float> prop_CurrentAngle;

// STUB: WEBSERVICE 0x100d9020
void Motor::setMaxVelocity(float value)
{
	if (value != joint->getMaxVelocity()) {
		joint->setMaxVelocity(value);
		raisePropertyChanged(prop_MaxVelocity);
	}
}

// STUB: WEBSERVICE 0x100d90c0
void Motor::setCurrentAngle(float value)
{
	if (value != joint->getCurrentAngle()) {
		joint->setCurrentAngle(value);
		raisePropertyChanged(prop_CurrentAngle);
	}
}

} // namespace RBX
