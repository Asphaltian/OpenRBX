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

// SIZE 0x10c
class JointInstance : public DescribedNonCreatable<JointInstance, Instance, sJointInstance>
{
protected:
	undefined m_unk0x0f8[0x108 - 0x0f8]; // 0x0f8
	Joint* joint;                        // 0x108
};

DECOMP_SIZE_ASSERT(JointInstance, 0x10c)

// SIZE 0x11c
class AutoJoint : public DescribedNonCreatable<AutoJoint, JointInstance, sAutoJoint>
{
private:
	undefined m_unk0x10c[0x11c - 0x10c]; // 0x10c
};

DECOMP_SIZE_ASSERT(AutoJoint, 0x11c)

// SIZE 0x11c
class Motor : public DescribedCreatable<Motor, AutoJoint, sMotor>
{
public:
	void setMaxVelocity(float value);
	void setCurrentAngle(float value);
};

DECOMP_SIZE_ASSERT(Motor, 0x11c)

// SIZE 0x11c
class Glue : public DescribedCreatable<Glue, AutoJoint, sGlue>
{
};

DECOMP_SIZE_ASSERT(Glue, 0x11c)

// SIZE 0x11c
class Rotate : public DescribedCreatable<Rotate, AutoJoint, sRotate>
{
};

DECOMP_SIZE_ASSERT(Rotate, 0x11c)

// SIZE 0x11c
class RotateP : public DescribedCreatable<RotateP, AutoJoint, sRotateP>
{
};

DECOMP_SIZE_ASSERT(RotateP, 0x11c)

// SIZE 0x11c
class RotateV : public DescribedCreatable<RotateV, AutoJoint, sRotateV>
{
};

DECOMP_SIZE_ASSERT(RotateV, 0x11c)

// SIZE 0x11c
class Snap : public DescribedCreatable<Snap, AutoJoint, sSnap>
{
};

DECOMP_SIZE_ASSERT(Snap, 0x11c)

// SIZE 0x11c
class Weld : public DescribedCreatable<Weld, AutoJoint, sWeld>
{
};

DECOMP_SIZE_ASSERT(Weld, 0x11c)

// clang-format off
// STUB: WEBSERVICE 0x100d7df0
// RBX::JointInstance::~JointInstance
// STUB: WEBSERVICE 0x100d8110
// RBX::AutoJoint::~AutoJoint
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_JOINTINSTANCE_H
