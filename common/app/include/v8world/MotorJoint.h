#ifndef V8WORLD_MOTORJOINT_H
#define V8WORLD_MOTORJOINT_H

#include "v8world/Joint.h"

namespace RBX {

class MotorJoint : public Joint
{
public:
	// FUNCTION: WEBSERVICE 0x1009add0
	static bool isMotorJoint(Edge* edge)
	{
		return isJoint(edge) && static_cast<Joint*>(edge)->getJointType() == MOTOR_JOINT;
	}
};

} // namespace RBX

#endif // V8WORLD_MOTORJOINT_H
