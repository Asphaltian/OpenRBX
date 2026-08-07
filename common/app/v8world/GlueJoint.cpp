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
GlueJoint::GlueJoint(
	Primitive* p0,
	Primitive* p1,
	const CoordinateFrame& jointCoord0,
	const CoordinateFrame& jointCoord1
)
	: MultiJoint(p0, p1, jointCoord0, jointCoord1, 4)
{
	Face face0inP0 = p0->getFaceInObject(Matrix3ToNormalId(this->jointCoord0.rotation));
	Face face1inP1 = p1->getFaceInObject(normalIdOpposite(Matrix3ToNormalId(this->jointCoord1.rotation)));

	Face face0inJoint0 = face0inP0.toObjectSpace(jointCoord0);
	Face face1inJoint1 = face1inP1.toObjectSpace(jointCoord1);

	Face overlapInJoint0 = face0inJoint0.projectOverlapOnMe(face1inJoint1);

	overlapInJoint0.snapToGrid(0.1f);

	overlapInP0 = overlapInJoint0.toWorldSpace(jointCoord0);
	overlapInP1 = overlapInJoint0.toWorldSpace(jointCoord1);
}

bool GlueJoint::compatibleSurfaces(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1)
{
	SurfaceType surfaceType0 = prim0->getSurfaceType(normalId0);
	SurfaceType surfaceType1 = prim1->getSurfaceType(normalId1);

	return surfaceType0 == GLUE || surfaceType1 == GLUE;
}

// FUNCTION: WEBSERVICE 0x101205e0
GlueJoint* GlueJoint::canBuildJoint(Primitive* p0, Primitive* p1, NormalId nId0, NormalId nId1)
{
	if (compatibleSurfaces(p0, p1, nId0, nId1) && canBuildJointLoose(p0, p1, nId0, nId1)) {

		Face face0 = p0->getFaceInWorld(nId0);
		Face face1 = p1->getFaceInWorld(nId1);

		Face overlap0 = face0.projectOverlapOnMe(face1);
		Face overlap1 = face1.projectOverlapOnMe(face0);

		Face objectOverlap0 = overlap0.toObjectSpace(p0->getCoordinateFrame());
		Face objectOverlap1 = overlap1.toObjectSpace(p1->getCoordinateFrame());

		objectOverlap0.snapToGrid(0.1f);
		objectOverlap1.snapToGrid(0.1f);

		Face worldOverlap0 = objectOverlap0.toWorldSpace(p0->getCoordinateFrame());
		Face worldOverlap1 = objectOverlap1.toWorldSpace(p1->getCoordinateFrame());

		if (Face::cornersAligned(worldOverlap0, worldOverlap1, 0.05f)) {
			CoordinateFrame coord0 = p0->getFaceCoordInObject(nId0);
			CoordinateFrame world = p0->getCoordinateFrame() * coord0;
			CoordinateFrame object = p1->getCoordinateFrame().toObjectSpace(world);
			CoordinateFrame coord1 = Math::snapToGrid(object, 0.1f);

			return new GlueJoint(p0, p1, coord0, coord1);
		}
	}

	return NULL;
}

// STUB: WEBSERVICE 0x10120810
void GlueJoint::putInKernel(Kernel* _kernel)
{
	MultiJoint::putInKernel(_kernel);

	Body* body0 = getPrimitive(0)->getBody();
	Body* body1 = getPrimitive(1)->getBody();

	NormalId nId0 = Matrix3ToNormalId(jointCoord0.rotation);

	for (int i = 0; i < 4; i++) {
		Vector3 p0World = body0->getCoordinateFrame().pointToWorldSpace(overlapInP0[i]);
		Vector3 p1World = body1->getCoordinateFrame().pointToWorldSpace(overlapInP1[i]);

		Point* point0 = getKernel()->newPoint(body0, p0World);
		Point* point1 = getKernel()->newPoint(body1, p1World);

		Connector* connector = new NormalBreakConnector(point0, point1, getJointK(), getMaxForce(), nId0);

		addToMultiJoint(point0, point1, connector);
	}
}

} // namespace RBX
