#ifndef GUI_GUI_H
#define GUI_GUI_H

#include "decomp.h"
#include "gui/GuiEvent.h"
#include "gui/Layout.h"
#include "rbxgraphics/Adorn.h"
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
class __declspec(novtable) GuiItem : public Instance, public GuiTarget
{
protected:
	virtual void onLoseFocus();                                        // vtable+0x40
	virtual bool canLoseFocus();                                       // vtable+0x44
	virtual G3D::Vector2 getPosition() const;                          // vtable+0x48
	virtual G3D::Vector2 getChildPosition(const GuiItem* child) const; // vtable+0x4c
	virtual int getFontSize() const;                                   // vtable+0x50
	virtual bool isVisible() const;                                    // vtable+0x54
	virtual std::string getTitle();                                    // vtable+0x58

public:
	virtual G3D::Vector2 getSize() const; // vtable+0x5c
	virtual void render2d(Adorn* adorn);  // vtable+0x60

	virtual GuiResponse process(const GuiEvent& event);

	void addGuiItem(shared_ptr<GuiItem> item);

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
	GuiItem* getFocus();
	void loseFocus();

	Rect getMyRect() const;
	G3D::Rect2D getMyRect2D() const;

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
	static void setCanvasSize(const G3D::Vector2& value);
	static G3D::Vector2 getCanvasSize() { return canvasSize; }
	static G3D::Vector2 toPixelSize(const G3D::Vector2& size);
	static int normalizedFontSize(int size);

	virtual void render2d(Adorn* adorn);

private:
	virtual G3D::Vector2 getSize() const;

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
