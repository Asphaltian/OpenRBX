#ifndef V8WORLD_SNAPJOINT_H
#define V8WORLD_SNAPJOINT_H

#include "decomp.h"
#include "util/NormalId.h"
#include "v8world/RigidJoint.h"

namespace RBX {

class Primitive;

// VTABLE: WEBSERVICE 0x1023cf60
// SIZE 0x88
class SnapJoint : public RigidJoint
{
public:
	SnapJoint() {}

	SnapJoint(Primitive* prim0, Primitive* prim1, const CoordinateFrame& coord0, const CoordinateFrame& coord1)
		: RigidJoint(prim0, prim1, coord0, coord1)
	{
	}

	virtual ~SnapJoint() {} // vtable+0x00

	static SnapJoint* canBuildJoint(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1);

private:
	// FUNCTION: WEBSERVICE 0x100c66c0 FOLDED
	virtual JointType getJointType() const { return SNAP_JOINT; } // vtable+0x14

	static bool compatibleSurfaces(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1);
};

DECOMP_SIZE_ASSERT(SnapJoint, 0x88)

} // namespace RBX

#endif // V8WORLD_SNAPJOINT_H
