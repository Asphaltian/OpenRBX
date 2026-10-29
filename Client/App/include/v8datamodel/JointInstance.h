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

class PartInstance;

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
	JointInstance(Joint* joint);

	virtual ~JointInstance();
	Joint::JointType getJointType() const;

protected:
	Joint* joint; // 0x108

	virtual bool shouldRender3dAdorn() const;
	virtual void render3dAdorn(Adorn* adorn);
};

DECOMP_SIZE_ASSERT(JointInstance, 0x10c)

// SIZE 0x11c
class AutoJoint : public DescribedNonCreatable<AutoJoint, JointInstance, sAutoJoint>
{
private:
	boost::shared_ptr<PartInstance> part[2]; // 0x10c

	void setPart(int i, PartInstance* value);
	void setPartNull(int i);
	void setPrimitiveFromPart(int i);

	virtual bool askSetParent(const Instance* instance) const;
	virtual void onAncestorChanged(const AncestorChanged& event);

	World* computeWorld();

public:
	AutoJoint() {}
	AutoJoint(Joint* _joint);

	virtual ~AutoJoint();

	PartInstance* getPart0() const;
	PartInstance* getPart1() const;
	void setPart0(PartInstance* value);
	void setPart1(PartInstance* value);

	const CoordinateFrame& getC0() const;
	const CoordinateFrame& getC1() const;
	void setC0(const CoordinateFrame& value);
	void setC1(const CoordinateFrame& value);
};

DECOMP_SIZE_ASSERT(AutoJoint, 0x11c)

// SIZE 0x11c
class Motor : public DescribedCreatable<Motor, AutoJoint, sMotor>
{
private:
	MotorJoint* getMotorJoint() { return static_cast<MotorJoint*>(joint); }
	void renderJointCylinder(Adorn*, int);
	const MotorJoint* getMotorJoint() const { return static_cast<const MotorJoint*>(joint); }

public:
	Motor();
	Motor(Joint* joint);

	float getMaxVelocity() const;
	void setMaxVelocity(float value);
	float getDesiredAngle() const;
	void setDesiredAngle(float value);
	float getCurrentAngle() const;
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
// TEMPLATE: WEBSERVICE 0x100d8a40
// RBX::DescribedNonCreatable<RBX::JointInstance,RBX::Instance,&RBX::sJointInstance>::DescribedNonCreatable<RBX::JointInstance,RBX::Instance,&RBX::sJointInstance>
// TEMPLATE: WEBSERVICE 0x100d8c40
// RBX::NonFactoryProduct<RBX::JointInstance,&RBX::sAutoJoint>::NonFactoryProduct<RBX::JointInstance,&RBX::sAutoJoint><RBX::Joint *>
// TEMPLATE: WEBSERVICE 0x100d8cf0
// RBX::DescribedNonCreatable<RBX::AutoJoint,RBX::JointInstance,&RBX::sAutoJoint>::DescribedNonCreatable<RBX::AutoJoint,RBX::JointInstance,&RBX::sAutoJoint><RBX::Joint *>
// FUNCTION: WEBSERVICE 0x1021c470
// `dynamic initializer for 'prop_Part0''
// FUNCTION: WEBSERVICE 0x1021c4b0
// `dynamic initializer for 'prop_Part1''
// FUNCTION: WEBSERVICE 0x1021c4f0
// `dynamic initializer for 'prop_C1''
// FUNCTION: WEBSERVICE 0x1021c530
// `dynamic initializer for 'prop_C0''
// FUNCTION: WEBSERVICE 0x1021c570
// `dynamic initializer for 'prop_MaxVelocity''
// FUNCTION: WEBSERVICE 0x1021c5b0
// `dynamic initializer for 'prop_DesiredAngle''
// FUNCTION: WEBSERVICE 0x1021c5f0
// `dynamic initializer for 'prop_CurrentAngle''
// SYNTHETIC: WEBSERVICE 0x10223d90
// `dynamic atexit destructor for 'prop_Part1''
// SYNTHETIC: WEBSERVICE 0x10223db0
// `dynamic atexit destructor for 'prop_Part0''
// SYNTHETIC: WEBSERVICE 0x10223dd0
// `dynamic atexit destructor for 'prop_CurrentAngle''
// SYNTHETIC: WEBSERVICE 0x10223df0
// `dynamic atexit destructor for 'prop_C0''
// SYNTHETIC: WEBSERVICE 0x10223e10
// `dynamic atexit destructor for 'prop_C1''
// SYNTHETIC: WEBSERVICE 0x10223e30
// `dynamic atexit destructor for 'prop_DesiredAngle''
// SYNTHETIC: WEBSERVICE 0x10223e50
// `dynamic atexit destructor for 'prop_MaxVelocity''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_JOINTINSTANCE_H
