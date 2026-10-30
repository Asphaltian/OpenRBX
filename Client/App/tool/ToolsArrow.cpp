#include "tool/ToolsArrow.h"

#include "rbxgraphics/Adorn.h"
#include "tool/DragTool.h"
#include "util/ISelectable3d.h"
#include "util/Name.h"
#include "util/UIEvent.h"
#include "util/UserInputBase.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Workspace.h"

#include <algorithm>

namespace RBX {

char sBoxSelectCommand[] = "BoxSelect";

char sArrowTool[] = "Arrow";

// FUNCTION: WEBSERVICE 0x100bf0e0
// FUNCTION: ROBLOX 0x0063c600
void ArrowToolBase::onMouseHover(const UIEvent& uiEvent)
{
	onMouseIdle(uiEvent);
}

// FUNCTION: WEBSERVICE 0x100bf0f0
// FUNCTION: ROBLOX 0x00620d50
void ArrowToolBase::onMouseIdle(const UIEvent& uiEvent)
{
	overInstance = getUnlockedPart(uiEvent) != NULL;
}

// FUNCTION: WEBSERVICE 0x100bf110
// FUNCTION: ROBLOX 0x00620d70
const std::string ArrowToolBase::getCursorName() const
{
	return overInstance ? "DragCursor" : "ArrowCursor";
}

// FUNCTION: WEBSERVICE 0x100bf3a0
// FUNCTION: ROBLOX 0x00620fa0
void BoxSelectCommand::render2d(Adorn* adorn)
{
	G3D::Rect2D temp = G3D::Rect2D::xyxy(mouseDownView.x, mouseDownView.y, mouseCurrentView.x, mouseCurrentView.y);

	adorn->outlineRect2d(temp, 0.5f, G3D::Color4(G3D::Color3::gray()));
}

// FUNCTION: WEBSERVICE 0x100bfe60
// FUNCTION: ROBLOX 0x00621880
void BoxSelectCommand::getMouseInstances(std::set<Instance*>& instances, const UIEvent& uiEvent, G3D::Rect2D& selectBox)
{
	Camera* camera = workspace->getCamera();
	G3D::Rect2D viewport = G3D::Rect2D::xyxy(0, 0, uiEvent.windowSize.x, uiEvent.windowSize.y);

	for (unsigned int i = 0; i < workspace->numChildren(); ++i) {
		ILocation* location = dynamic_cast<ILocation*>(workspace->getChild(i));
		ISelectable3d* selectable = dynamic_cast<ISelectable3d*>(workspace->getChild(i));
		if (location && selectable) {
			Instance* instance = workspace->getChild(i);
			if (!PartInstance::getLocked(instance)) {
				G3D::Vector3 gridPos = location->getLocation().translation;
				G3D::Vector3 projection = camera->getGCamera().project(gridPos, viewport);
				if (selectBox.contains(projection.xy())) {
					instances.insert(instance);
				}
			}
		}
	}
}

// FUNCTION: WEBSERVICE 0x100c0070
// FUNCTION: ROBLOX 0x00621a90
BoxSelectCommand::BoxSelectCommand(Workspace* workspace)
	: Named<MouseCommand, sBoxSelectCommand>(workspace), selection(workspace), mouseDownView(0, 0),
	  mouseCurrentView(0, 0)
{
}

// FUNCTION: WEBSERVICE 0x100c0790
// FUNCTION: ROBLOX 0x006221b0
MouseCommand* ArrowToolBase::onMouseDown(const UIEvent& uiEvent)
{
	G3D::Vector3 hitWorld;
	PartInstance* hitPart = getUnlockedPart(uiEvent, hitWorld);

	if (hitPart) {
		Instance* topSelectable = getTopSelectable3d(hitPart);
		UserInputBase* userInput = uiEvent.userInput;

		ServiceClient<Selection> selection(workspace);

		if (userInput->shiftKeyDown() || userInput->ctrlKeyDown()) {
			if (selection->isSelected(topSelectable)) {
				selection->removeFromSelection(topSelectable);
			}
			else {
				selection->addToSelection(topSelectable);
			}
		}
		else {
			if (!selection->isSelected(topSelectable)) {
				selection->setSelection(topSelectable);
			}

			if (uiEvent.eventType == UIEvent::MOUSE_LEFT_BUTTON_DOWN) {
				ServiceClient<FilteredSelection<Instance> > instanceSelection(workspace);

				return DragTool::onMouseDown(hitPart, hitWorld, instanceSelection->items(), uiEvent, workspace);
			}
		}
	}
	else {
		MouseCommand* boxSelect = new BoxSelectCommand(workspace);
		return boxSelect->onMouseDown(uiEvent);
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x100c09e0
// FUNCTION: ROBLOX 0x00622400
void BoxSelectCommand::selectAnd(const std::set<Instance*>& newItemsInBox)
{
	std::set_difference(
		newItemsInBox.begin(),
		newItemsInBox.end(),
		previousItemsInBox.begin(),
		previousItemsInBox.end(),
		Selection::AddIterator(selection)
	);

	std::set_difference(
		previousItemsInBox.begin(),
		previousItemsInBox.end(),
		newItemsInBox.begin(),
		newItemsInBox.end(),
		Selection::RemoveIterator(selection)
	);

	previousItemsInBox = newItemsInBox;
}

// FUNCTION: WEBSERVICE 0x100c0ab0
// FUNCTION: ROBLOX 0x006224d0
void BoxSelectCommand::selectReverse(const std::set<Instance*>& newItemsInBox)
{
	std::set_difference(
		newItemsInBox.begin(),
		newItemsInBox.end(),
		previousItemsInBox.begin(),
		previousItemsInBox.end(),
		Selection::ToggleIterator(selection)
	);

	std::set_difference(
		previousItemsInBox.begin(),
		previousItemsInBox.end(),
		newItemsInBox.begin(),
		newItemsInBox.end(),
		Selection::ToggleIterator(selection)
	);

	previousItemsInBox = newItemsInBox;
}

// FUNCTION: WEBSERVICE 0x100c0b80
// FUNCTION: ROBLOX 0x006225a0
MouseCommand* BoxSelectCommand::onMouseDown(const UIEvent& uiEvent)
{
	previousItemsInBox.clear();

	capture();

	reverseSelecting = uiEvent.userInput->shiftKeyDown();
	if (!reverseSelecting) {
		selection->clearSelection();
	}

	mouseDownView = uiEvent.mousePosition;
	mouseCurrentView = uiEvent.mousePosition;

	return this;
}

// FUNCTION: WEBSERVICE 0x100c0c20
// FUNCTION: ROBLOX 0x00622640
void BoxSelectCommand::onMouseMove(const UIEvent& uiEvent)
{
	mouseCurrentView = uiEvent.mousePosition;

	std::set<Instance*> instances;

#ifdef COMPAT_MODE
	G3D::Rect2D selectBox = G3D::Rect2D::xyxy(mouseDownView.x, mouseDownView.y, mouseCurrentView.x, mouseCurrentView.y);
	getMouseInstances(instances, uiEvent, selectBox);
#else
	getMouseInstances(
		instances, uiEvent, G3D::Rect2D::xyxy(mouseDownView.x, mouseDownView.y, mouseCurrentView.x, mouseCurrentView.y)
	);
#endif

	if (reverseSelecting) {
		selectReverse(instances);
	}
	else {
		selectAnd(instances);
	}
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sBoxSelectCommand>();
template void RBX::Name::callDoDeclare<RBX::sBoxSelectCommand>();
