#include "v8world/MotorJoint.h"

#include "placeholder.h"
#include "util/Math.h"
#include "v8kernel/Body.h"
#include "v8world/Primitive.h"
#include "v8world/World.h"

#include <cmath>
#include <cstddef>

namespace RBX {

Joint::JointType MotorJoint::getJointType() const
{
	return MOTOR_JOINT;
}

MotorJoint::MotorJoint() : link(new RevoluteLink()), currentAngle(0.0f), maxVelocity(0.0f), desiredAngle(0.0f)
{
}

RevoluteLink* MotorJoint::resetLink()
{
	int index = getPrimitive(1)->getBody()->getParent() != getPrimitive(0)->getBody();

	link->reset(index == 0 ? jointCoord0 : jointCoord1, index == 1 ? jointCoord0 : jointCoord1);

	link->setJointAngle(currentAngle);

	return link;
}

bool MotorJoint::isAligned()
{
	CoordinateFrame baseWorld = getPrimitive(0)->getCoordinateFrame() * jointCoord0;
	CoordinateFrame rotorWorld = getPrimitive(1)->getCoordinateFrame() * jointCoord1;

	return Math::fuzzyEq(baseWorld.translation, rotorWorld.translation, 1.0e-5f) &&
		   Math::fuzzyEq(baseWorld.rotation.getColumn(2), rotorWorld.rotation.getColumn(2), 1.0e-5f);
}

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

MotorJoint::~MotorJoint()
{
	delete link;
}

void MotorJoint::stepUi(int uiStepId)
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
