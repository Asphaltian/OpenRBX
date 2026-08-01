#include "v8world/MotorJoint.h"

#include "decomp.h"

#include <cstddef>

namespace RBX {

// FUNCTION: WEBSERVICE 0x1010a910 FOLDED
Joint::JointType MotorJoint::getJointType() const
{
	return MOTOR_JOINT;
}

// STUB: WEBSERVICE 0x1011ff70
RevoluteLink* MotorJoint::resetLink()
{
	STUB(0x1011ff70);
	return NULL;
}

} // namespace RBX
