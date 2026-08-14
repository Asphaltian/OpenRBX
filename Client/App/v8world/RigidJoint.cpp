#include "v8world/RigidJoint.h"

#include "decomp.h"
#include "util/Math.h"
#include "v8world/Primitive.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x1011ebc0
bool RigidJoint::isAligned()
{
	Primitive* prim0 = getPrimitive(0);
	CoordinateFrame c0World = prim0->getCoordinateFrame() * jointCoord0;

	Primitive* prim1 = getPrimitive(1);
	CoordinateFrame c1World = prim1->getCoordinateFrame() * jointCoord1;

	return Math::fuzzyEq(c0World, c1World, 1e-5f, 1e-5f);
}

// FUNCTION: WEBSERVICE 0x1011ec20
CoordinateFrame RigidJoint::align(Primitive* pMove, Primitive* pStay)
{
	CoordinateFrame& coord0 = pMove == getPrimitive(0) ? jointCoord0 : jointCoord1;
	CoordinateFrame& coord1 = pStay == getPrimitive(0) ? jointCoord0 : jointCoord1;

	CoordinateFrame jointWorld = pStay->getCoordinateFrame() * coord1;

	return jointWorld * coord0.inverse();
}

// FUNCTION: WEBSERVICE 0x1011ec80
CoordinateFrame RigidJoint::getChildInParent(Primitive* parent, Primitive* child)
{
	CoordinateFrame& parentCoord = parent == getPrimitive(0) ? jointCoord0 : jointCoord1;
	CoordinateFrame& childCoord = child == getPrimitive(0) ? jointCoord0 : jointCoord1;

	return parentCoord * childCoord.inverse();
}

// FUNCTION: WEBSERVICE 0x1011ecd0
void RigidJoint::faceIdToCoords(
	Primitive* p0,
	Primitive* p1,
	NormalId nId0,
	NormalId nId1,
	CoordinateFrame& c0,
	CoordinateFrame& c1
)
{
	c0 = p0->getFaceCoordInObject(nId0);

	CoordinateFrame worldC = p0->getCoordinateFrame() * c0;

	c1 = p1->getCoordinateFrame().toObjectSpace(worldC);
}

} // namespace RBX
