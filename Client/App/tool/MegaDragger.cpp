#include "tool/MegaDragger.h"

#include "tool/DragUtilities.h"
#include "tool/Dragger.h"
#include "util/Math.h"
#include "v8datamodel/MouseCommand.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/RootInstance.h"
#include "v8world/SpatialHash.h"
#include "v8world/World.h"

namespace RBX {

void MegaDragger::continueDragging()
{
	DragUtilities::unJoinAndSetDragging(dragParts);
}

bool MegaDragger::anyDragPartAlive()
{
	return DragUtilities::anyPartAlive(dragParts);
}

bool MegaDragger::mousePartAlive()
{
	return PartInstance::nonNullInWorkspace(mousePart.lock());
}

void MegaDragger::startDragging()
{
	DragUtilities::unJoinAndSetDragging(dragParts);

	if (mousePartAlive()) {
		rootInstance->setCameraIgnoreParts(mousePart.lock().get());
	}

	joined = false;
}

G3D::Vector3 MegaDragger::hitObjectOrPlane(const UIEvent& uiEvent)
{
	G3D::Vector3 hitPoint;
	G3D::Array<Primitive*> primitives;
	DragUtilities::partsToPrimitives(dragParts, primitives);

	DragUtilities::hitObjectOrPlane(
		*contactManager, MouseCommand::getUnitMouseRay(uiEvent, rootInstance), &primitives, hitPoint
	);

	return hitPoint;
}

void MegaDragger::alignAndCleanParts()
{
	if (mousePartAlive()) {
		PartInstance* part = mousePart.lock().get();

		if (!part->aligned()) {
			CoordinateFrame orgCoord = part->worldSnapLocation();
			CoordinateFrame alignedCoord = Math::snapToGrid(orgCoord, Dragger::dragSnap());
			DragUtilities::move(dragParts, orgCoord, alignedCoord);
		}
	}

	DragUtilities::clean(dragParts);
}

G3D::Vector3 MegaDragger::safeMoveYDrop(const G3D::Vector3& tryDrag)
{
	G3D::Array<Primitive*> primitives;
	DragUtilities::partsToPrimitives(dragParts, primitives);

	return Dragger::safeMoveYDrop(primitives, tryDrag, *contactManager);
}

G3D::Vector3 MegaDragger::safeMoveNoDrop(const G3D::Vector3& tryDrag)
{
	G3D::Array<Primitive*> primitives;
	DragUtilities::partsToPrimitives(dragParts, primitives);

	return Dragger::safeMoveNoDrop(primitives, tryDrag, *contactManager);
}

G3D::Vector3 MegaDragger::safeMoveAlongLine(const G3D::Vector3& tryDrag)
{
	G3D::Array<Primitive*> primitives;
	DragUtilities::partsToPrimitives(dragParts, primitives);

	if (primitives.size() > 0) {
		G3D::Vector3 answer = Dragger::safeMoveAlongLine(primitives, tryDrag, *contactManager);
		return answer;
	}

	return G3D::Vector3::zero();
}

void MegaDragger::safeRotate(const G3D::Matrix3& rotMatrix)
{
	G3D::Array<Primitive*> primitives;

	DragUtilities::partsToPrimitives(dragParts, primitives);

	Dragger::safeRotate(primitives, rotMatrix, *contactManager);
}

MegaDragger::MegaDragger(
	PartInstance* mousePartPtr, const std::vector<PVInstance*>& pvInstances, RootInstance* rootInstance
)
	: mousePart(shared_from(mousePartPtr)), joined(true), rootInstance(rootInstance),
	  contactManager(&rootInstance->getWorld()->getContactManager())
{
	DragUtilities::pvsToParts(pvInstances, dragParts);
}

MegaDragger::MegaDragger(
	PartInstance* mousePartPtr, const std::vector<boost::weak_ptr<PartInstance> >& partArray, RootInstance* rootInstance
)
	: mousePart(shared_from(mousePartPtr)), dragParts(partArray), joined(true), rootInstance(rootInstance),
	  contactManager(&rootInstance->getWorld()->getContactManager())
{
}

MegaDragger::~MegaDragger()
{
	if (!joined) {
		DragUtilities::joinAndStopDragging(dragParts);
	}
}

void MegaDragger::finishDragging()
{
	rootInstance->clearCameraIgnoreParts();

	DragUtilities::joinAndStopDragging(dragParts);

	if (DragUtilities::anyPartAlive(dragParts)) {
		rootInstance->setInsertPoint(DragUtilities::computeExtents(dragParts).topCenter());
	}

	joined = true;
}

} // namespace RBX
