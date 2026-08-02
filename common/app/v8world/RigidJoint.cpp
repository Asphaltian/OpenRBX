#include "v8world/RigidJoint.h"

#include "decomp.h"
#include "util/Math.h"
#include "v8world/Primitive.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x1011ebc0
bool RigidJoint::isAligned()
{
	Primitive* prim0 = getPrimitive(0);
	CoordinateFrame coord0 = prim0->getCoordinateFrame() * jointCoord0;

	Primitive* prim1 = getPrimitive(1);
	CoordinateFrame coord1 = prim1->getCoordinateFrame() * jointCoord1;

	return Math::fuzzyEq(coord0, coord1, 1e-5f, 1e-5f);
}

// FUNCTION: WEBSERVICE 0x1011ec20
CoordinateFrame RigidJoint::align(Primitive* prim0, Primitive* prim1)
{
	CoordinateFrame& coord0 = prim0 == getPrimitive(0) ? jointCoord0 : jointCoord1;
	CoordinateFrame& coord1 = prim1 == getPrimitive(0) ? jointCoord0 : jointCoord1;

	CoordinateFrame world = prim1->getCoordinateFrame() * coord1;

	return world * coord0.inverse();
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
	Primitive* prim0,
	Primitive* prim1,
	NormalId normalId0,
	NormalId normalId1,
	CoordinateFrame& coord0,
	CoordinateFrame& coord1
)
{
	coord0 = prim0->getFaceCoordInObject(normalId0);

	CoordinateFrame world = prim0->getCoordinateFrame() * coord0;

	coord1 = prim1->getCoordinateFrame().toObjectSpace(world);
}

} // namespace RBX
