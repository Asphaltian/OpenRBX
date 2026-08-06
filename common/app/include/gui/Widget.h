#ifndef GUI_WIDGET_H
#define GUI_WIDGET_H

#include "decomp.h"
#include "gui/GUI.h"

namespace RBX {

// SIZE 0x110
class __declspec(novtable) Widget : public GuiItem
{
public:
	enum WidgetState
	{
		NOTHING = 0,
		HOVER = 1,
		DOWN_OVER = 2,
		DOWN_AWAY = 3
	};

protected:
	virtual GuiResponse process(const GuiEvent& event);
	virtual void onLoseFocus();
	virtual void render2d(Adorn* adorn);

	virtual void onClick(); // vtable+0x64

	WidgetState widgetState; // 0x10c

private:
	GuiResponse processMouse(const GuiEvent& event);
	GuiResponse processKey(const GuiEvent& event);
};

DECOMP_SIZE_ASSERT(Widget, 0x110)

} // namespace RBX

#endif // GUI_WIDGET_H
