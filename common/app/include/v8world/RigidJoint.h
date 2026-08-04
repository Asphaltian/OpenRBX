#ifndef V8WORLD_RIGIDJOINT_H
#define V8WORLD_RIGIDJOINT_H

#include "decomp.h"
#include "v8world/Joint.h"

namespace RBX {

// SIZE 0x88
class RigidJoint : public Joint
{
public:
	static bool jointIsRigid(Joint* joint)
	{
		JointType jointType = joint->getJointType();
		return jointType == WELD_JOINT || jointType == SNAP_JOINT;
	}

	// FUNCTION: WEBSERVICE 0x1009ada0
	static bool isRigidJoint(Edge* edge) { return isJoint(edge) && jointIsRigid(static_cast<Joint*>(edge)); }

	virtual bool isAligned();                                          // vtable+0x24
	virtual CoordinateFrame align(Primitive* prim0, Primitive* prim1); // vtable+0x28

	CoordinateFrame getChildInParent(Primitive* parent, Primitive* child);

	static void faceIdToCoords(
		Primitive* prim0,
		Primitive* prim1,
		NormalId normalId0,
		NormalId normalId1,
		CoordinateFrame& coord0,
		CoordinateFrame& coord1
	);
};

DECOMP_SIZE_ASSERT(RigidJoint, 0x88)

} // namespace RBX

#endif // V8WORLD_RIGIDJOINT_H
