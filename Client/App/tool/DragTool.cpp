#include "tool/DragTool.h"

#include "tool/DragUtilities.h"
#include "tool/GroupDragTool.h"
#include "tool/PartDragTool.h"

namespace RBX {

// STUB: WEBSERVICE 0x101160e0
// STUB: ROBLOX 0x00662410
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

} // namespace RBX
