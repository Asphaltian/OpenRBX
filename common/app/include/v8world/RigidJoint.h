#ifndef V8WORLD_RIGIDJOINT_H
#define V8WORLD_RIGIDJOINT_H

#include "decomp.h"
#include "v8world/Joint.h"

namespace RBX {

// VTABLE: WEBSERVICE 0x1023cef0
// SIZE 0x88
class RigidJoint : public Joint
{
public:
	virtual ~RigidJoint() {} // vtable+0x00

	// FUNCTION: WEBSERVICE 0x1009ada0
	static bool isRigidJoint(Edge* e) { return isJoint(e) && jointIsRigid(static_cast<Joint*>(e)); }

	// SYNTHETIC: WEBSERVICE 0x100d7a40
	// RBX::WeldJoint::`scalar deleting destructor'

	virtual bool isAligned();                                          // vtable+0x24
	virtual CoordinateFrame align(Primitive* prim0, Primitive* prim1); // vtable+0x28

	CoordinateFrame getChildInParent(Primitive* parent, Primitive* child);

private:
	static bool jointIsRigid(Joint* joint)
	{
		JointType jointType = joint->getJointType();
		return jointType == WELD_JOINT || jointType == SNAP_JOINT;
	}

protected:
	RigidJoint() {}

	RigidJoint(Primitive* prim0, Primitive* prim1, const CoordinateFrame& coord0, const CoordinateFrame& coord1)
		: Joint(prim0, prim1, coord0, coord1)
	{
	}

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
