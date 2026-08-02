#ifndef V8DATAMODEL_JOINTINSTANCE_H
#define V8DATAMODEL_JOINTINSTANCE_H

#include "decomp.h"
#include "v8tree/Instance.h"
#include "v8world/MotorJoint.h"

namespace RBX {

extern const char sAutoJoint[];

extern const char sJointInstance[];

extern const char sMotor[];

extern const char sRotateV[];

extern const char sRotateP[];

extern const char sRotate[];

extern const char sGlue[];

extern const char sWeld[];

extern const char sSnap[];

// SIZE 0x11c
class Motor : public Instance
{
public:
	void setMaxVelocity(float value);
	void setCurrentAngle(float value);

private:
	undefined m_unk0x0f8[0x108 - 0x0f8]; // 0x0f8
	MotorJoint* joint;                   // 0x108
	undefined m_unk0x10c[0x11c - 0x10c]; // 0x10c
};

DECOMP_SIZE_ASSERT(Motor, 0x11c)

} // namespace RBX

#endif // V8DATAMODEL_JOINTINSTANCE_H
