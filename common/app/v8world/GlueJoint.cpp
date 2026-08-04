#include "v8world/GlueJoint.h"

#include "decomp.h"
#include "util/Math.h"
#include "util/Units.h"
#include "v8kernel/Body.h"
#include "v8kernel/Connector.h"
#include "v8kernel/Constants.h"
#include "v8kernel/Kernel.h"
#include "v8world/Primitive.h"

#include <cstddef>

namespace RBX {

// FUNCTION: WEBSERVICE 0x101201d0
GlueJoint::GlueJoint() : MultiJoint(4)
{
}

// FUNCTION: WEBSERVICE 0x10120290
float GlueJoint::getMaxForce()
{
	Vector2 size = overlapInP0.size();
	float maxForce = Constants::getKmsMaxJointForce(size.x, size.y);

	return Units::kmsForceToRbx(maxForce);
}

// FUNCTION: WEBSERVICE 0x10120340
GlueJoint::GlueJoint(Primitive* prim0, Primitive* prim1, const CoordinateFrame& coord0, const CoordinateFrame& coord1)
	: MultiJoint(prim0, prim1, coord0, coord1, 4)
{
	Face face0 = prim0->getFaceInObject(Matrix3ToNormalId(jointCoord0.rotation));
	Face face1 = prim1->getFaceInObject(normalIdOpposite(Matrix3ToNormalId(jointCoord1.rotation)));

	Face objectFace0 = face0.toObjectSpace(coord0);
	Face objectFace1 = face1.toObjectSpace(coord1);

	Face overlap = objectFace0.projectOverlapOnMe(objectFace1);

	overlap.snapToGrid(0.1f);

	overlapInP0 = overlap.toWorldSpace(coord0);
	overlapInP1 = overlap.toWorldSpace(coord1);
}

bool GlueJoint::compatibleSurfaces(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1)
{
	SurfaceType surfaceType0 = prim0->getSurfaceType(normalId0);
	SurfaceType surfaceType1 = prim1->getSurfaceType(normalId1);

	return surfaceType0 == GLUE || surfaceType1 == GLUE;
}

// FUNCTION: WEBSERVICE 0x101205e0
GlueJoint* GlueJoint::canBuildJoint(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1)
{
	if (compatibleSurfaces(prim0, prim1, normalId0, normalId1) &&
		canBuildJointLoose(prim0, prim1, normalId0, normalId1)) {

		Face face0 = prim0->getFaceInWorld(normalId0);
		Face face1 = prim1->getFaceInWorld(normalId1);

		Face overlap0 = face0.projectOverlapOnMe(face1);
		Face overlap1 = face1.projectOverlapOnMe(face0);

		Face objectOverlap0 = overlap0.toObjectSpace(prim0->getCoordinateFrame());
		Face objectOverlap1 = overlap1.toObjectSpace(prim1->getCoordinateFrame());

		objectOverlap0.snapToGrid(0.1f);
		objectOverlap1.snapToGrid(0.1f);

		Face worldOverlap0 = objectOverlap0.toWorldSpace(prim0->getCoordinateFrame());
		Face worldOverlap1 = objectOverlap1.toWorldSpace(prim1->getCoordinateFrame());

		if (Face::cornersAligned(worldOverlap0, worldOverlap1, 0.05f)) {
			CoordinateFrame coord0 = prim0->getFaceCoordInObject(normalId0);
			CoordinateFrame world = prim0->getCoordinateFrame() * coord0;
			CoordinateFrame object = prim1->getCoordinateFrame().toObjectSpace(world);
			CoordinateFrame coord1 = Math::snapToGrid(object, 0.1f);

			return new GlueJoint(prim0, prim1, coord0, coord1);
		}
	}

	return NULL;
}

// STUB: WEBSERVICE 0x10120810
void GlueJoint::putInKernel(Kernel* kernel)
{
	MultiJoint::putInKernel(kernel);

	Body* body0 = getPrimitive(0)->getBody();
	Body* body1 = getPrimitive(1)->getBody();

	NormalId normalId = Matrix3ToNormalId(jointCoord0.rotation);

	for (int i = 0; i < 4; ++i) {
		Vector3 pos0 = body0->getCoordinateFrame().pointToWorldSpace(overlapInP0[i]);
		Vector3 pos1 = body1->getCoordinateFrame().pointToWorldSpace(overlapInP1[i]);

		Point* point0 = getKernel()->newPoint(body0, pos0);
		Point* point1 = getKernel()->newPoint(body1, pos1);

		Connector* connector = new NormalBreakConnector(point0, point1, getJointK(), getMaxForce(), normalId);

		addToMultiJoint(point0, point1, connector);
	}
}

} // namespace RBX
