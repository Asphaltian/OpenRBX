#include "gui/GUI.h"

#include <G3D/g3dmath.h>
#include <math.h>

namespace RBX {

G3D::Vector2 GuiRoot::canvasSize;

// FUNCTION: WEBSERVICE 0x100d0fc0
const G3D::Color4& GuiItem::disabledFill()
{
	static G3D::Color4 c(0.7f, 0.7f, 0.7f, 0.5f);
	return c;
}

// FUNCTION: WEBSERVICE 0x100d1000
const G3D::Color4& GuiItem::translucentBackdrop()
{
	static G3D::Color4 c(0.6f, 0.6f, 0.6f, 0.6f);
	return c;
}

// FUNCTION: WEBSERVICE 0x100d1040
GuiItem* GuiItem::getGuiParent()
{
	return dynamic_cast<GuiItem*>(getParent());
}

// FUNCTION: WEBSERVICE 0x100d1060
Rect GuiItem::getMyRect() const
{
	Vector2 pos = getPosition();

	return Rect::fromLowSize(pos, getSize());
}

// FUNCTION: WEBSERVICE 0x100d1180
void RelativePanel::init(const Layout& layout)
{
	setName("RelativePanel");
	layoutStyle = layout.layoutStyle;
	backdropColor = layout.backdropColor;
	xLocation = layout.xLocation;
	yLocation = layout.yLocation;
	offset = layout.offset;
}

// FUNCTION: WEBSERVICE 0x100d1230
void TopMenuBar::init()
{
	layoutStyle = Layout::HORIZONTAL;
	backdropColor = G3D::Color4::clear();
	visible = true;
}

// FUNCTION: WEBSERVICE 0x100d1270
G3D::Vector2 GuiRoot::toPixelSize(const G3D::Vector2& percent)
{
	G3D::Vector2 pixelSize(0, 0);

	if (canvasSize.y > 0.75f * canvasSize.x) {
		pixelSize = (0.01f * percent) * G3D::Vector2(canvasSize.x, 0.75f * canvasSize.x);
	}
	else {
		pixelSize = (0.01f * percent) * G3D::Vector2(1.33f * canvasSize.y, canvasSize.y);
	}

	return pixelSize;
}

// FUNCTION: WEBSERVICE 0x100d12f0
int GuiRoot::normalizedFontSize(int fontSize)
{
	static G3D::Vector2 percentSize(100.0f, 75.0f);

	return G3D::iRound(floorf(fontSize * toPixelSize(percentSize).x * 0.001f));
}

// FUNCTION: WEBSERVICE 0x100d1ae0
void GuiItem::loseFocus()
{
	if (focus != NULL) {
		focus->onLoseFocus();
	}

	focus.reset();
}

// FUNCTION: WEBSERVICE 0x100d1b50
void GuiItem::onDescendentRemoving(const shared_ptr<Instance>& instance)
{
	if (instance.get() == focus.get()) {
		loseFocus();
	}

	Instance::onDescendentRemoving(instance);
}

} // namespace RBX
