#include "tool/RunDragger.h"

#include "tool/Dragger.h"
#include "util/Extents.h"
#include "util/Math.h"
#include "util/Velocity.h"
#include "v8datamodel/Filters.h"
#include "v8datamodel/MouseCommand.h"
#include "v8datamodel/PartInstance.h"
#include "v8world/Contact.h"

#include <G3D/Plane.h>

namespace RBX {

// FUNCTION: WEBSERVICE 0x1012a690
void RunDragger::SnapInfo::updateSurfaceFromHit()
{
	const G3D::CoordinateFrame& snapCoord = snap->getCoordinateFrame();
	G3D::Vector3 hitInSnap = snapCoord.pointToObjectSpace(hitWorld);

	Extents snapExtents = snap->getExtentsLocal();
	surface = snapExtents.closestFace(hitInSnap);
}

// FUNCTION: WEBSERVICE 0x1012a760
void RunDragger::SnapInfo::updateHitFromSurface(const G3D::Ray& mouseRay)
{
	Extents snapExtents = snap->getExtentsLocal();
	const G3D::CoordinateFrame& snapCoord = snap->getCoordinateFrame();
	G3D::Ray rayInSnap = snapCoord.toObjectSpace(mouseRay);
	G3D::Plane plane = snapExtents.getPlane(surface);

	G3D::Vector3 hitInSnap;
	if (Math::intersectRayPlane(rayInSnap, plane, hitInSnap)) {
		hitWorld = snapCoord.pointToWorldSpace(hitInSnap);
	}
	else {
		Math::intersectRayPlane(rayInSnap, plane, hitInSnap);
	}
}

// FUNCTION: WEBSERVICE 0x1012a890
RunDragger::~RunDragger()
{
}

// FUNCTION: WEBSERVICE 0x1012a950
bool RunDragger::adjacent(Primitive* p0, Primitive* p1)
{
	for (Contact* contact = p0->getFirstContact(); contact; contact = p0->getNextContact(contact)) {
		if (contact->otherPrimitive(p0) == p1) {
			return contact->computeIsAdjacent(0.01f);
		}
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x1012a9a0
bool RunDragger::tooCloseToCamera()
{
	G3D::Vector3 dragPointWorld = drag->getCoordinateFrame().pointToWorldSpace(dragPointLocal);

	return (dragPointWorld - mouseRay.origin).magnitude() < 6.0f;
}

// FUNCTION: WEBSERVICE 0x1012aa30
void RunDragger::rotatePart(PartInstance* part)
{
	G3D::CoordinateFrame rotated = part->getCoordinateFrame();
	Math::rotateMatrixAboutY90(rotated.rotation, 1);
	part->setCoordinateFrame(rotated);
}

// FUNCTION: WEBSERVICE 0x1012aae0
float RunDragger::SnapInfo::hitOutsideExtents()
{
	Extents snapExtents = snap->getExtentsLocal();
	const G3D::CoordinateFrame& snapCoord = snap->getCoordinateFrame();
	G3D::Vector3 hitInSnap = snapCoord.pointToObjectSpace(hitWorld);
	G3D::Vector3 clipInSnap = snapExtents.clip(hitInSnap);
	return (hitInSnap - clipInSnap).magnitude();
}

// FUNCTION: WEBSERVICE 0x1012abd0
RunDragger::RunDragger() : workspace(NULL), drag(NULL)
{
}

// FUNCTION: WEBSERVICE 0x1012ac40
RunDragger::SnapInfo RunDragger::createSnapSurface(Primitive* snap, G3D::Array<NormalId>* ignore)
{
	SnapInfo answer;
	answer.snap = snap;

	Extents snapExtents = snap->getExtentsLocal();
	const G3D::CoordinateFrame& snapCoord = snap->getCoordinateFrame();

	G3D::Ray rayInSnap = snapCoord.toObjectSpace(mouseRay);

	float bestDistance = 0.0f;
	bool found = false;

	for (int i = 0; i < 6; ++i) {
		NormalId surface = (NormalId) i;
		if (!ignore || ignore->find(surface) == ignore->end()) {
			G3D::Plane plane = snapExtents.getPlane(surface);
			if (plane.halfSpaceContainsFinite(rayInSnap.origin)) {
				G3D::Vector3 hitInSnap;
				if (Math::intersectRayPlane(rayInSnap, plane, hitInSnap)) {
					G3D::Vector3 hitWorld = snapCoord.pointToWorldSpace(hitInSnap);
					float distance = (mouseRay.origin - hitWorld).magnitude();
					if (distance > bestDistance) {
						bestDistance = distance;
						answer.surface = surface;
						answer.hitWorld = hitWorld;
						found = true;
					}
				}
			}
		}
	}

	return found ? answer : SnapInfo();
}

// FUNCTION: WEBSERVICE 0x1012af40
bool RunDragger::notTried(Primitive* check, const G3D::Array<Primitive*>& tried)
{
	return check && tried.find(check) == tried.end();
}

// FUNCTION: WEBSERVICE 0x1012af90
RunDragger::SnapInfo RunDragger::bestProximatePart(
	const G3D::Array<Primitive*>& triedSnap,
	bool (Contact::*proximityTest)(float)
)
{
	SnapInfo answer;

	for (Contact* contact = drag->getFirstContact(); contact; contact = drag->getNextContact(contact)) {
		if ((contact->*proximityTest)(0.01f)) {
			Primitive* test = contact->otherPrimitive(drag);
			if (notTried(test, triedSnap)) {
				SnapInfo testInfo = createSnapSurface(test, NULL);
				if (testInfo.snap) {
					if (testInfo.hitOutsideExtents() < 1e10f) {
						answer = testInfo;
					}
				}
			}
		}
	}

	return answer;
}

// FUNCTION: WEBSERVICE 0x1012b0f0
void RunDragger::turnUpright(PartInstance* part)
{
	if (part->getSurfaces().isStandardPart()) {
		G3D::CoordinateFrame dragCoord = part->getCoordinateFrame();
		if (Math::getClosestObjectNormalId(G3D::Vector3::unitY(), dragCoord.rotation) != NORM_Y) {
			dragCoord.rotation = G3D::Matrix3::identity();
			part->setCoordinateFrame(dragCoord);
		}
	}
}

// FUNCTION: WEBSERVICE 0x1012b1a0
void RunDragger::tiltPart(PartInstance* part, const G3D::CoordinateFrame& camera)
{
	G3D::CoordinateFrame dragCoord = part->getCoordinateFrame();
	NormalId normalId = Math::getClosestObjectNormalId(camera.rightVector(), dragCoord.rotation);
	G3D::Vector3 axis = Math::getWorldNormal(normalId, dragCoord);
	G3D::Matrix3 tiltM = G3D::Matrix3::fromAxisAngle(axis, 1.5707964f);
	dragCoord.rotation = tiltM * dragCoord.rotation;
	part->setCoordinateFrame(dragCoord);
}

// FUNCTION: WEBSERVICE 0x1012b3a0
RunDragger::SnapInfo RunDragger::rayHitsPart(const G3D::Array<Primitive*>& triedSnap, bool forceAdjacent)
{
	SnapInfo answer;
	PartByLocalCharacter filter(workspace);

	if (PartInstance* part = MouseCommand::getMousePart(
			mouseRay,
			*workspace->getWorld()->getContactManager(),
			drag,
			&filter,
			answer.hitWorld,
			2048.0f
		)) {
		answer.snap = part->getPrimitive();
		if (notTried(answer.snap, triedSnap)) {
			if (!forceAdjacent || adjacent(answer.snap, drag)) {
				answer.updateSurfaceFromHit();
				return answer;
			}
		}
	}

	return SnapInfo();
}

// FUNCTION: WEBSERVICE 0x1012b520
RunDragger::SnapInfo RunDragger::findSnap(const G3D::Array<Primitive*>& triedSnap)
{
	SnapInfo answer;

	answer = rayHitsPart(triedSnap, true);
	if (answer.snap) {
		return answer;
	}

	answer = bestProximatePart(triedSnap, &Contact::computeIsAdjacent);
	if (answer.snap) {
		return answer;
	}

	answer = bestProximatePart(triedSnap, &Contact::computeIsColliding);
	if (answer.snap) {
		return answer;
	}

	answer = rayHitsPart(triedSnap, false);
	if (answer.snap) {
		return answer;
	}

	return SnapInfo();
}

// FUNCTION: WEBSERVICE 0x1012b6f0
void RunDragger::smartPtrHackPre()
{
	boost::shared_ptr<PartInstance> p = snapPart.lock();
	if (PartInstance::nonNullInWorkspace(p)) {
		snapInfo.snap = p->getPrimitive();
	}
	else {
		snapInfo.snap = NULL;
	}
}

// FUNCTION: WEBSERVICE 0x1012b7b0
void RunDragger::init(
	Workspace* _workspace,
	boost::weak_ptr<PartInstance> _dragPart,
	const G3D::Vector3& _dragPointWorld
)
{
	workspace = _workspace;
	dragPart = _dragPart;
	drag = _dragPart.lock()->getPrimitive();
	dragPointLocal = drag->getCoordinateFrame().pointToObjectSpace(_dragPointWorld);
	drag->setVelocity(Velocity::zero());
	snapInfo = SnapInfo();
	turnUpright(dragPart.lock().get());
}

// FUNCTION: WEBSERVICE 0x1012b9d0
bool RunDragger::snapDragPart()
{
	Face snapFace = snapInfo.snap->getFaceInWorld(snapInfo.surface);
	Face dragFace = drag->getFaceInWorld(dragSurface);

	G3D::Vector3 snapCornerWorld = snapFace[0];
	G3D::Vector3 dragCornerWorld = dragFace[0];

	G3D::CoordinateFrame snapFaceCoords(snapInfo.snap->getCoordinateFrame().rotation, snapCornerWorld);

	G3D::Vector3 dragCornerLocal = snapFaceCoords.pointToObjectSpace(dragCornerWorld);
	G3D::Vector3 snapDragCornerLocal = Math::iRoundVector3(dragCornerLocal);

	G3D::Vector3 delta = snapFaceCoords.pointToWorldSpace(snapDragCornerLocal) - dragCornerWorld;

	G3D::CoordinateFrame newDragCoord = drag->getCoordinateFrame();
	newDragCoord.translation += delta;
	dragPart.lock()->setCoordinateFrame(newDragCoord);

	if (snapDragCornerLocal != snapInfo.lastDragSnap) {
		snapInfo.lastDragSnap = snapDragCornerLocal;
		return true;
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x1012bcb0
bool RunDragger::rayHitsCloserPart()
{
	G3D::Array<Primitive*> ignoreNone;
	SnapInfo answer = rayHitsPart(ignoreNone, false);
	if (answer.snap && answer.snap != snapInfo.snap) {
		float answerDistance = (answer.hitWorld - mouseRay.origin).magnitude();
		float snapDistance = (snapInfo.hitWorld - mouseRay.origin).magnitude();
		if (answerDistance < snapDistance) {
			return true;
		}
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x1012bdb0
void RunDragger::findSafeY()
{
	if (colliding()) {
		PartInstance* part = PartInstance::fromPrimitive(drag);
		G3D::Array<Primitive*> temp;
		temp.append(part->getPrimitive());

		G3D::Vector3 moved =
			Dragger::safeMoveNoDrop(temp, G3D::Vector3::zero(), *workspace->getWorld()->getContactManager());

		G3D::CoordinateFrame newC = drag->getCoordinateFrame();
		newC.translation += moved;
		dragPart.lock()->setCoordinateFrame(newC);
	}
}

// FUNCTION: WEBSERVICE 0x1012bf40
void RunDragger::smartPtrHackPost()
{
	if (snapInfo.snap) {
		snapPart = shared_from(PartInstance::fromPrimitive(snapInfo.snap));
	}
	else {
		snapPart.reset();
	}
}

// STUB: WEBSERVICE 0x1012c040
bool RunDragger::moveDragPart()
{
	const G3D::CoordinateFrame& snapCoord = snapInfo.snap->getCoordinateFrame();
	int face = snapInfo.surface / 3;
	int axis = snapInfo.surface - face * 3;
	int sign = 1 - face * 2;
	G3D::Vector3 snapNormal =
		G3D::Vector3(snapCoord.rotation[0][axis], snapCoord.rotation[1][axis], snapCoord.rotation[2][axis]) *
		(float) sign;

	G3D::Matrix3 dragInWorld = drag->getCoordinateFrame().rotation;

	G3D::Matrix3 alignedInWorld = Math::alignAxesClosest(dragInWorld, snapCoord.rotation);
	G3D::Matrix3 alignedInDrag = dragInWorld.transpose() * alignedInWorld;
	G3D::Matrix3 dragRotation = snapCoord.rotation * alignedInDrag.transpose();

	Math::orthonormalizeIfNecessary(dragRotation);

	G3D::CoordinateFrame dragCoord(dragRotation, drag->getCoordinateFrame().translation);

	drag->setCoordinateFrame(dragCoord);

	dragSurface = Math::getClosestObjectNormalId(-snapNormal, dragRotation);

	G3D::Ray throughDragHit =
		G3D::Ray::fromOriginAndDirection(dragCoord.pointToWorldSpace(dragPointLocal), mouseRay.direction);
	G3D::Ray rayInDrag = dragCoord.toObjectSpace(throughDragHit);
	Extents dragExtentsLocal = drag->getExtentsLocal();
	G3D::Plane plane = dragExtentsLocal.getPlane(dragSurface);
	Math::intersectRayPlane(rayInDrag, plane, dragHitLocal);

	G3D::Vector3 clampedHitLocal = dragHitLocal.clamp(dragExtentsLocal.min() * 100.0f, dragExtentsLocal.max() * 100.0f);
	G3D::Vector3 dragHitWorld = dragCoord.pointToWorldSpace(clampedHitLocal);

	dragCoord.translation = snapInfo.hitWorld + (dragCoord.translation - dragHitWorld);
	dragPart.lock()->setCoordinateFrame(dragCoord);

	return snapDragPart();
}

// FUNCTION: WEBSERVICE 0x1012c440
bool RunDragger::fallOffPart(bool& snapped)
{
	G3D::Array<NormalId> triedSurfaces;

	while (snapInfo.snap && fallOffEdge()) {
		triedSurfaces.append(snapInfo.surface);
		snapInfo = createSnapSurface(snapInfo.snap, &triedSurfaces);
		if (snapInfo.snap) {
			snapped = moveDragPart();
		}
	}

	return snapInfo.snap == NULL;
}

// FUNCTION: WEBSERVICE 0x1012c5e0
void RunDragger::findNoSnapPosition(const G3D::CoordinateFrame& original)
{
	dragPart.lock()->setCoordinateFrame(original);
	findSafeY();
}

// FUNCTION: WEBSERVICE 0x1012c680
bool RunDragger::snap(const G3D::Ray& _mouseRay)
{
	smartPtrHackPre();

	mouseRay = _mouseRay;

	G3D::CoordinateFrame originalDrag = drag->getCoordinateFrame();

	G3D::Array<Primitive*> triedSnap;

	if (snapInfo.snap) {
		snapInfo.updateHitFromSurface(mouseRay);
	}
	else {
		snapInfo = findSnap(triedSnap);
	}

	while (snapInfo.snap && triedSnap.find(snapInfo.snap) == triedSnap.end()) {
		triedSnap.append(snapInfo.snap);

		bool snapped = moveDragPart();

		if (!fallOffPart(snapped) && !colliding() && !rayHitsCloserPart() && !tooCloseToCamera()) {
			smartPtrHackPost();
			return snapped;
		}

		snapInfo = findSnap(triedSnap);
	}

	findNoSnapPosition(originalDrag);
	smartPtrHackPost();
	return false;
}

} // namespace RBX
