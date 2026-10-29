#include "tool/Dragger.h"

#include "decomp.h"
#include "tool/DragUtilities.h"
#include "util/Math.h"
#include "v8datamodel/PartInstance.h"
#include "v8world/SpatialHash.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x10105680
// FUNCTION: ROBLOX 0x0061da40
G3D::Vector3 Dragger::toGrid(const G3D::Vector3& v)
{
	return Math::toGrid(v, dragSnap());
}

// FUNCTION: WEBSERVICE 0x101056d0
// FUNCTION: ROBLOX 0x0061da90
void Dragger::movePrimitives(const G3D::Array<Primitive*>& primitives, const G3D::Vector3& delta)
{
	if (delta != G3D::Vector3::zero()) {
		for (int i = 0; i < primitives.size(); i++) {
			Primitive* primitive = primitives[i];
			PartInstance* part = PartInstance::fromPrimitive(primitive);
			DragUtilities::moveAndClean(part, delta);
		}
	}
}

// FUNCTION: WEBSERVICE 0x10105760
// FUNCTION: ROBLOX 0x0061dc20
Extents Dragger::computeExtents(const G3D::Array<Primitive*>& primitives)
{
	Extents answer = Extents::negativeInfiniteExtents();

	for (int i = 0; i < primitives.size(); i++) {
		answer.unionWith(primitives[i]->getExtentsWorld());
	}

	return answer;
}

// FUNCTION: WEBSERVICE 0x10105820
// FUNCTION: ROBLOX 0x0061dce0
bool Dragger::intersectingWorldOrOthers(const G3D::Array<Primitive*>& primitives, ContactManager& contactManager)
{
	Extents group = computeExtents(primitives);

	return group.min().y < -0.01f || contactManager.intersectingOthers(primitives, 0.01f);
}

// FUNCTION: WEBSERVICE 0x10105870
// FUNCTION: ROBLOX 0x0061de20
void Dragger::movePrimitivesDelta(
	const G3D::Array<Primitive*>& primitives, const G3D::Vector3& delta, G3D::Vector3& movedSoFar
)
{
	movePrimitives(primitives, delta);
	movedSoFar = toGrid(delta + movedSoFar);
}

// FUNCTION: WEBSERVICE 0x10105910
// FUNCTION: ROBLOX 0x0061dec0
void Dragger::searchUpFine(
	const G3D::Array<Primitive*>& primitives, G3D::Vector3& movedSoFar, ContactManager& contactManager
)
{
	movePrimitivesDelta(primitives, G3D::Vector3(0.0f, 0.1f, 0.0f), movedSoFar);

	if (intersectingWorldOrOthers(primitives, contactManager)) {
		searchUpFine(primitives, movedSoFar, contactManager);
	}
}

// FUNCTION: WEBSERVICE 0x10105990
// FUNCTION: ROBLOX 0x0061df40
void Dragger::searchDownFine(
	const G3D::Array<Primitive*>& primitives, G3D::Vector3& movedSoFar, ContactManager& contactManager
)
{
	movePrimitivesDelta(primitives, G3D::Vector3(0.0f, -0.1f, 0.0f), movedSoFar);

	if (!intersectingWorldOrOthers(primitives, contactManager)) {
		searchDownFine(primitives, movedSoFar, contactManager);
	}
	else {
		searchUpFine(primitives, movedSoFar, contactManager);
	}
}

// FUNCTION: WEBSERVICE 0x10105a10
// FUNCTION: ROBLOX 0x0061dfc0
void Dragger::searchUpGross(
	const G3D::Array<Primitive*>& primitives, G3D::Vector3& movedSoFar, ContactManager& contactManager
)
{
	movePrimitivesDelta(primitives, G3D::Vector3(0.0f, 1.2f, 0.0f), movedSoFar);

	if (intersectingWorldOrOthers(primitives, contactManager)) {
		searchUpGross(primitives, movedSoFar, contactManager);
	}
	else {
		searchDownFine(primitives, movedSoFar, contactManager);
	}
}

// FUNCTION: WEBSERVICE 0x10105a90
// FUNCTION: ROBLOX 0x0061e040
void Dragger::searchDownGross(
	const G3D::Array<Primitive*>& primitives, G3D::Vector3& movedSoFar, ContactManager& contactManager
)
{
	movePrimitivesDelta(primitives, G3D::Vector3(0.0f, -1.2f, 0.0f), movedSoFar);

	if (!intersectingWorldOrOthers(primitives, contactManager)) {
		searchDownGross(primitives, movedSoFar, contactManager);
	}
	else {
		searchUpFine(primitives, movedSoFar, contactManager);
	}
}

// FUNCTION: WEBSERVICE 0x10105b10
// FUNCTION: ROBLOX 0x0061e370
G3D::Vector3 Dragger::safeMoveYDrop(
	const G3D::Array<Primitive*>& primitives, const G3D::Vector3& tryMove, ContactManager& contactManager
)
{
	if (primitives.size() == 0) {
		return G3D::Vector3::zero();
	}

	G3D::Vector3 moved(tryMove.x, tryMove.y - 1.2f, tryMove.z);
	moved = Math::toGrid(moved, 0.1f);

	movePrimitives(primitives, moved);

	if (intersectingWorldOrOthers(primitives, contactManager)) {
		searchUpGross(primitives, moved, contactManager);
	}
	else {
		searchDownGross(primitives, moved, contactManager);
	}

	return moved;
}

// FUNCTION: WEBSERVICE 0x10105c10
// FUNCTION: ROBLOX 0x0061e470
G3D::Vector3 Dragger::safeMoveNoDrop(
	const G3D::Array<Primitive*>& primitives, const G3D::Vector3& tryMove, ContactManager& contactManager
)
{
	G3D::Vector3 movedSoFar(0.0f, 0.0f, 0.0f);

	if (primitives.size() == 0) {
		return G3D::Vector3::zero();
	}

	if (tryMove != G3D::Vector3::zero()) {
		movePrimitives(primitives, tryMove);
		movedSoFar += tryMove;
	}

	if (intersectingWorldOrOthers(primitives, contactManager)) {
		searchUpGross(primitives, movedSoFar, contactManager);
	}

	return movedSoFar;
}

// FUNCTION: WEBSERVICE 0x10105d40
// FUNCTION: ROBLOX 0x0061e5a0
void Dragger::safeRotate(
	const G3D::Array<Primitive*>& primitives, const G3D::Matrix3& rotate, ContactManager& contactManager
)
{
	if (primitives.size() != 0) {
		G3D::Vector3 center = toGrid(computeExtents(primitives).center());

		CoordinateFrame centerCoord(center);
		CoordinateFrame rotationCoord(rotate, G3D::Vector3::zero());

		for (int i = 0; i < primitives.size(); i++) {
			Primitive* primitive = primitives[i];
			CoordinateFrame primInCenter = centerCoord.toObjectSpace(primitive->getCoordinateFrame());
			CoordinateFrame newPrimInCenter = rotationCoord * primInCenter;
			CoordinateFrame primInWorld = centerCoord * newPrimInCenter;
			PartInstance* part = PartInstance::fromPrimitive(primitive);
			part->setCoordinateFrame(primInWorld);
		}
	}
}

} // namespace RBX
