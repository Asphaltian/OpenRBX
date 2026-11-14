#ifndef V8DATAMODEL_JOINTINSTANCE_H
#define V8DATAMODEL_JOINTINSTANCE_H

#include "placeholder.h"
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
	Joint* joint;

	virtual bool shouldRender3dAdorn() const;
	virtual void render3dAdorn(Adorn* adorn);
};

class AutoJoint : public DescribedNonCreatable<AutoJoint, JointInstance, sAutoJoint>
{
private:
	boost::shared_ptr<PartInstance> part[2];

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

class Glue : public DescribedCreatable<Glue, AutoJoint, sGlue>
{
public:
	Glue();
	Glue(Joint* joint);
};

class Rotate : public DescribedCreatable<Rotate, AutoJoint, sRotate>
{
public:
	Rotate();
	Rotate(Joint* joint);
};

class RotateP : public DescribedCreatable<RotateP, AutoJoint, sRotateP>
{
public:
	RotateP();
	RotateP(Joint* joint);
};

class RotateV : public DescribedCreatable<RotateV, AutoJoint, sRotateV>
{
public:
	RotateV();
	RotateV(Joint* joint);
};

class Snap : public DescribedCreatable<Snap, AutoJoint, sSnap>
{
public:
	Snap();
	Snap(Joint* joint);
};

class Weld : public DescribedCreatable<Weld, AutoJoint, sWeld>
{
public:
	Weld();
	Weld(Joint* joint);
};

} // namespace RBX

#endif // V8DATAMODEL_JOINTINSTANCE_H
