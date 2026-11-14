#include "tool/DragTool.h"

#include "appdraw/HitTest.h"
#include "tool/DragUtilities.h"
#include "tool/GroupDragTool.h"
#include "tool/PartDragTool.h"

#include <G3D/CollisionDetection.h>
#include <G3D/Sphere.h>

namespace RBX {

MouseCommand* DragTool::onMouseDown(
	PartInstance* hitPart,
	const G3D::Vector3& hitWorld,
	const std::vector<Instance*>& dragInstances,
	const UIEvent& uiEvent,
	Workspace* workspace
)
{
	std::vector<boost::weak_ptr<PartInstance> > partArray;
	DragUtilities::instancesToParts(dragInstances, partArray);

	MouseCommand* dragger;
	if (partArray.size() == 1) {
		dragger = new PartDragTool(hitPart, hitWorld, workspace);
	}
	else {
		dragger = new GroupDragTool(hitPart, hitWorld, partArray, workspace);
	}

	return dragger->onMouseDown(uiEvent);
}

bool FUN_00662530(
	const Extents& extents,
	const CoordinateFrame& location,
	const G3D::Ray& ray,
	G3D::Vector3& hitPoint,
	NormalId& normalId
)
{
	for (int i = 0; i < 6; i++) {
		G3D::Vector3 local = extents.faceCenter((NormalId) i);
		local += normalIdToVector3((NormalId) i) * HitTest::handleOffset();
		G3D::Vector3 handle = location.pointToWorldSpace(local);
		float radius = HitTest::distanceToHandleRadius(ray.origin, handle);
		G3D::Vector3 hitLocation;
		if (G3D::CollisionDetection::collisionTimeForMovingPointFixedSphere(
				ray.origin, ray.direction, G3D::Sphere(handle, radius), hitLocation
			) != G3D::inf()) {
			normalId = (NormalId) i;
			hitPoint = handle;
			return true;
		}
	}

	return false;
}

bool FUN_00662700(const Extents& extents, const G3D::Ray& ray, G3D::Vector3& hitPoint, NormalId& normalId)
{
	CoordinateFrame location(extents.center());
	return FUN_00662530(
		Extents::fromCenterCorner(G3D::Vector3::zero(), extents.size() * 0.5f), location, ray, hitPoint, normalId
	);
}

} // namespace RBX
