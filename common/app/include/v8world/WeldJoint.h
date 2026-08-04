#ifndef V8WORLD_WELDJOINT_H
#define V8WORLD_WELDJOINT_H

#include "decomp.h"
#include "util/NormalId.h"
#include "v8world/RigidJoint.h"

namespace RBX {

class Primitive;

// VTABLE: WEBSERVICE 0x1023cf28
// SIZE 0x88
class WeldJoint : public RigidJoint
{
public:
	WeldJoint() {}

	WeldJoint(Primitive* prim0, Primitive* prim1, const CoordinateFrame& coord0, const CoordinateFrame& coord1)
		: RigidJoint(prim0, prim1, coord0, coord1)
	{
	}

	virtual ~WeldJoint() {} // vtable+0x00

	static WeldJoint* canBuildJoint(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1);

private:
	// FUNCTION: WEBSERVICE 0x100c6bd0
	virtual JointType getJointType() const { return WELD_JOINT; } // vtable+0x14

	static bool compatibleSurfaces(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1);
};

DECOMP_SIZE_ASSERT(WeldJoint, 0x88)

} // namespace RBX

#endif // V8WORLD_WELDJOINT_H
