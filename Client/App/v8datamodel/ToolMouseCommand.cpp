#include "v8datamodel/ToolMouseCommand.h"

#include "humanoid/Humanoid.h"
#include "util/UIEvent.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Tool.h"

#include <G3D/Ray.h>

namespace RBX {

char sToolMouseCommand[] = "ToolMouseCommand";

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

MouseCommand* ToolMouseCommand::onMouseDown(const UIEvent& uiEvent)
{
	updateTargetPoint(uiEvent);
	tool->activate();

	return ScriptMouseCommand::onMouseDown(uiEvent);
}

void ToolMouseCommand::onMouseHover(const UIEvent& uiEvent)
{
	updateTargetPoint(uiEvent);

	ScriptMouseCommand::onMouseHover(uiEvent);
}

void ToolMouseCommand::onMouseIdle(const UIEvent& uiEvent)
{
	updateTargetPoint(uiEvent);

	ScriptMouseCommand::onMouseIdle(uiEvent);
}

MouseCommand* ToolMouseCommand::onMouseUp(const UIEvent& uiEvent)
{
	updateTargetPoint(uiEvent);
	tool->deactivate();

	return ScriptMouseCommand::onMouseDown(uiEvent);
}

} // namespace RBX
