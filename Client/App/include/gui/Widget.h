#ifndef GUI_WIDGET_H
#define GUI_WIDGET_H

#include "decomp.h"
#include "gui/GUI.h"

#include <G3D/Color3.h>
#include <G3D/Color4.h>

namespace RBX {

// SIZE 0x110
class Widget : public GuiItem
{
public:
	enum WidgetState
	{
		NOTHING = 0,
		HOVER = 1,
		DOWN_OVER = 2,
		DOWN_AWAY = 3
	};

public:
	Widget();

protected:
	virtual GuiResponse process(const GuiEvent& event);

	// FUNCTION: WEBSERVICE 0x100c74d0
	virtual void onLoseFocus() { widgetState = NOTHING; }

	virtual void render2d(Adorn* adorn);

	virtual void onClick(const GuiEvent& event) {} // vtable+0x64

	// FUNCTION: WEBSERVICE 0x100c74e0 FOLDED
	virtual bool onDrag(const GuiEvent& event) { return false; } // vtable+0x68

	virtual void onDown(const UIEvent& event) {} // vtable+0x6c
	virtual void onUp(const UIEvent& event) {}   // vtable+0x70

	// FUNCTION: WEBSERVICE 0x100c7500
	virtual G3D::Color4 getFontColor() { return G3D::Color4(G3D::Color3::white(), 1); } // vtable+0x74

	// FUNCTION: WEBSERVICE 0x100c7530
	virtual bool isEnabled() { return isVisible(); } // vtable+0x78

	// FUNCTION: WEBSERVICE 0x100c74f0
	virtual int getFontSize() const { return 12; }

	WidgetState widgetState; // 0x10c

private:
	GuiResponse processMouse(const GuiEvent& event);
	GuiResponse processKey(const GuiEvent& event);
};

DECOMP_SIZE_ASSERT(Widget, 0x110)

} // namespace RBX

#endif // GUI_WIDGET_H
