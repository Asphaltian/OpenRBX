#ifndef GUI_GUI_H
#define GUI_GUI_H

#include "gui/GuiEvent.h"
#include "gui/Layout.h"
#include "rbxgraphics/Adorn.h"
#include "placeholder.h"
#include "util/Name.h"
#include "util/Rect.h"
#include "v8tree/Instance.h"

#include <G3D/Color4.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/Matrix4.h>
#include <G3D/Rect2D.h>
#include <G3D/Vector2.h>
#include <G3D/Vector2int16.h>

namespace RBX {

class GuiItem : public Instance, public GuiTarget
{
public:
	GuiItem();

private:
	virtual bool askAddChild(const Instance* instance) const { return dynamic_cast<const GuiItem*>(instance) != NULL; }

	virtual const Name& getClassName() const { return Name::getNullName(); }

public:

protected:
	virtual void onLoseFocus() {}
	virtual bool canLoseFocus() { return false; }

	virtual G3D::Vector2 getPosition() const { return getGuiParent()->getChildPosition(this); }

	virtual G3D::Vector2 getChildPosition(const GuiItem* child) const { return G3D::Vector2::zero(); }

	virtual int getFontSize() const { return 14; }

	virtual bool isVisible() const { return true; }

	virtual std::string getTitle() { return getName(); }

public:
	virtual G3D::Vector2 getSize() const { return guiSize; }

	virtual void render2d(Adorn* adorn) {}

	virtual GuiResponse process(const GuiEvent& event);

	void addGuiItem(shared_ptr<GuiItem> item) { item->setParent(this); }

	void setGuiSize(const G3D::Vector2& value) { guiSize = value; }
	const G3D::Vector2& getGuiSize() const { return guiSize; }

	const GuiItem* getGuiParent() const;
	GuiItem* getGuiParent();

	const GuiItem* getGuiItem(int index) const;
	GuiItem* getGuiItem(int index);

	static const G3D::Color4& enabledFill();
	static const G3D::Color4& disabledFill();
	static const G3D::Color4& menuEnabledFill();
	static const G3D::Color4& menuDisabledFill();
	static const G3D::Color4& translucentBorder();
	static const G3D::Color4& menuSelect();
	static const G3D::Color4& menuBackground();
	static const G3D::Color4& fontBorder();
	static const G3D::Color4& translucentBackdrop();
	static const G3D::Color4& toolboxColor();

protected:
	GuiItem* getFocus() { return focus.get(); }
	void loseFocus();

	Rect getMyRect() const;

	void label2d(
		Adorn* adorn, const std::string& label, const G3D::Color4& fill, const G3D::Color4& border, Adorn::XAlign align
	) const;
	G3D::Rect2D getMyRect2D() const { return getMyRect().toRect2D(); }

private:
	GuiResponse processNonFocus(const GuiEvent& event);
	void switchFocus(GuiItem* item);

	virtual void onDescendentRemoving(const shared_ptr<Instance>& instance);

	shared_ptr<GuiItem> focus;
	G3D::Vector2 guiSize;
};

class GuiRoot : public GuiItem
{
	static bool preProcess(const GuiEvent&);

public:
	GuiRoot();

	static void setCanvasSize(const G3D::Vector2& value) { canvasSize = value; }
	static G3D::Vector2 getCanvasSize() { return canvasSize; }
	static G3D::Vector2 toPixelSize(const G3D::Vector2& size);
	static int normalizedFontSize(int size);

	virtual void render2d(Adorn* adorn);
	static GuiResponse processTarget(const GuiEvent&, GuiTarget*);
	static void render2dItem(Adorn*, GuiItem*);

protected:
	virtual bool askSetParent(const Instance* instance) const { return true; }

private:
	virtual G3D::Vector2 getSize() const { return canvasSize; }

	static G3D::Vector2 canvasSize;
	static G3D::Matrix4 oldProjectionMatrix;
	static G3D::CoordinateFrame oldCameraWorld;
};

class TopMenuBar : public GuiItem
{
public:
	TopMenuBar() { init(); }
	TopMenuBar(const std::string& p_unk0x04, Layout::Style p_unk0x08, bool p_unk0x0c = false);
	TopMenuBar(const std::string& p_unk0x04, Layout::Style p_unk0x08, G3D::Color4 p_unk0x0c);

	virtual GuiResponse process(const GuiEvent& event);
	virtual void render2d(Adorn* adorn);
	virtual G3D::Vector2 getSize() const;
	virtual bool isVisible() const { return visible; }

	void setVisible(bool value) { visible = value; }

protected:
	virtual G3D::Vector2 getChildPosition(const GuiItem* child) const;

	G3D::Color4 backdropColor;
	Layout::Style layoutStyle;
	bool visible;

private:
	void init();
};

class RelativePanel : public TopMenuBar
{
public:
	RelativePanel() : offset(0, 0)
	{
		Layout layout;
		init(layout);
	}

	RelativePanel(const Layout& layout) : offset(0, 0) { init(layout); }

	virtual G3D::Vector2 getPosition() const;

protected:
	void init(const Layout& layout);

	Rect::Location xLocation;
	Rect::Location yLocation;
	G3D::Vector2int16 offset;
};

class PercentPanel : public TopMenuBar
{
public:
	PercentPanel(Rect::Location xLocation, Rect::Location yLocation, const G3D::Vector2 position)
		: xLocation(xLocation), yLocation(yLocation), position(position)
	{
	}

	virtual G3D::Vector2 getPosition() const;

protected:
	Rect::Location xLocation;
	Rect::Location yLocation;
	G3D::Vector2 position;
};

class UnifiedWidget : public GuiItem
{
public:
	enum MenuState
	{
		NOTHING = 0,
		HOVER = 1,
		SHOWN_APPEARING = 2,
		SHOWN = 3
	};

private:
	MenuState menuState;

	GuiResponse processShown_InTitle(const GuiEvent& p_unk0x04);
	GuiResponse processShown_OutOfTitle(const GuiEvent& p_unk0x04);
	GuiResponse processNothing(const GuiEvent& p_unk0x04);
	GuiResponse processHover(const GuiEvent& p_unk0x04);
	GuiResponse processShown(const GuiEvent& p_unk0x04);
	GuiResponse processKey(const GuiEvent& p_unk0x04);
	void render2dChildren(Adorn* p_unk0x04);
	virtual G3D::Vector2 getChildPosition(const GuiItem* child) const;
	virtual bool canLoseFocus() { return true; }
	virtual void onLoseFocus() { menuState = NOTHING; }
	virtual int getFontSize() const;
	void init();

protected:
	bool showChildren() { return menuState >= SHOWN_APPEARING; }
	virtual void onMenuStateChanged() {}
	virtual G3D::Vector2 firstChildPosition() const;
	virtual G3D::Vector2 childOffset() const;
	virtual void render2dMe(Adorn* p_unk0x04);

public:
	UnifiedWidget(const std::string& p_unk0x04);
	UnifiedWidget() { init(); }

	MenuState getMenuState() const { return menuState; }
	void setMenuState(MenuState p_unk0x04);

	virtual GuiResponse process(const GuiEvent& p_unk0x04);
	virtual void render2d(Adorn* adorn);
};

class MenuSpacer : public GuiItem
{
protected:
	void addItem(GuiItem* item);

public:
	MenuSpacer(float x, float y) { setGuiSize(G3D::Vector2(x, y)); }
};

class TextDisplay : public GuiItem
{
private:
	void init();

protected:
	std::string label;
	int fontSize;
	G3D::Color4 fontColor;
	G3D::Color4 borderColor;
	Adorn::XAlign align;

	virtual int getFontSize() const;
	virtual std::string getLabel() const;

public:
	TextDisplay(const std::string& p_unk0x04, const std::string& p_unk0x08);
	TextDisplay();

	virtual void render2d(Adorn* adorn);

	void setLabel(const std::string& value) { label = value; }

	void setFontSize(int value)
	{
		fontSize = value;
		setGuiSize(G3D::Vector2(10.0f * fontSize, 2.0f * fontSize));
	}

	void setFontColor(const G3D::Color4& value) { fontColor = value; }
	void setBorderColor(const G3D::Color4& value) { borderColor = value; }
	void setAlign(Adorn::XAlign value) { align = value; }
};

} // namespace RBX

#endif // GUI_GUI_H
