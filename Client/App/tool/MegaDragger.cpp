#include "tool/MegaDragger.h"

#include "tool/DragUtilities.h"
#include "tool/Dragger.h"
#include "util/Math.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/RootInstance.h"
#include "v8world/SpatialHash.h"
#include "v8world/World.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x10116200
void MegaDragger::continueDragging()
{
	DragUtilities::unJoinAndSetDragging(dragParts);
}

// FUNCTION: WEBSERVICE 0x10116210
bool MegaDragger::mousePartAlive()
{
	return PartInstance::nonNullInWorkspace(mousePart.lock());
}

// FUNCTION: WEBSERVICE 0x10116230
void MegaDragger::startDragging()
{
	DragUtilities::unJoinAndSetDragging(dragParts);

	if (mousePartAlive()) {
		rootInstance->setCameraIgnoreParts(mousePart.lock().get());
	}

	joined = false;
}

// STUB: WEBSERVICE 0x101162f0
G3D::Vector3 MegaDragger::hitObjectOrPlane(const UIEvent& uiEvent)
{
	STUB(0x101162f0);
	return G3D::Vector3::zero();
}

// FUNCTION: WEBSERVICE 0x101163b0
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
G3D::Vector3 MegaDragger::safeMoveYDrop(const G3D::Vector3& tryDrag)
{
	G3D::Array<Primitive*> primitives;
	DragUtilities::partsToPrimitives(dragParts, primitives);

	return Dragger::safeMoveYDrop(primitives, tryDrag, *contactManager);
}

// FUNCTION: WEBSERVICE 0x101165b0
G3D::Vector3 MegaDragger::safeMoveNoDrop(const G3D::Vector3& tryDrag)
{
	G3D::Array<Primitive*> primitives;
	DragUtilities::partsToPrimitives(dragParts, primitives);

	return Dragger::safeMoveNoDrop(primitives, tryDrag, *contactManager);
}

// FUNCTION: WEBSERVICE 0x10116630
void MegaDragger::safeRotate(const G3D::Matrix3& rotMatrix)
{
	G3D::Array<Primitive*> primitives;

	DragUtilities::partsToPrimitives(dragParts, primitives);

	Dragger::safeRotate(primitives, rotMatrix, *contactManager);
}

// STUB: WEBSERVICE 0x10116790
MegaDragger::MegaDragger(
	PartInstance* mousePartPtr,
	const std::vector<PVInstance*>& pvInstances,
	RootInstance* rootInstance
)
	: mousePart(shared_from(mousePartPtr)), joined(true), rootInstance(rootInstance),
	  contactManager(rootInstance->getWorld()->getContactManager())
{
	DragUtilities::pvsToParts(pvInstances, dragParts);
}

// STUB: WEBSERVICE 0x10116870
MegaDragger::MegaDragger(
	PartInstance* mousePartPtr,
	const std::vector<boost::weak_ptr<PartInstance> >& partArray,
	RootInstance* rootInstance
)
	: mousePart(shared_from(mousePartPtr)), dragParts(partArray), joined(true), rootInstance(rootInstance),
	  contactManager(rootInstance->getWorld()->getContactManager())
{
}

// FUNCTION: WEBSERVICE 0x10116930
MegaDragger::~MegaDragger()
{
	if (!joined) {
		DragUtilities::joinAndStopDragging(dragParts);
	}
}

// FUNCTION: WEBSERVICE 0x101169e0
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
