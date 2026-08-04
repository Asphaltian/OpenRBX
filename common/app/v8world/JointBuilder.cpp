#include "v8world/JointBuilder.h"

#include "decomp.h"
#include "util/Math.h"
#include "v8world/GlueJoint.h"
#include "v8world/Primitive.h"
#include "v8world/RotateJoint.h"
#include "v8world/SnapJoint.h"
#include "v8world/WeldJoint.h"

#include <cstddef>

namespace RBX {

// FUNCTION: WEBSERVICE 0x1011cdb0
Joint* JointBuilder::canJoin(Primitive* prim0, Primitive* prim1)
{
	const Extents& extents0 = prim0->getFastFuzzyExtents();
	const Extents& extents1 = prim1->getFastFuzzyExtents();

	if (extents0.separatedByMoreThan(extents1, 0.05f)) {
		return NULL;
	}

	const CoordinateFrame& coord0 = prim0->getCoordinateFrame();
	const CoordinateFrame& coord1 = prim1->getCoordinateFrame();

	for (int i = 0; i < 6; ++i) {
		NormalId normalId0 = (NormalId) i;
		Vector3 normal = -Math::getWorldNormal(normalId0, coord0);
		NormalId normalId1 = Math::getClosestObjectNormalId(normal, coord1.rotation);

		if (Joint* joint = RotateJoint::canBuildJoint(prim0, prim1, normalId0, normalId1)) {
			return joint;
		}

		if (Joint* joint = WeldJoint::canBuildJoint(prim0, prim1, normalId0, normalId1)) {
			return joint;
		}

		if (Joint* joint = SnapJoint::canBuildJoint(prim0, prim1, normalId0, normalId1)) {
			return joint;
		}

		if (Joint* joint = GlueJoint::canBuildJoint(prim0, prim1, normalId0, normalId1)) {
			return joint;
		}
	}

	return NULL;
}

} // namespace RBX
