#include "v8datamodel/JointInstance.h"

#include "reflection/property.h"
#include "util/Name.h"

namespace RBX {

const char sAutoJoint[] = "AutoJoint";

const char sJointInstance[] = "JointInstance";

const char sMotor[] = "Motor";

const char sRotateV[] = "RotateV";

const char sRotateP[] = "RotateP";

const char sRotate[] = "Rotate";

const char sGlue[] = "Glue";

const char sWeld[] = "Weld";

const char sSnap[] = "Snap";

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

template const RBX::Name& RBX::Name::doDeclare<RBX::sJointInstance>();
template void RBX::Name::callDoDeclare<RBX::sJointInstance>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sAutoJoint>();
template void RBX::Name::callDoDeclare<RBX::sAutoJoint>();
