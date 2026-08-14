#ifndef V8WORLD_JOINT_H
#define V8WORLD_JOINT_H

#include "decomp.h"
#include "util/NormalId.h"
#include "v8world/Primitive.h"

#include <G3D/CoordinateFrame.h>

namespace RBX {

class IJointOwner;

using G3D::CoordinateFrame;

// VTABLE: WEBSERVICE 0x10247fb0
// SIZE 0x88
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
	virtual ~Joint(); // vtable+0x00

	// SYNTHETIC: WEBSERVICE 0x1011ea60
	// RBX::Joint::`scalar deleting destructor'

protected:
	// FUNCTION: WEBSERVICE 0x10059d20 FOLDED
	virtual EdgeType getEdgeType() const { return JOINT; } // vtable+0x0c

public:
	virtual void setPrimitive(int index, Primitive* primitive); // vtable+0x10

	static bool isJoint(Edge* edge) { return edge->getEdgeType() == JOINT; }

	static bool isAutoJoinJoint(Joint* joint)
	{
		JointType jointType = joint->getJointType();

		return jointType != FREE_JOINT && jointType != ANCHOR_JOINT;
	}

	// FUNCTION: WEBSERVICE 0x10059d20 FOLDED
	virtual JointType getJointType() const { return NO_JOINT; } // vtable+0x14
	// FUNCTION: WEBSERVICE 0x100eb790 FOLDED
	virtual bool isBreakable() const { return false; } // vtable+0x18

	// FUNCTION: WEBSERVICE 0x100eb790 FOLDED
	virtual bool isBroken() const { return false; }                        // vtable+0x1c
	virtual bool joinsFace(Primitive* primitive, NormalId normalId) const; // vtable+0x20

	// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
	virtual bool isAligned() { return true; } // vtable+0x24

	virtual CoordinateFrame align(Primitive* prim0, Primitive* prim1); // vtable+0x28

	// FUNCTION: WEBSERVICE 0x100eb790 FOLDED
	virtual bool canStepUi() const { return false; } // vtable+0x2c

	virtual void stepUi(int frameCount) {} // vtable+0x30

	void setJointOwner(IJointOwner* value);

	void setJointCoord(int index, const CoordinateFrame& value);

	const CoordinateFrame& getJointCoord(int index) const { return index == 0 ? jointCoord0 : jointCoord1; }

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

	IJointOwner* jointOwner; // 0x20

protected:
	static bool canBuildJointLoose(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1);
	static bool canBuildJointTight(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1);

	bool active; // 0x24

protected:
	CoordinateFrame jointCoord0; // 0x28
	CoordinateFrame jointCoord1; // 0x58
};

DECOMP_SIZE_ASSERT(Joint, 0x88)

// VTABLE: WEBSERVICE 0x10247e6c
// SIZE 0x88
class AnchorJoint : public Joint
{
public:
	AnchorJoint(Primitive* p) : Joint(p, NULL, CoordinateFrame(), CoordinateFrame()) {}

	static bool isAnchorJoint(const Joint* joint) { return joint->getJointType() == ANCHOR_JOINT; }

private:
	// FUNCTION: WEBSERVICE 0x1011b580 FOLDED
	virtual JointType getJointType() const { return ANCHOR_JOINT; } // vtable+0x14
};

DECOMP_SIZE_ASSERT(AnchorJoint, 0x88)

// VTABLE: WEBSERVICE 0x10247ea4
// SIZE 0x88
class FreeJoint : public Joint
{
public:
	FreeJoint(Primitive* p) : Joint(p, NULL, CoordinateFrame(), CoordinateFrame()) {}

	// SYNTHETIC: WEBSERVICE 0x1011b590
	// RBX::FreeJoint::`scalar deleting destructor'

	static bool isFreeJoint(const Joint* joint) { return joint->getJointType() == FREE_JOINT; }

private:
	// FUNCTION: WEBSERVICE 0x1011b5b0
	virtual JointType getJointType() const { return FREE_JOINT; } // vtable+0x14
};

DECOMP_SIZE_ASSERT(FreeJoint, 0x88)

} // namespace RBX

#endif // V8WORLD_JOINT_H
