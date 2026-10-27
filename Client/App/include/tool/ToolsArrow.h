#ifndef TOOL_TOOLSARROW_H
#define TOOL_TOOLSARROW_H

#include "decomp.h"
#include "util/Name.h"
#include "v8datamodel/MouseCommand.h"

#include <string>

namespace RBX {

class Workspace;

extern char sBoxSelectCommand[];

extern char sArrowTool[];

// SIZE 0x24
class __declspec(novtable) ArrowToolBase : public MouseCommand
{
protected:
	bool overInstance; // 0x20

	virtual void onMouseIdle(const UIEvent& uiEvent);
	virtual void onMouseHover(const UIEvent& uiEvent);
	virtual MouseCommand* onMouseDown(const UIEvent& uiEvent);
	virtual const std::string getCursorName() const;

public:
	ArrowToolBase(Workspace* workspace) : MouseCommand(workspace), overInstance(false) {}
	virtual ~ArrowToolBase() {}
};

DECOMP_SIZE_ASSERT(ArrowToolBase, 0x24)

// SIZE 0x24
class ArrowTool : public Named<ArrowToolBase, sArrowTool>
{
public:
	ArrowTool(Workspace* workspace) : Named<ArrowToolBase, sArrowTool>(workspace) {}

	// FUNCTION: WEBSERVICE 0x1006a710
	virtual MouseCommand* isSticky() const { return new ArrowTool(workspace); }
	virtual ~ArrowTool() {}
};

DECOMP_SIZE_ASSERT(ArrowTool, 0x24)

} // namespace RBX

#endif // TOOL_TOOLSARROW_H
