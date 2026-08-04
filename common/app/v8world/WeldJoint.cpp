#include "v8world/WeldJoint.h"

#include "decomp.h"
#include "v8world/Primitive.h"
#include "v8world/SurfaceData.h"

#include <cstddef>

namespace RBX {

bool WeldJoint::compatibleSurfaces(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1)
{
	SurfaceType surfaceType0 = prim0->getSurfaceType(normalId0);
	SurfaceType surfaceType1 = prim1->getSurfaceType(normalId1);

	return surfaceType0 == WELD || surfaceType1 == WELD;
}

// FUNCTION: WEBSERVICE 0x10127730
WeldJoint* WeldJoint::canBuildJoint(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1)
{
	if (compatibleSurfaces(prim0, prim1, normalId0, normalId1) &&
		canBuildJointTight(prim0, prim1, normalId0, normalId1)) {

		CoordinateFrame coord0;
		CoordinateFrame coord1;

		faceIdToCoords(prim0, prim1, normalId0, normalId1, coord0, coord1);

		return new WeldJoint(prim0, prim1, coord0, coord1);
	}

	return NULL;
}

} // namespace RBX
