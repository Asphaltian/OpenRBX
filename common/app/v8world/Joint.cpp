#include "v8world/Joint.h"

#include "decomp.h"
#include "util/Face.h"
#include "util/Math.h"
#include "v8world/Primitive.h"
#include "v8world/World.h"

#include <cstddef>

namespace RBX {

// FUNCTION: WEBSERVICE 0x100d7b80
CoordinateFrame Joint::align(Primitive* prim0, Primitive* prim1)
{
	return CoordinateFrame();
}

// FUNCTION: WEBSERVICE 0x101019b0 FOLDED
bool Joint::joinsFace(Primitive* primitive, NormalId normalId) const
{
	return false;
}

// FUNCTION: WEBSERVICE 0x1011e870
Joint::~Joint()
{
}

// FUNCTION: WEBSERVICE 0x1011e880
void Joint::setJointOwner(IJointOwner* value)
{
	jointOwner = value;
}

// FUNCTION: WEBSERVICE 0x1011e890
bool Joint::canBuildJoint(
	Primitive* prim0,
	Primitive* prim1,
	NormalId normalId0,
	NormalId normalId1,
	float maxUnaligned,
	float maxOverlapOrGap
)
{
	if (!Math::fuzzyAxisAligned(
			prim0->getCoordinateFrame().rotation,
			prim1->getCoordinateFrame().rotation,
			maxUnaligned
		)) {
		return false;
	}

	Face face0 = prim0->getFaceInWorld(normalId0);
	Face face1 = prim1->getFaceInWorld(normalId1);

	if (!Face::hasOverlap(face0, face1, 0.35f)) {
		return false;
	}

	return Face::overlapWithinPlanes(face0, face1, maxOverlapOrGap) ? true : false;
}

// FUNCTION: WEBSERVICE 0x1011e940
bool Joint::canBuildJointTight(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1)
{
	return canBuildJoint(prim0, prim1, normalId0, normalId1, 0.01f, 0.01f);
}

// FUNCTION: WEBSERVICE 0x1011e970
bool Joint::canBuildJointLoose(Primitive* prim0, Primitive* prim1, NormalId normalId0, NormalId normalId1)
{
	return canBuildJoint(prim0, prim1, normalId0, normalId1, 0.05f, 0.05f);
}

// FUNCTION: WEBSERVICE 0x1011e9a0
Joint::Joint(Primitive* prim0, Primitive* prim1, const CoordinateFrame& coord0, const CoordinateFrame& coord1)
	: Edge(prim0, prim1), jointOwner(NULL), active(false)
{
	jointCoord0 = Math::snapToGrid(coord0, 0.1f);
	jointCoord1 = Math::snapToGrid(coord1, 0.1f);
}

// FUNCTION: WEBSERVICE 0x1011ea80
Joint::Joint() : Edge(NULL, NULL), jointOwner(NULL), active(false)
{
}

// FUNCTION: WEBSERVICE 0x1011eac0
void Joint::setPrimitive(int index, Primitive* primitive)
{
	if (primitive != getPrimitive(index)) {

		World* world = getWorld();

		if (world != NULL && getPrimitive(index) != NULL) {
			world->onJointPrimitiveNulling(this, getPrimitive(index));
		}

		Edge::setPrimitive(index, primitive);

		if (world != NULL && primitive != NULL) {
			world->onJointPrimitiveSet(this, primitive);
		}
	}
}

// FUNCTION: WEBSERVICE 0x1011eb30
void Joint::setJointCoord(int index, const CoordinateFrame& value)
{
	if (value != getJointCoord(index)) {
		if (index == 0) {
			jointCoord0 = value;
		}
		else {
			jointCoord1 = value;
		}

		if (World* world = getWorld()) {
			world->removeJoint(this);
			world->insertJoint(this);
		}
	}
}

} // namespace RBX
