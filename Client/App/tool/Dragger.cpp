#include "tool/Dragger.h"

#include "tool/DragUtilities.h"
#include "placeholder.h"
#include "util/Math.h"
#include "v8datamodel/PartInstance.h"
#include "v8world/SpatialHash.h"

namespace RBX {

G3D::Vector3 Dragger::toGrid(const G3D::Vector3& v)
{
	return Math::toGrid(v, dragSnap());
}

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

Extents Dragger::computeExtents(const std::vector<Primitive*>& primitives)
{
	Extents answer = Extents::negativeInfiniteExtents();

	for (unsigned int i = 0; i < primitives.size(); i++) {
		answer.unionWith(primitives[i]->getExtentsWorld());
	}

	return answer;
}

Extents Dragger::computeExtents(const G3D::Array<Primitive*>& primitives)
{
	Extents answer = Extents::negativeInfiniteExtents();

	for (int i = 0; i < primitives.size(); i++) {
		answer.unionWith(primitives[i]->getExtentsWorld());
	}

	return answer;
}

bool Dragger::intersectingWorldOrOthers(const G3D::Array<Primitive*>& primitives, ContactManager& contactManager)
{
	Extents group = computeExtents(primitives);

	return group.min().y < -0.01f || contactManager.intersectingOthers(primitives, 0.01f);
}

void Dragger::movePrimitivesGoal(
	const G3D::Array<Primitive*>& primitives, const G3D::Vector3& p_unk0x08, G3D::Vector3& movedSoFar
)
{
	G3D::Vector3 delta = toGrid(p_unk0x08 - movedSoFar);

	if (delta != G3D::Vector3::zero()) {
		movePrimitives(primitives, delta);
		movedSoFar = p_unk0x08;
	}
}

void Dragger::movePrimitivesDelta(
	const G3D::Array<Primitive*>& primitives, const G3D::Vector3& delta, G3D::Vector3& movedSoFar
)
{
	movePrimitives(primitives, delta);
	movedSoFar = toGrid(delta + movedSoFar);
}

void Dragger::searchUpFine(
	const G3D::Array<Primitive*>& primitives, G3D::Vector3& movedSoFar, ContactManager& contactManager
)
{
	movePrimitivesDelta(primitives, G3D::Vector3(0.0f, 0.1f, 0.0f), movedSoFar);

	if (intersectingWorldOrOthers(primitives, contactManager)) {
		searchUpFine(primitives, movedSoFar, contactManager);
	}
}

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

void Dragger::safePlaceAlongLine(
	const G3D::Array<Primitive*>& primitives,
	const G3D::Vector3& p_unk0x08,
	const G3D::Vector3& p_unk0x0c,
	G3D::Vector3& movedSoFar,
	ContactManager& contactManager
)
{
	movePrimitivesGoal(primitives, p_unk0x0c, movedSoFar);

	if (intersectingWorldOrOthers(primitives, contactManager)) {
		G3D::Vector3 middle = toGrid((p_unk0x08 + p_unk0x0c) * 0.51f);

		movePrimitivesGoal(primitives, middle, movedSoFar);

		bool intersecting = intersectingWorldOrOthers(primitives, contactManager);

		if (middle == p_unk0x0c) {
			if (intersecting) {
				movePrimitivesGoal(primitives, p_unk0x08, movedSoFar);
			}
		}
		else if (intersecting) {
			safePlaceAlongLine(primitives, p_unk0x08, middle, movedSoFar, contactManager);
		}
		else {
			safePlaceAlongLine(primitives, middle, p_unk0x0c, movedSoFar, contactManager);
		}
	}
}

G3D::Vector3 Dragger::safeMoveAlongLine(
	const G3D::Array<Primitive*>& primitives, const G3D::Vector3& tryMove, ContactManager& contactManager
)
{
	if (primitives.size() == 0) {
		return G3D::Vector3::zero();
	}

	G3D::Vector3 movedSoFar = G3D::Vector3::zero();

	if (tryMove != G3D::Vector3::zero()) {
		safePlaceAlongLine(primitives, G3D::Vector3::zero(), tryMove, movedSoFar, contactManager);
	}

	return movedSoFar;
}

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
