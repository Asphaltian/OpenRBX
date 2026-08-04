#include "v8world/SnapJoint.h"

#include "decomp.h"
#include "v8world/Primitive.h"
#include "v8world/SurfaceData.h"

#include <cstddef>

namespace RBX {

bool SnapJoint::compatibleSurfaces(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1)
{
	SurfaceType surfaceType0 = prim0->getSurfaceType(normalId0);
	SurfaceType surfaceType1 = prim1->getSurfaceType(normalId1);

	return (surfaceType0 == STUDS && surfaceType1 == INLET) || (surfaceType0 == INLET && surfaceType1 == STUDS);
}

// FUNCTION: WEBSERVICE 0x10127640
SnapJoint* SnapJoint::canBuildJoint(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1)
{
	if (compatibleSurfaces(prim0, prim1, normalId0, normalId1) &&
		canBuildJointTight(prim0, prim1, normalId0, normalId1)) {

		CoordinateFrame coord0;
		CoordinateFrame coord1;

		faceIdToCoords(prim0, prim1, normalId0, normalId1, coord0, coord1);

		return new SnapJoint(prim0, prim1, coord0, coord1);
	}

	return NULL;
}

} // namespace RBX
