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
Joint* JointBuilder::canJoin(Primitive* p0, Primitive* p1)
{
	const Extents& r0 = p0->getFastFuzzyExtents();
	const Extents& r1 = p1->getFastFuzzyExtents();

	if (r0.separatedByMoreThan(r1, 0.05f)) {
		return NULL;
	}

	const CoordinateFrame& coord0 = p0->getCoordinateFrame();
	const CoordinateFrame& coord1 = p1->getCoordinateFrame();

	for (int i = 0; i < 6; ++i) {
		NormalId normalId0 = (NormalId) i;
		Vector3 normal = -Math::getWorldNormal(normalId0, coord0);
		NormalId normalId1 = Math::getClosestObjectNormalId(normal, coord1.rotation);

		if (Joint* joint = RotateJoint::canBuildJoint(p0, p1, normalId0, normalId1)) {
			return joint;
		}

		if (Joint* joint = WeldJoint::canBuildJoint(p0, p1, normalId0, normalId1)) {
			return joint;
		}

		if (Joint* joint = SnapJoint::canBuildJoint(p0, p1, normalId0, normalId1)) {
			return joint;
		}

		if (Joint* joint = GlueJoint::canBuildJoint(p0, p1, normalId0, normalId1)) {
			return joint;
		}
	}

	return NULL;
}

} // namespace RBX
