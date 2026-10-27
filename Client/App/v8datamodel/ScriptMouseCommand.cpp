#include "v8datamodel/ScriptMouseCommand.h"

#include "util/Name.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x100cac10 FOLDED
const Name& ScriptMouseCommand::getName() const
{
	return Name::declare<sMouse>();
}

// FUNCTION: WEBSERVICE 0x10116ff0
ScriptMouseCommand::~ScriptMouseCommand()
{
	mouse->setCommand(NULL);

	mouse->disconnect_all_slots();
}

// FUNCTION: WEBSERVICE 0x101170a0
TextureId ScriptMouseCommand::getCursorId() const
{
	return mouse->getIcon();
}

// FUNCTION: WEBSERVICE 0x101170c0 FOLDED
void ScriptMouseCommand::onMouseHover(const UIEvent& uiEvent)
{
	mouse->update(uiEvent);
}

// FUNCTION: WEBSERVICE 0x101170c0 FOLDED
void ScriptMouseCommand::onMouseIdle(const UIEvent& uiEvent)
{
	mouse->update(uiEvent);
}

// FUNCTION: WEBSERVICE 0x101170d0 FOLDED
MouseCommand* ScriptMouseCommand::onMouseDown(const UIEvent& uiEvent)
{
	mouse->update(uiEvent);
	return this;
}

// FUNCTION: WEBSERVICE 0x101170d0 FOLDED
MouseCommand* ScriptMouseCommand::onMouseUp(const UIEvent& uiEvent)
{
	mouse->update(uiEvent);
	return this;
}

// FUNCTION: WEBSERVICE 0x101170d0 FOLDED
MouseCommand* ScriptMouseCommand::onPeekKeyDown(const UIEvent& uiEvent)
{
	mouse->update(uiEvent);
	return this;
}

// FUNCTION: WEBSERVICE 0x10117110
ScriptMouseCommand::ScriptMouseCommand(Workspace* workspace) : MouseCommand(workspace)
{
	mouse = Creatable<Instance>::create<Mouse>();
	mouse->setCommand(this);
}

} // namespace RBX
