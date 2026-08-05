#ifndef V8WORLD_MOTORJOINT_H
#define V8WORLD_MOTORJOINT_H

#include "decomp.h"
#include "v8kernel/Link.h"
#include "v8world/Joint.h"

namespace RBX {

// VTABLE: WEBSERVICE 0x102480bc
// SIZE 0x98
class MotorJoint : public Joint
{
public:
	MotorJoint();

	virtual ~MotorJoint(); // vtable+0x00

	// SYNTHETIC: WEBSERVICE 0x101201b0
	// RBX::MotorJoint::`scalar deleting destructor'

	// FUNCTION: WEBSERVICE 0x1009add0
	static bool isMotorJoint(Edge* e) { return isJoint(e) && static_cast<Joint*>(e)->getJointType() == MOTOR_JOINT; }

	virtual JointType getJointType() const; // vtable+0x14

	virtual bool isAligned(); // vtable+0x24

	virtual void stepUi(int frameCount); // vtable+0x30

	RevoluteLink* resetLink();

	float getCurrentAngle() const { return currentAngle; }
	float getMaxVelocity() const { return maxVelocity; }

	void setCurrentAngle(float value);
	void setMaxVelocity(float value) { maxVelocity = value; }

	float getDesiredAngle() const { return desiredAngle; }
	void setDesiredAngle(float value) { desiredAngle = value; }

private:
	RevoluteLink* link; // 0x88
	float currentAngle; // 0x8c
	float maxVelocity;  // 0x90
	float desiredAngle; // 0x94
};

DECOMP_SIZE_ASSERT(MotorJoint, 0x98)

} // namespace RBX

#endif // V8WORLD_MOTORJOINT_H
