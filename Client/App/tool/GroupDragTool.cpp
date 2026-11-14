#include "tool/GroupDragTool.h"

#include "tool/Dragger.h"
#include "util/Math.h"
#include "util/Name.h"
#include "util/UIEvent.h"
#include "v8datamodel/Workspace.h"

#include <G3D/Plane.h>

namespace RBX {

char sGroupDragTool[] = "GroupDragTool";

MouseCommand* GroupDragTool::onMouseDown(const UIEvent& uiEvent)
{
	downPoint = G3D::Vector2(uiEvent.mousePosition);
	capture();
	return this;
}

GroupDragTool::GroupDragTool(
	PartInstance* mousePart,
	const G3D::Vector3& hitPointWorld,
	const std::vector<boost::weak_ptr<PartInstance> >& partArray,
	Workspace* workspace
)
	: Named<MouseCommand, sGroupDragTool>(workspace), dragging(false)
{
	megaDragger.reset(new MegaDragger(mousePart, partArray, workspace));
}

void GroupDragTool::onMouseMove(const UIEvent& uiEvent)
{
	if (!dragging) {
		if ((G3D::Vector2(uiEvent.mousePosition) - downPoint).length() > 4.0f) {
			dragging = true;
			megaDragger->startDragging();
			megaDragger->alignAndCleanParts();
			lastHit = megaDragger->hitObjectOrPlane(uiEvent);
		}
	}

	onMouseIdle(uiEvent);
}

void GroupDragTool::onMouseIdle(const UIEvent& uiEvent)
{
	G3D::Vector3 hit;

	if (captured() && dragging && megaDragger->mousePartAlive() &&
		Math::intersectRayPlane(getSearchRay(uiEvent), G3D::Plane(G3D::Vector3::unitY(), G3D::Vector3::zero()), hit)) {
		hit = megaDragger->hitObjectOrPlane(uiEvent);

		megaDragger->continueDragging();

		G3D::Vector3 hitSnapped = Dragger::toGrid(hit);

		G3D::Vector3 delta = Dragger::toGrid(hitSnapped - lastHit);

		lastHit = hitSnapped;

		megaDragger->safeMoveYDrop(delta);
	}
}

MouseCommand* GroupDragTool::onMouseUp(const UIEvent& uiEvent)
{
	if (dragging) {
		megaDragger->finishDragging();
		dragging = false;
	}

	releaseCapture();
	return NULL;
}

MouseCommand* GroupDragTool::onKeyDown(const UIEvent& uiEvent)
{
	if (dragging) {
		switch (uiEvent.key) {
		case SDLK_r:
			megaDragger->safeRotate(Math::matrixRotateY());
			break;

		case SDLK_t:
			megaDragger->safeRotate(Math::matrixTiltZ());
			break;
		}

		return this;
	}

	return NULL;
}

GroupDragTool::~GroupDragTool()
{
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sGroupDragTool>();
template void RBX::Name::callDoDeclare<RBX::sGroupDragTool>();
