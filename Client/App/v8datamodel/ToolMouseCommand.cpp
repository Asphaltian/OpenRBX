#include "v8datamodel/ToolMouseCommand.h"

#include "humanoid/Humanoid.h"
#include "util/UIEvent.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Tool.h"

#include <G3D/Ray.h>

namespace RBX {

char sToolMouseCommand[] = "ToolMouseCommand";

// STUB: WEBSERVICE 0x101171e0
// STUB: ROBLOX 0x00656970
void ToolMouseCommand::updateTargetPoint(const UIEvent& uiEvent)
{
	G3D::Vector3 hitWorld(0, 0, 0);

	if (Humanoid* humanoid = Humanoid::getLocalHumanoidFromContext(tool.get())) {
		if (!getPartByLocalCharacter(uiEvent, hitWorld)) {
			G3D::Ray ray = getSearchRay(uiEvent);
			hitWorld = humanoid->getHead() ? humanoid->getHead()->getCoordinateFrame().translation : ray.origin;
			hitWorld += ray.direction;
		}

		humanoid->setTargetPoint(hitWorld);
	}
}

// FUNCTION: WEBSERVICE 0x101172d0
// FUNCTION: ROBLOX 0x00656a60
MouseCommand* ToolMouseCommand::onMouseDown(const UIEvent& uiEvent)
{
	updateTargetPoint(uiEvent);
	tool->activate();

	return ScriptMouseCommand::onMouseDown(uiEvent);
}

// FUNCTION: WEBSERVICE 0x10117300 FOLDED
// FUNCTION: ROBLOX 0x00656a90 FOLDED
void ToolMouseCommand::onMouseHover(const UIEvent& uiEvent)
{
	updateTargetPoint(uiEvent);

	ScriptMouseCommand::onMouseHover(uiEvent);
}

// FUNCTION: WEBSERVICE 0x10117300 FOLDED
// FUNCTION: ROBLOX 0x00656a90 FOLDED
void ToolMouseCommand::onMouseIdle(const UIEvent& uiEvent)
{
	updateTargetPoint(uiEvent);

	ScriptMouseCommand::onMouseIdle(uiEvent);
}

// FUNCTION: WEBSERVICE 0x10117320
// FUNCTION: ROBLOX 0x00656ab0
MouseCommand* ToolMouseCommand::onMouseUp(const UIEvent& uiEvent)
{
	updateTargetPoint(uiEvent);
	tool->deactivate();

	return ScriptMouseCommand::onMouseDown(uiEvent);
}

} // namespace RBX
