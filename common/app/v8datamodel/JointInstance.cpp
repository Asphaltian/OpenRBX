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
	if (value != static_cast<MotorJoint*>(joint)->getMaxVelocity()) {
		static_cast<MotorJoint*>(joint)->setMaxVelocity(value);
		raisePropertyChanged(prop_MaxVelocity);
	}
}

// STUB: WEBSERVICE 0x100d90c0
void Motor::setCurrentAngle(float value)
{
	if (value != static_cast<MotorJoint*>(joint)->getCurrentAngle()) {
		static_cast<MotorJoint*>(joint)->setCurrentAngle(value);
		raisePropertyChanged(prop_CurrentAngle);
	}
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sJointInstance>();
template void RBX::Name::callDoDeclare<RBX::sJointInstance>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sAutoJoint>();
template void RBX::Name::callDoDeclare<RBX::sAutoJoint>();

template class RBX::FactoryProduct<RBX::Glue, RBX::AutoJoint, RBX::sGlue>;
template class RBX::Reflection::
	Described<RBX::Glue, RBX::sGlue, RBX::FactoryProduct<RBX::Glue, RBX::AutoJoint, RBX::sGlue> >;
template class RBX::DescribedCreatable<RBX::Glue, RBX::AutoJoint, RBX::sGlue>;

template class RBX::FactoryProduct<RBX::Rotate, RBX::AutoJoint, RBX::sRotate>;
template class RBX::Reflection::
	Described<RBX::Rotate, RBX::sRotate, RBX::FactoryProduct<RBX::Rotate, RBX::AutoJoint, RBX::sRotate> >;
template class RBX::DescribedCreatable<RBX::Rotate, RBX::AutoJoint, RBX::sRotate>;

template class RBX::FactoryProduct<RBX::RotateP, RBX::AutoJoint, RBX::sRotateP>;
template class RBX::Reflection::
	Described<RBX::RotateP, RBX::sRotateP, RBX::FactoryProduct<RBX::RotateP, RBX::AutoJoint, RBX::sRotateP> >;
template class RBX::DescribedCreatable<RBX::RotateP, RBX::AutoJoint, RBX::sRotateP>;

template class RBX::FactoryProduct<RBX::RotateV, RBX::AutoJoint, RBX::sRotateV>;
template class RBX::Reflection::
	Described<RBX::RotateV, RBX::sRotateV, RBX::FactoryProduct<RBX::RotateV, RBX::AutoJoint, RBX::sRotateV> >;
template class RBX::DescribedCreatable<RBX::RotateV, RBX::AutoJoint, RBX::sRotateV>;

template class RBX::FactoryProduct<RBX::Snap, RBX::AutoJoint, RBX::sSnap>;
template class RBX::Reflection::
	Described<RBX::Snap, RBX::sSnap, RBX::FactoryProduct<RBX::Snap, RBX::AutoJoint, RBX::sSnap> >;
template class RBX::DescribedCreatable<RBX::Snap, RBX::AutoJoint, RBX::sSnap>;

template class RBX::FactoryProduct<RBX::Weld, RBX::AutoJoint, RBX::sWeld>;
template class RBX::Reflection::
	Described<RBX::Weld, RBX::sWeld, RBX::FactoryProduct<RBX::Weld, RBX::AutoJoint, RBX::sWeld> >;
template class RBX::DescribedCreatable<RBX::Weld, RBX::AutoJoint, RBX::sWeld>;
