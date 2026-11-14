#ifndef V8WORLD_SNAPJOINT_H
#define V8WORLD_SNAPJOINT_H

#include "placeholder.h"
#include "util/NormalId.h"
#include "v8world/RigidJoint.h"

namespace RBX {

class Primitive;

class SnapJoint : public RigidJoint
{
public:
	SnapJoint() {}

	SnapJoint(Primitive* prim0, Primitive* prim1, const CoordinateFrame& coord0, const CoordinateFrame& coord1)
		: RigidJoint(prim0, prim1, coord0, coord1)
	{
	}

	virtual ~SnapJoint() {}

	static SnapJoint* canBuildJoint(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1);

private:
	virtual JointType getJointType() const { return SNAP_JOINT; }

	static bool compatibleSurfaces(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1);
};

} // namespace RBX

#endif // V8WORLD_SNAPJOINT_H
