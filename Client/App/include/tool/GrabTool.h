#ifndef TOOL_GRABTOOL_H
#define TOOL_GRABTOOL_H

#include "placeholder.h"
#include "util/Name.h"
#include "v8datamodel/MouseCommand.h"

#include <string>

namespace RBX {

class Workspace;

extern char sGrabTool[];

class GrabTool : public Named<MouseCommand, sGrabTool>
{
private:
	std::string cursor;

	virtual void onMouseIdle(const UIEvent& uiEvent) { onMouseHover(uiEvent); }
	virtual void onMouseHover(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);

	virtual const std::string getCursorName() const { return cursor; }

public:
	GrabTool(Workspace* workspace);
	virtual ~GrabTool();

	virtual MouseCommand* isSticky() const { return new GrabTool(workspace); }
};

} // namespace RBX

#endif // TOOL_GRABTOOL_H
