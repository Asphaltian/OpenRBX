#ifndef GUI_GUIDRAW_H
#define GUI_GUIDRAW_H

#include "decomp.h"
#include "gui/Widget.h"
#include "rbxgraphics/Adorn.h"
#include "rbxgraphics/TextureProxyBase.h"
#include "util/Rect.h"
#include "util/TextureId.h"

#include <G3D/Color4.h>
#include <G3D/ReferenceCount.h>
#include <G3D/Vector2.h>

namespace RBX {

// SIZE 0x38
class GuiDrawImage
{
private:
	TextureId currentTexture;                               // 0x00
	G3D::ReferenceCountedPointer<TextureProxyBase> normal;  // 0x20
	G3D::ReferenceCountedPointer<TextureProxyBase> disable; // 0x24
	G3D::ReferenceCountedPointer<TextureProxyBase> hover;   // 0x28
	G3D::ReferenceCountedPointer<TextureProxyBase> down;    // 0x2c
	mutable G3D::Vector2 size;                              // 0x30

	void draw(
		Adorn* adorn,
		G3D::ReferenceCountedPointer<TextureProxyBase> texture,
		const Rect& rect,
		const G3D::Color4& behind,
		const G3D::Color4& inFront
	);
	void setImageFromName(Adorn*, const std::string&);

public:
	// FUNCTION: WEBSERVICE 0x10006260
	// FUNCTION: ROBLOX 0x005e1a20
	GuiDrawImage() {}

	void render2d(Adorn* adorn, bool enabled, Rect& rect, Widget::WidgetState state);

	void setImageSize(const G3D::Vector2& value) { size = value; }
	G3D::Vector2 getImageSize() const;

	bool setImage(Adorn* adorn, const TextureId& textureId);
};

DECOMP_SIZE_ASSERT(GuiDrawImage, 0x38)

} // namespace RBX

#endif // GUI_GUIDRAW_H
