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
WeldJoint* WeldJoint::canBuildJoint(Primitive* p0, Primitive* p1, NormalId nId0, NormalId nId1)
{
	if (compatibleSurfaces(p0, p1, nId0, nId1) && canBuildJointTight(p0, p1, nId0, nId1)) {

		CoordinateFrame c0;
		CoordinateFrame c1;

		faceIdToCoords(p0, p1, nId0, nId1, c0, c1);

		return new WeldJoint(p0, p1, c0, c1);
	}

	return NULL;
}

} // namespace RBX
