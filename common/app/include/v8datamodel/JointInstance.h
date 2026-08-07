#ifndef V8DATAMODEL_JOINTINSTANCE_H
#define V8DATAMODEL_JOINTINSTANCE_H

#include "decomp.h"
#include "util/IRenderable.h"
#include "v8tree/Instance.h"
#include "v8world/Edge.h"
#include "v8world/GlueJoint.h"
#include "v8world/MotorJoint.h"
#include "v8world/RotateJoint.h"
#include "v8world/SnapJoint.h"
#include "v8world/WeldJoint.h"

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
class JointInstance : public DescribedNonCreatable<JointInstance, Instance, sJointInstance>,
					  public IRenderable,
					  public IJointOwner
{
public:
	JointInstance() {}
	JointInstance(Joint* joint) : joint(joint) {}

protected:
	Joint* joint; // 0x108
};

DECOMP_SIZE_ASSERT(JointInstance, 0x10c)

// SIZE 0x11c
class AutoJoint : public DescribedNonCreatable<AutoJoint, JointInstance, sAutoJoint>
{
public:
	AutoJoint() {}
	AutoJoint(Joint* joint) : DescribedNonCreatable<AutoJoint, JointInstance, sAutoJoint>(joint) {}

private:
	undefined m_unk0x10c[0x11c - 0x10c]; // 0x10c
};

DECOMP_SIZE_ASSERT(AutoJoint, 0x11c)

// SIZE 0x11c
class Motor : public DescribedCreatable<Motor, AutoJoint, sMotor>
{
public:
	Motor();
	Motor(Joint* joint);

	float getMaxVelocity() const;
	float getCurrentAngle() const;

	void setMaxVelocity(float value);
	void setCurrentAngle(float value);
};

DECOMP_SIZE_ASSERT(Motor, 0x11c)

// SIZE 0x11c
class Glue : public DescribedCreatable<Glue, AutoJoint, sGlue>
{
public:
	Glue();
	Glue(Joint* joint);
};

DECOMP_SIZE_ASSERT(Glue, 0x11c)

// SIZE 0x11c
class Rotate : public DescribedCreatable<Rotate, AutoJoint, sRotate>
{
public:
	Rotate();
	Rotate(Joint* joint);
};

DECOMP_SIZE_ASSERT(Rotate, 0x11c)

// SIZE 0x11c
class RotateP : public DescribedCreatable<RotateP, AutoJoint, sRotateP>
{
public:
	RotateP();
	RotateP(Joint* joint);
};

DECOMP_SIZE_ASSERT(RotateP, 0x11c)

// SIZE 0x11c
class RotateV : public DescribedCreatable<RotateV, AutoJoint, sRotateV>
{
public:
	RotateV();
	RotateV(Joint* joint);
};

DECOMP_SIZE_ASSERT(RotateV, 0x11c)

// SIZE 0x11c
class Snap : public DescribedCreatable<Snap, AutoJoint, sSnap>
{
public:
	Snap();
	Snap(Joint* joint);
};

DECOMP_SIZE_ASSERT(Snap, 0x11c)

// SIZE 0x11c
class Weld : public DescribedCreatable<Weld, AutoJoint, sWeld>
{
public:
	Weld();
	Weld(Joint* joint);
};

DECOMP_SIZE_ASSERT(Weld, 0x11c)

// clang-format off
// STUB: WEBSERVICE 0x100d7df0
// RBX::JointInstance::~JointInstance
// STUB: WEBSERVICE 0x100d8110
// RBX::AutoJoint::~AutoJoint
// clang-format on

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021c570
// `dynamic initializer for 'prop_MaxVelocity''
// SYNTHETIC: WEBSERVICE 0x1021c5f0
// `dynamic initializer for 'prop_CurrentAngle''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_JOINTINSTANCE_H
