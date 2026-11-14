#ifndef GUI_WIDGET_H
#define GUI_WIDGET_H

#include "gui/GUI.h"
#include "placeholder.h"

#include <G3D/Color3.h>
#include <G3D/Color4.h>

namespace RBX {

class Verb;

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

	virtual void onLoseFocus() { widgetState = NOTHING; }

	virtual void render2d(Adorn* adorn);

	virtual void onClick(const GuiEvent& event) {}

	virtual bool onDrag(const GuiEvent& event) { return false; }

	virtual void onDown(const UIEvent& event) {}
	virtual void onUp(const UIEvent& event) {}

	virtual G3D::Color4 getFontColor() { return G3D::Color4(G3D::Color3::white(), 1); }

	virtual bool isEnabled() { return isVisible(); }

	virtual int getFontSize() const { return 12; }

	WidgetState widgetState;

private:
	GuiResponse processMouse(const GuiEvent& event);
	GuiResponse processKey(const GuiEvent& event);
};

class VerbWidget : public Widget
{
private:
	Verb* verb;

protected:
	Verb* getVerb() { return verb; }
	virtual bool isEnabled();
	virtual void onClick(const GuiEvent& p_unk0x04);

public:
	VerbWidget(Verb* p_unk0x04);
	VerbWidget();
	virtual ~VerbWidget();
};

} // namespace RBX

#endif // GUI_WIDGET_H
