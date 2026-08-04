#include "v8world/MotorJoint.h"

#include "decomp.h"
#include "util/Math.h"
#include "v8kernel/Body.h"
#include "v8world/Primitive.h"
#include "v8world/World.h"

#include <cmath>
#include <cstddef>

namespace RBX {

// FUNCTION: WEBSERVICE 0x1010a910 FOLDED
Joint::JointType MotorJoint::getJointType() const
{
	return MOTOR_JOINT;
}

// FUNCTION: WEBSERVICE 0x1011fed0
MotorJoint::MotorJoint() : link(new RevoluteLink()), currentAngle(0.0f), maxVelocity(0.0f), desiredAngle(0.0f)
{
}

// FUNCTION: WEBSERVICE 0x1011ff70
RevoluteLink* MotorJoint::resetLink()
{
	int index = getPrimitive(1)->getBody()->getParent() != getPrimitive(0)->getBody();

	link->reset(index == 0 ? jointCoord0 : jointCoord1, index == 1 ? jointCoord0 : jointCoord1);

	link->setJointAngle(currentAngle);

	return link;
}

// FUNCTION: WEBSERVICE 0x1011ffd0
bool MotorJoint::isAligned()
{
	CoordinateFrame coord0 = getPrimitive(0)->getCoordinateFrame() * jointCoord0;
	CoordinateFrame coord1 = getPrimitive(1)->getCoordinateFrame() * jointCoord1;

	return Math::fuzzyEq(coord0.translation, coord1.translation, 1.0e-5f) &&
		   Math::fuzzyEq(coord0.rotation.getColumn(2), coord1.rotation.getColumn(2), 1.0e-5f);
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

// FUNCTION: WEBSERVICE 0x101200d0
MotorJoint::~MotorJoint()
{
	delete link;
}

// FUNCTION: WEBSERVICE 0x10120140
void MotorJoint::stepUi(int frameCount)
{
	float maxStep = fabs(maxVelocity);

	float delta = desiredAngle - currentAngle;

	if (fabs(delta) < maxStep) {
		setCurrentAngle(desiredAngle);
	}
	else if (delta > 0.0f) {
		setCurrentAngle(currentAngle + maxStep);
	}
	else {
		setCurrentAngle(currentAngle - maxStep);
	}
}

} // namespace RBX
