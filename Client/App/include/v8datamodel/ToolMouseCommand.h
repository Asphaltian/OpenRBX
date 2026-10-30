#ifndef V8DATAMODEL_TOOLMOUSECOMMAND_H
#define V8DATAMODEL_TOOLMOUSECOMMAND_H

#include "decomp.h"
#include "util/Name.h"
#include "v8datamodel/ScriptMouseCommand.h"

#include <boost/shared_ptr.hpp>

namespace RBX {

class Tool;

extern char sToolMouseCommand[];

// SIZE 0x30
class ToolMouseCommand : public Named<ScriptMouseCommand, sToolMouseCommand>
{
private:
	boost::shared_ptr<Tool> tool; // 0x28

	void updateTargetPoint(const UIEvent& uiEvent);

public:
	// FUNCTION: WEBSERVICE 0x100cac30
	// FUNCTION: ROBLOX 0x0060f970
	ToolMouseCommand(Workspace* workspace, Tool* tool)
		: Named<ScriptMouseCommand, sToolMouseCommand>(workspace), tool(shared_from(tool))
	{
	}

	// FUNCTION: WEBSERVICE 0x100caca0
	// FUNCTION: ROBLOX 0x0060f9e0
	virtual ~ToolMouseCommand() {}

	// SYNTHETIC: WEBSERVICE 0x100cad30
	// SYNTHETIC: ROBLOX 0x0060fa70
	// RBX::ToolMouseCommand::`scalar deleting destructor'

	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual void onMouseHover(const UIEvent& uiEvent);
	virtual void onMouseIdle(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseUp(const UIEvent& uiEvent);
};

DECOMP_SIZE_ASSERT(ToolMouseCommand, 0x30)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x100c9dd0
// RBX::Named<RBX::ScriptMouseCommand,&RBX::sToolMouseCommand>::~Named<RBX::ScriptMouseCommand,&RBX::sToolMouseCommand>
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_TOOLMOUSECOMMAND_H
