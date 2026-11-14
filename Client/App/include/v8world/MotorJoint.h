#ifndef V8WORLD_MOTORJOINT_H
#define V8WORLD_MOTORJOINT_H

#include "placeholder.h"
#include "v8kernel/Link.h"
#include "v8world/Joint.h"

namespace RBX {

class MotorJoint : public Joint
{
public:
	MotorJoint();

	virtual ~MotorJoint();

	static bool isMotorJoint(Edge* e) { return isJoint(e) && static_cast<Joint*>(e)->getJointType() == MOTOR_JOINT; }
	unsigned int hashCode() const;

private:
	virtual JointType getJointType() const;

	virtual bool isAligned();

	int getParentId() const;
	void setJointAngle(float);

public:
	virtual void stepUi(int frameCount);

	RevoluteLink* resetLink();

	float getCurrentAngle() const { return currentAngle; }

	void setCurrentAngle(float value);
	G3D::CoordinateFrame getMeInOther(Primitive*);

private:
	RevoluteLink* link;
	float currentAngle;

public:
	float maxVelocity;
	float desiredAngle;
};

} // namespace RBX

#endif // V8WORLD_MOTORJOINT_H
