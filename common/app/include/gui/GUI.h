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

	// FUNCTION: WEBSERVICE 0x100c7460
	virtual bool askAddChild(const Instance* instance) const { return dynamic_cast<const GuiItem*>(instance) != NULL; }

protected:
	virtual void onLoseFocus() {}                 // vtable+0x40
	virtual bool canLoseFocus() { return false; } // vtable+0x44

	// FUNCTION: WEBSERVICE 0x100c7490
	virtual G3D::Vector2 getPosition() const { return getGuiParent()->getChildPosition(this); } // vtable+0x48

	// FUNCTION: WEBSERVICE 0x100c74b0
	virtual G3D::Vector2 getChildPosition(const GuiItem* child) const { return G3D::Vector2::zero(); } // vtable+0x4c

	// FUNCTION: WEBSERVICE 0x100c72b0
	virtual int getFontSize() const { return 14; } // vtable+0x50

	virtual bool isVisible() const { return true; } // vtable+0x54

	// FUNCTION: WEBSERVICE 0x100c72c0
	virtual std::string getTitle() { return getName(); } // vtable+0x58

public:
	// FUNCTION: WEBSERVICE 0x100c75b0
	virtual G3D::Vector2 getSize() const { return guiSize; } // vtable+0x5c

	virtual void render2d(Adorn* adorn) {} // vtable+0x60

	virtual GuiResponse process(const GuiEvent& event);

	void addGuiItem(shared_ptr<GuiItem> item);

	void setGuiSize(const G3D::Vector2& value) { guiSize = value; }
	const G3D::Vector2& getGuiSize() const { return guiSize; }

	DECOMP_NOINLINE const GuiItem* getGuiParent() const;
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
	GuiItem* getFocus();
	void loseFocus();

	Rect getMyRect() const;

	void label2d(
		Adorn* adorn,
		const std::string& label,
		const G3D::Color4& fill,
		const G3D::Color4& border,
		Adorn::XAlign align
	) const;
	// STUB: WEBSERVICE 0x100d1460
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
public:
	GuiRoot();

	static void setCanvasSize(const G3D::Vector2& value) { canvasSize = value; }
	static G3D::Vector2 getCanvasSize() { return canvasSize; }
	static G3D::Vector2 toPixelSize(const G3D::Vector2& size);
	static int normalizedFontSize(int size);

	virtual void render2d(Adorn* adorn);

private:
	// FUNCTION: WEBSERVICE 0x100d16c0
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
	virtual GuiResponse process(const GuiEvent& event);
	virtual void render2d(Adorn* adorn);
	virtual G3D::Vector2 getSize() const;
	virtual bool isVisible() const;

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
	virtual G3D::Vector2 getPosition() const;

protected:
	void init(const Layout& layout);

	Rect::Location xLocation; // 0x124
	Rect::Location yLocation; // 0x128
	G3D::Vector2int16 offset; // 0x12c
};

DECOMP_SIZE_ASSERT(RelativePanel, 0x130)

} // namespace RBX

#endif // GUI_GUI_H
