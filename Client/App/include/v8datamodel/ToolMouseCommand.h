#ifndef V8DATAMODEL_TOOLMOUSECOMMAND_H
#define V8DATAMODEL_TOOLMOUSECOMMAND_H

#include "placeholder.h"
#include "util/Name.h"
#include "v8datamodel/ScriptMouseCommand.h"

#include <boost/shared_ptr.hpp>

namespace RBX {

class Tool;

extern char sToolMouseCommand[];

class ToolMouseCommand : public Named<ScriptMouseCommand, sToolMouseCommand>
{
private:
	boost::shared_ptr<Tool> tool;

	void updateTargetPoint(const UIEvent& uiEvent);

public:
	ToolMouseCommand(Workspace* workspace, Tool* tool)
		: Named<ScriptMouseCommand, sToolMouseCommand>(workspace), tool(shared_from(tool))
	{
	}

	virtual ~ToolMouseCommand() {}

	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual void onMouseHover(const UIEvent& uiEvent);
	virtual void onMouseIdle(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseUp(const UIEvent& uiEvent);
};

} // namespace RBX

#endif // V8DATAMODEL_TOOLMOUSECOMMAND_H
