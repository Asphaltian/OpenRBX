#include "tool/ToolsArrow.h"

#include "util/Name.h"
#include "util/UIEvent.h"

namespace RBX {

char sBoxSelectCommand[] = "BoxSelect";

char sArrowTool[] = "Arrow";

// FUNCTION: WEBSERVICE 0x100bf0e0
void ArrowToolBase::onMouseHover(const UIEvent& uiEvent)
{
	onMouseIdle(uiEvent);
}

// FUNCTION: WEBSERVICE 0x100bf0f0
void ArrowToolBase::onMouseIdle(const UIEvent& uiEvent)
{
	overInstance = getUnlockedPart(uiEvent) != NULL;
}

// FUNCTION: WEBSERVICE 0x100bf110
const std::string ArrowToolBase::getCursorName() const
{
	return overInstance ? "DragCursor" : "ArrowCursor";
}

// STUB: WEBSERVICE 0x100c0790
MouseCommand* ArrowToolBase::onMouseDown(const UIEvent& uiEvent)
{
	STUB(0x100c0790);
	return NULL;
}

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sBoxSelectCommand>();
template void RBX::Name::callDoDeclare<RBX::sBoxSelectCommand>();
