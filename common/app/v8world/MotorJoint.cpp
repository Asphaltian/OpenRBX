#include "v8world/MotorJoint.h"

#include "decomp.h"
#include "v8kernel/Body.h"
#include "v8world/Primitive.h"
#include "v8world/World.h"

#include <cstddef>

namespace RBX {

// FUNCTION: WEBSERVICE 0x1010a910 FOLDED
Joint::JointType MotorJoint::getJointType() const
{
	return MOTOR_JOINT;
}

// FUNCTION: WEBSERVICE 0x1011ff70
RevoluteLink* MotorJoint::resetLink()
{
	int index = getPrimitive(1)->getBody()->getParent() != getPrimitive(0)->getBody();

	link->reset(index == 0 ? jointCoord0 : jointCoord1, index == 1 ? jointCoord0 : jointCoord1);

	link->setJointAngle(currentAngle);

	return link;
}

// FUNCTION: WEBSERVICE 0x10120070
void MotorJoint::setCurrentAngle(float value)
{
	if (currentAngle != value) {
		currentAngle = value;

		World* world = getWorld();

		if (world != NULL) {
			world->onMotorAngleChanged(this);
			link->setJointAngle(value);
		}
	}
}

} // namespace RBX
