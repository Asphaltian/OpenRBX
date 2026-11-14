#include "v8datamodel/ScriptMouseCommand.h"

#include "util/Name.h"

namespace RBX {

const Name& ScriptMouseCommand::getName() const
{
	return Name::declare<sMouse>();
}

ScriptMouseCommand::~ScriptMouseCommand()
{
	mouse->setCommand(NULL);

	mouse->disconnect_all_slots();
}

TextureId ScriptMouseCommand::getCursorId() const
{
	return mouse->getIcon();
}

void ScriptMouseCommand::onMouseHover(const UIEvent& uiEvent)
{
	mouse->update(uiEvent);
}

void ScriptMouseCommand::onMouseIdle(const UIEvent& uiEvent)
{
	mouse->update(uiEvent);
}

MouseCommand* ScriptMouseCommand::onMouseDown(const UIEvent& uiEvent)
{
	mouse->update(uiEvent);
	return this;
}

MouseCommand* ScriptMouseCommand::onMouseUp(const UIEvent& uiEvent)
{
	mouse->update(uiEvent);
	return this;
}

MouseCommand* ScriptMouseCommand::onPeekKeyDown(const UIEvent& uiEvent)
{
	mouse->update(uiEvent);
	return this;
}

ScriptMouseCommand::ScriptMouseCommand(Workspace* workspace) : MouseCommand(workspace)
{
	mouse = Creatable<Instance>::create<Mouse>();
	mouse->setCommand(this);
}

} // namespace RBX
