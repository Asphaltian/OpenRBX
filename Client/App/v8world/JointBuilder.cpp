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
	const Extents& e0 = p0->getFastFuzzyExtents();
	const Extents& e1 = p1->getFastFuzzyExtents();

	if (e0.separatedByMoreThan(e1, 0.05f)) {
		return NULL;
	}

	const Matrix3& r0 = p0->getCoordinateFrame().rotation;
	const Matrix3& r1 = p1->getCoordinateFrame().rotation;

	for (int i = 0; i < 6; ++i) {
		NormalId normalId0 = (NormalId) i;
		Vector3 normal = -Math::getWorldNormal(normalId0, r0);
		NormalId normalId1 = Math::getClosestObjectNormalId(normal, r1);

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
