#ifndef V8WORLD_JOINT_H
#define V8WORLD_JOINT_H

#include "placeholder.h"
#include "util/NormalId.h"
#include "v8world/Primitive.h"

#include <G3D/CoordinateFrame.h>

namespace RBX {

class IJointOwner;

using G3D::CoordinateFrame;

class Joint : public Edge
{
public:
	enum JointType
	{
		NO_JOINT = 0,
		ROTATE_JOINT = 1,
		ROTATE_P_JOINT = 2,
		ROTATE_V_JOINT = 3,
		GLUE_JOINT = 4,
		ANCHOR_JOINT = 5,
		WELD_JOINT = 6,
		SNAP_JOINT = 7,
		MOTOR_JOINT = 8,
		FREE_JOINT = 9,
	};

protected:
	Joint();
	Joint(Primitive* prim0, Primitive* prim1, const CoordinateFrame& coord0, const CoordinateFrame& coord1);

public:
	virtual ~Joint();

protected:
	virtual EdgeType getEdgeType() const { return JOINT; }

public:
	virtual void setPrimitive(int index, Primitive* primitive);

	static bool isJoint(Edge* edge) { return edge->getEdgeType() == JOINT; }

	static bool isAssemblyJoint(Edge* edge)
	{
		return isJoint(edge) && static_cast<Joint*>(edge)->getJointType() >= ANCHOR_JOINT;
	}
	static bool isGroundJoint(Joint*);

	static bool isAutoJoinJoint(Joint* joint)
	{
		JointType jointType = joint->getJointType();

		return jointType != FREE_JOINT && jointType != ANCHOR_JOINT;
	}

	virtual JointType getJointType() const { return NO_JOINT; }
	static JointType getJointType(Edge*);
	virtual bool isBreakable() const { return false; }

	virtual bool isBroken() const { return false; }
	virtual bool joinsFace(Primitive* primitive, NormalId normalId) const;

	virtual bool isAligned() { return true; }

	virtual CoordinateFrame align(Primitive* prim0, Primitive* prim1);

	virtual bool canStepUi() const { return false; }

	virtual void stepUi(int frameCount) {}

	void setJointOwner(IJointOwner* value);
	IJointOwner* getJointOwner() const;

	void setJointCoord(int index, const CoordinateFrame& value);

	const CoordinateFrame& getJointCoord(int index) const { return index == 0 ? jointCoord0 : jointCoord1; }
	NormalId getNormalId(int) const;

	bool getActive() const { return active; }
	void setActive(bool value) { active = value; }

private:
	static bool canBuildJoint(
		Primitive* prim0,
		Primitive* prim1,
		NormalId normalId0,
		NormalId normalId1,
		float maxUnaligned,
		float maxOverlapOrGap
	);

	IJointOwner* jointOwner;

protected:
	static bool canBuildJointLoose(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1);
	static bool canBuildJointTight(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1);

	bool active;

protected:
	CoordinateFrame jointCoord0;
	CoordinateFrame jointCoord1;
};

class AnchorJoint : public Joint
{
public:
	AnchorJoint(Primitive* p) : Joint(p, NULL, CoordinateFrame(), CoordinateFrame()) {}

	static bool isAnchorJoint(const Joint* joint) { return joint->getJointType() == ANCHOR_JOINT; }

private:
	virtual JointType getJointType() const { return ANCHOR_JOINT; }
};

class FreeJoint : public Joint
{
public:
	FreeJoint(Primitive* p) : Joint(p, NULL, CoordinateFrame(), CoordinateFrame()) {}

	static bool isFreeJoint(const Joint* joint) { return joint->getJointType() == FREE_JOINT; }

private:
	virtual JointType getJointType() const { return FREE_JOINT; }
};

} // namespace RBX

#endif // V8WORLD_JOINT_H
