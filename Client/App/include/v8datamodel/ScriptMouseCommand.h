#ifndef V8DATAMODEL_SCRIPTMOUSECOMMAND_H
#define V8DATAMODEL_SCRIPTMOUSECOMMAND_H

#include "decomp.h"
#include "v8datamodel/Mouse.h"
#include "v8datamodel/MouseCommand.h"

#include <boost/shared_ptr.hpp>

namespace RBX {

// SIZE 0x28
class ScriptMouseCommand : public MouseCommand
{
private:
	boost::shared_ptr<Mouse> mouse; // 0x20

public:
	ScriptMouseCommand(Workspace* workspace);
	virtual ~ScriptMouseCommand();

	// FUNCTION: WEBSERVICE 0x100c7840
	// FUNCTION: ROBLOX 0x005e1a80
	boost::shared_ptr<Mouse> getMouse() { return mouse; }

	virtual TextureId getCursorId() const;
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual void onMouseHover(const UIEvent& uiEvent);
	virtual void onMouseIdle(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseUp(const UIEvent& uiEvent);
	virtual MouseCommand* onPeekKeyDown(const UIEvent& uiEvent);
	virtual const Name& getName() const;

	// SYNTHETIC: WEBSERVICE 0x101170f0
	// RBX::ScriptMouseCommand::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(ScriptMouseCommand, 0x28)

} // namespace RBX

#endif // V8DATAMODEL_SCRIPTMOUSECOMMAND_H
