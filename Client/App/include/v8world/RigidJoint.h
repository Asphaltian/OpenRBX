#ifndef V8WORLD_RIGIDJOINT_H
#define V8WORLD_RIGIDJOINT_H

#include "placeholder.h"
#include "v8world/Joint.h"

namespace RBX {

class RigidJoint : public Joint
{
public:
	virtual ~RigidJoint() {}

	static bool isRigidJoint(Edge* e) { return isJoint(e) && jointIsRigid(static_cast<Joint*>(e)); }

	virtual bool isAligned();
	virtual CoordinateFrame align(Primitive* prim0, Primitive* prim1);

	CoordinateFrame getChildInParent(Primitive* parent, Primitive* child);

private:
	static bool jointIsRigid(Joint* joint)
	{
		JointType jointType = joint->getJointType();
		return jointType == WELD_JOINT || jointType == SNAP_JOINT;
	}

public:
	RigidJoint() {}

	RigidJoint(Primitive* prim0, Primitive* prim1, const CoordinateFrame& coord0, const CoordinateFrame& coord1)
		: Joint(prim0, prim1, coord0, coord1)
	{
	}

protected:
	static void faceIdToCoords(
		Primitive* prim0,
		Primitive* prim1,
		NormalId normalId0,
		NormalId normalId1,
		CoordinateFrame& coord0,
		CoordinateFrame& coord1
	);
};

} // namespace RBX

#endif // V8WORLD_RIGIDJOINT_H
