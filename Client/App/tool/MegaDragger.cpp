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

// FUNCTION: WEBSERVICE 0x10116200
// FUNCTION: ROBLOX 0x0061cfa0
void MegaDragger::continueDragging()
{
	DragUtilities::unJoinAndSetDragging(dragParts);
}

// FUNCTION: WEBSERVICE 0x10116210
// FUNCTION: ROBLOX 0x0061d020
bool MegaDragger::mousePartAlive()
{
	return PartInstance::nonNullInWorkspace(mousePart.lock());
}

// FUNCTION: WEBSERVICE 0x10116230
// FUNCTION: ROBLOX 0x0061d040
void MegaDragger::startDragging()
{
	DragUtilities::unJoinAndSetDragging(dragParts);

	if (mousePartAlive()) {
		rootInstance->setCameraIgnoreParts(mousePart.lock().get());
	}

	joined = false;
}

// FUNCTION: WEBSERVICE 0x101162f0
// FUNCTION: ROBLOX 0x0061d100
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

// FUNCTION: WEBSERVICE 0x101163b0
// FUNCTION: ROBLOX 0x0061d1c0
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

// FUNCTION: WEBSERVICE 0x10116530
// FUNCTION: ROBLOX 0x0061d340
G3D::Vector3 MegaDragger::safeMoveYDrop(const G3D::Vector3& tryDrag)
{
	G3D::Array<Primitive*> primitives;
	DragUtilities::partsToPrimitives(dragParts, primitives);

	return Dragger::safeMoveYDrop(primitives, tryDrag, *contactManager);
}

// FUNCTION: WEBSERVICE 0x101165b0
// FUNCTION: ROBLOX 0x0061d3c0
G3D::Vector3 MegaDragger::safeMoveNoDrop(const G3D::Vector3& tryDrag)
{
	G3D::Array<Primitive*> primitives;
	DragUtilities::partsToPrimitives(dragParts, primitives);

	return Dragger::safeMoveNoDrop(primitives, tryDrag, *contactManager);
}

// FUNCTION: WEBSERVICE 0x10116630
// FUNCTION: ROBLOX 0x0061d550
void MegaDragger::safeRotate(const G3D::Matrix3& rotMatrix)
{
	G3D::Array<Primitive*> primitives;

	DragUtilities::partsToPrimitives(dragParts, primitives);

	Dragger::safeRotate(primitives, rotMatrix, *contactManager);
}

// FUNCTION: WEBSERVICE 0x10116790
// FUNCTION: ROBLOX 0x0061d730
MegaDragger::MegaDragger(
	PartInstance* mousePartPtr, const std::vector<PVInstance*>& pvInstances, RootInstance* rootInstance
)
	: mousePart(shared_from(mousePartPtr)), joined(true), rootInstance(rootInstance),
	  contactManager(&rootInstance->getWorld()->getContactManager())
{
	DragUtilities::pvsToParts(pvInstances, dragParts);
}

// FUNCTION: WEBSERVICE 0x10116870
// FUNCTION: ROBLOX 0x0061d810
MegaDragger::MegaDragger(
	PartInstance* mousePartPtr, const std::vector<boost::weak_ptr<PartInstance> >& partArray, RootInstance* rootInstance
)
	: mousePart(shared_from(mousePartPtr)), dragParts(partArray), joined(true), rootInstance(rootInstance),
	  contactManager(&rootInstance->getWorld()->getContactManager())
{
}

// FUNCTION: WEBSERVICE 0x10116930
// FUNCTION: ROBLOX 0x0061d8d0
MegaDragger::~MegaDragger()
{
	if (!joined) {
		DragUtilities::joinAndStopDragging(dragParts);
	}
}

// FUNCTION: WEBSERVICE 0x101169e0
// FUNCTION: ROBLOX 0x0061d980
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
