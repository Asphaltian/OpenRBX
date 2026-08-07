#include "gui/Widget.h"

namespace RBX {

// STUB: WEBSERVICE 0x10116aa0
DECOMP_NOINLINE GuiResponse Widget::processMouse(const GuiEvent& event)
{
	STUB(0x10116aa0);

	return GuiResponse::notUsed();
}

// STUB: WEBSERVICE 0x10116cd0
GuiResponse Widget::process(const GuiEvent& event)
{
	if (isEnabled() && event.isMouseEvent()) {
		return processMouse(event);
	}

	return GuiResponse::notUsed();
}

} // namespace RBX
