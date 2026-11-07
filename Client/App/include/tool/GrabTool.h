#ifndef TOOL_GRABTOOL_H
#define TOOL_GRABTOOL_H

#include "decomp.h"
#include "util/Name.h"
#include "v8datamodel/MouseCommand.h"

#include <string>

namespace RBX {

class Workspace;

extern char sGrabTool[];

// SIZE 0x3c
class GrabTool : public Named<MouseCommand, sGrabTool>
{
private:
	std::string cursor; // 0x20

	virtual void onMouseIdle(const UIEvent& uiEvent) { onMouseHover(uiEvent); }
	virtual void onMouseHover(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);

	// FUNCTION: ROBLOX 0x006227f0 FOLDED
	virtual const std::string getCursorName() const { return cursor; }

public:
	GrabTool(Workspace* workspace);
	virtual ~GrabTool();

	// FUNCTION: ROBLOX 0x0063be20
	virtual MouseCommand* isSticky() const { return new GrabTool(workspace); }

	// SYNTHETIC: ROBLOX 0x0063be00
	// RBX::GrabTool::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(GrabTool, 0x3c)

} // namespace RBX

#endif // TOOL_GRABTOOL_H
