#include "tool/PartDragTool.h"

#include "rbxgraphics/Adorn.h"
#include "util/Name.h"
#include "util/UIEvent.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Workspace.h"

#include <vector>

namespace RBX {

char sPartDragTool[] = "PartDragTool";

// FUNCTION: WEBSERVICE 0x10126360
// FUNCTION: ROBLOX 0x00662820
MouseCommand* PartDragTool::onMouseDown(const UIEvent& uiEvent)
{
	downPoint = G3D::Vector2(uiEvent.mousePosition);
	capture();
	return this;
}

// FUNCTION: WEBSERVICE 0x10126470
// FUNCTION: ROBLOX 0x00662930
void PartDragTool::onMouseDelta(const UIEvent& uiEvent)
{
	if (!dragging && megaDragger->mousePartAlive()) {
		dragging = true;
		megaDragger->startDragging();

		runDragger->init(workspace, megaDragger->getMousePart(), hitWorld);
	}

	onMouseIdle(uiEvent);
}

// FUNCTION: WEBSERVICE 0x101264d0
// FUNCTION: ROBLOX 0x00662990
void PartDragTool::onMouseIdle(const UIEvent& uiEvent)
{
	if (captured() && dragging && megaDragger->mousePartAlive()) {
		megaDragger->continueDragging();

		runDragger->snap(getUnitMouseRay(uiEvent));
	}
}

// FUNCTION: WEBSERVICE 0x10126540
// FUNCTION: ROBLOX 0x00662a00
MouseCommand* PartDragTool::onMouseUp(const UIEvent& uiEvent)
{
	if (dragging) {
		if (megaDragger->mousePartAlive()) {
			runDragger->snap(getUnitMouseRay(uiEvent));
		}

		megaDragger->finishDragging();
		dragging = false;
	}

	releaseCapture();

	return NULL;
}

// FUNCTION: WEBSERVICE 0x101265d0
// FUNCTION: ROBLOX 0x00662a90
PartDragTool::~PartDragTool()
{
}

// FUNCTION: WEBSERVICE 0x101266b0
// FUNCTION: ROBLOX 0x00662b70
void PartDragTool::onMouseMove(const UIEvent& uiEvent)
{
	if (!dragging && megaDragger->mousePartAlive()) {
		if ((G3D::Vector2(uiEvent.mousePosition) - downPoint).length() > 4.0f) {
			dragging = true;
			megaDragger->startDragging();

			runDragger->init(workspace, megaDragger->getMousePart(), hitWorld);
		}
	}

	if (dragging && megaDragger->mousePartAlive()) {
		megaDragger->getMousePart().lock().get();
	}

	onMouseIdle(uiEvent);
}

// FUNCTION: WEBSERVICE 0x10126800
// FUNCTION: ROBLOX 0x00662cc0
MouseCommand* PartDragTool::onKeyDown(const UIEvent& uiEvent)
{
	if (megaDragger->mousePartAlive()) {
		if (dragging) {
			switch (uiEvent.key) {
			case SDLK_r:
				RunDragger::rotatePart(megaDragger->getMousePart().lock().get());
				break;

			case SDLK_t:
				RunDragger::tiltPart(
					megaDragger->getMousePart().lock().get(), workspace->getCamera()->getCameraCoordinateFrame()
				);
				break;
			}
		}

		return this;
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x10126940
// FUNCTION: ROBLOX 0x00662e00
void PartDragTool::render3dAdorn(Adorn* adorn)
{
	if (megaDragger->mousePartAlive()) {
		megaDragger->getMousePart().lock()->render3dSelect(adorn, SELECT_NORMAL);
	}
}

// FUNCTION: WEBSERVICE 0x10126a40
// STUB: ROBLOX 0x00662f00
PartDragTool::PartDragTool(PartInstance* mousePart, const G3D::Vector3& hitPointWorld, Workspace* workspace)
	: Named<MouseCommand, sPartDragTool>(workspace), dragging(false), hitWorld(hitPointWorld)
{
	runDragger.reset(new RunDragger());

	std::vector<PVInstance*> pvInstances;
	pvInstances.push_back(mousePart);

	megaDragger.reset(new MegaDragger(mousePart, pvInstances, workspace));
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sPartDragTool>();
template void RBX::Name::callDoDeclare<RBX::sPartDragTool>();
