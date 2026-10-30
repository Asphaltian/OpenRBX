#ifndef GUI_GUI_H
#define GUI_GUI_H

#include "decomp.h"
#include "gui/GuiEvent.h"
#include "gui/Layout.h"
#include "rbxgraphics/Adorn.h"
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

// SIZE 0x10c
class GuiItem : public Instance, public GuiTarget
{
public:
	GuiItem();

private:
	// FUNCTION: WEBSERVICE 0x100c7460
	// FUNCTION: ROBLOX 0x0040dd90
	virtual bool askAddChild(const Instance* instance) const { return dynamic_cast<const GuiItem*>(instance) != NULL; }

	// FUNCTION: WEBSERVICE 0x100c7730 FOLDED
	// FUNCTION: ROBLOX 0x0040ddc0 FOLDED
	virtual const Name& getClassName() const { return Name::getNullName(); }

public:

protected:
	virtual void onLoseFocus() {}                 // vtable+0x40
	virtual bool canLoseFocus() { return false; } // vtable+0x44

	// FUNCTION: WEBSERVICE 0x100c7490
	// FUNCTION: ROBLOX 0x0040ddd0
	virtual G3D::Vector2 getPosition() const { return getGuiParent()->getChildPosition(this); } // vtable+0x48

	// FUNCTION: WEBSERVICE 0x100c74b0
	// FUNCTION: ROBLOX 0x0040ddf0
	virtual G3D::Vector2 getChildPosition(const GuiItem* child) const { return G3D::Vector2::zero(); } // vtable+0x4c

	// FUNCTION: WEBSERVICE 0x100c72b0
	// FUNCTION: ROBLOX 0x0040de10
	virtual int getFontSize() const { return 14; } // vtable+0x50

	virtual bool isVisible() const { return true; } // vtable+0x54

	// FUNCTION: WEBSERVICE 0x100c72c0
	// FUNCTION: ROBLOX 0x0040de20
	virtual std::string getTitle() { return getName(); } // vtable+0x58

public:
	// FUNCTION: WEBSERVICE 0x100c75b0
	// FUNCTION: ROBLOX 0x0040de50
	virtual G3D::Vector2 getSize() const { return guiSize; } // vtable+0x5c

	virtual void render2d(Adorn* adorn) {} // vtable+0x60

	virtual GuiResponse process(const GuiEvent& event);

	// FUNCTION: ROBLOX 0x00612660
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
	// FUNCTION: WEBSERVICE 0x100d1460
	// FUNCTION: ROBLOX 0x0058d2c0
	G3D::Rect2D getMyRect2D() const { return getMyRect().toRect2D(); }

private:
	GuiResponse processNonFocus(const GuiEvent& event);
	void switchFocus(GuiItem* item);

	virtual void onDescendentRemoving(const shared_ptr<Instance>& instance);

	shared_ptr<GuiItem> focus; // 0x0fc
	G3D::Vector2 guiSize;      // 0x104
};

DECOMP_SIZE_ASSERT(GuiItem, 0x10c)

// SIZE 0x10c
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
	// FUNCTION: WEBSERVICE 0x100e44b0 FOLDED
	// FUNCTION: ROBLOX 0x00633110 FOLDED
	virtual bool askSetParent(const Instance* instance) const { return true; }

private:
	// FUNCTION: WEBSERVICE 0x100d16c0
	// FUNCTION: ROBLOX 0x0058d540
	virtual G3D::Vector2 getSize() const { return canvasSize; }

	static G3D::Vector2 canvasSize;
	static G3D::Matrix4 oldProjectionMatrix;
	static G3D::CoordinateFrame oldCameraWorld;
};

DECOMP_SIZE_ASSERT(GuiRoot, 0x10c)

// SIZE 0x124
class TopMenuBar : public GuiItem
{
public:
	// FUNCTION: WEBSERVICE 0x100c7690
	// FUNCTION: ROBLOX 0x005e1880
	TopMenuBar() { init(); }
	TopMenuBar(const std::string& p_unk0x04, Layout::Style p_unk0x08, bool p_unk0x0c = false);
	TopMenuBar(const std::string& p_unk0x04, Layout::Style p_unk0x08, G3D::Color4 p_unk0x0c);

	virtual GuiResponse process(const GuiEvent& event);
	virtual void render2d(Adorn* adorn);
	virtual G3D::Vector2 getSize() const;
	// FUNCTION: WEBSERVICE 0x100c72f0
	// FUNCTION: ROBLOX 0x0058d770
	virtual bool isVisible() const { return visible; }

	void setVisible(bool value) { visible = value; }

protected:
	virtual G3D::Vector2 getChildPosition(const GuiItem* child) const;

	G3D::Color4 backdropColor; // 0x10c
	Layout::Style layoutStyle; // 0x11c
	bool visible;              // 0x120

private:
	void init();
};

DECOMP_SIZE_ASSERT(TopMenuBar, 0x124)

// SIZE 0x130
class RelativePanel : public TopMenuBar
{
public:
	// FUNCTION: WEBSERVICE 0x100c7740
	// FUNCTION: ROBLOX 0x005e1920
	RelativePanel()
	{
		offset.x = 0;
		offset.y = 0;

		Layout layout;
		layout.xLocation = Rect::LEFT;
		layout.yLocation = Rect::TOP;
		layout.offset.x = 0;
		layout.offset.y = 0;
		layout.layoutStyle = Layout::HORIZONTAL;
		layout.backdropColor = G3D::Color4::clear();
		init(layout);
	}

	virtual G3D::Vector2 getPosition() const;

protected:
	void init(const Layout& layout);

	Rect::Location xLocation; // 0x124
	Rect::Location yLocation; // 0x128
	G3D::Vector2int16 offset; // 0x12c
};

DECOMP_SIZE_ASSERT(RelativePanel, 0x130)

// SIZE 0x134
class PercentPanel : public TopMenuBar
{
public:
	PercentPanel(Rect::Location p_unk0x04, Rect::Location p_unk0x08, const G3D::Vector2 p_unk0x0c);

	virtual G3D::Vector2 getPosition() const;

protected:
	Rect::Location xLocation; // 0x124
	Rect::Location yLocation; // 0x128
	G3D::Vector2 position;    // 0x12c
};

DECOMP_SIZE_ASSERT(PercentPanel, 0x134)

// SIZE 0x110
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
	MenuState menuState; // 0x10c

	GuiResponse processShown_InTitle(const GuiEvent& p_unk0x04);
	GuiResponse processShown_OutOfTitle(const GuiEvent& p_unk0x04);
	GuiResponse processNothing(const GuiEvent& p_unk0x04);
	GuiResponse processHover(const GuiEvent& p_unk0x04);
	GuiResponse processShown(const GuiEvent& p_unk0x04);
	GuiResponse processKey(const GuiEvent& p_unk0x04);
	void render2dChildren(Adorn* p_unk0x04);
	virtual G3D::Vector2 getChildPosition(const GuiItem* p_unk0x04) const;
	virtual bool canLoseFocus() { return true; }
	virtual void onLoseFocus() { menuState = NOTHING; }
	virtual int getFontSize() const;
	void init();

protected:
	bool showChildren() { return menuState >= SHOWN_APPEARING; }
	virtual void onMenuStateChanged() {}             // vtable+0x64
	virtual G3D::Vector2 firstChildPosition() const; // vtable+0x68
	virtual G3D::Vector2 childOffset() const;        // vtable+0x6c
	virtual void render2dMe(Adorn* p_unk0x04);       // vtable+0x70

public:
	UnifiedWidget(const std::string& p_unk0x04);
	// FUNCTION: ROBLOX 0x00611cc0
	UnifiedWidget() { init(); }

	MenuState getMenuState() const { return menuState; }
	void setMenuState(MenuState p_unk0x04);

	virtual GuiResponse process(const GuiEvent& p_unk0x04);
	virtual void render2d(Adorn* p_unk0x04);
};

DECOMP_SIZE_ASSERT(UnifiedWidget, 0x110)

// SIZE 0x10c
class MenuSpacer : public GuiItem
{
protected:
	void addItem(GuiItem* item);

public:
	// FUNCTION: ROBLOX 0x00611d60
	MenuSpacer(float x, float y) { setGuiSize(G3D::Vector2(x, y)); }
};

DECOMP_SIZE_ASSERT(MenuSpacer, 0x10c)

// SIZE 0x150
class TextDisplay : public GuiItem
{
private:
	void init();

protected:
	std::string label;       // 0x10c
	int fontSize;            // 0x128
	G3D::Color4 fontColor;   // 0x12c
	G3D::Color4 borderColor; // 0x13c
	Adorn::XAlign align;     // 0x14c

	virtual int getFontSize() const;
	virtual std::string getLabel() const; // vtable+0x64

public:
	TextDisplay(const std::string& p_unk0x04, const std::string& p_unk0x08);
	TextDisplay();

	virtual void render2d(Adorn* p_unk0x04);

	void setLabel(const std::string& p_unk0x04);
	void setFontSize(int p_unk0x04);
	void setFontColor(const G3D::Color4& p_unk0x04);
	void setBorderColor(const G3D::Color4& p_unk0x04);
	void setAlign(Adorn::XAlign p_unk0x04);

	// SYNTHETIC: ROBLOX 0x0058dfa0
	// RBX::TextDisplay::~TextDisplay
	// SYNTHETIC: ROBLOX 0x0058dff0
	// RBX::TextDisplay::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(TextDisplay, 0x150)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021c440
// `dynamic initializer for 'RBX::GuiRoot::oldProjectionMatrix''
// SYNTHETIC: WEBSERVICE 0x1021c450
// `dynamic initializer for 'RBX::GuiRoot::oldCameraWorld''
// SYNTHETIC: WEBSERVICE 0x10223d80
// `dynamic atexit destructor for 'RBX::GuiRoot::oldCameraWorld''
// clang-format on

} // namespace RBX

#endif // GUI_GUI_H
