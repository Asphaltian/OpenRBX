#ifndef GUI_GUIDRAW_H
#define GUI_GUIDRAW_H

#include "gui/Widget.h"
#include "rbxgraphics/Adorn.h"
#include "rbxgraphics/TextureProxyBase.h"
#include "placeholder.h"
#include "util/Rect.h"
#include "util/TextureId.h"

#include <G3D/Color4.h>
#include <G3D/ReferenceCount.h>
#include <G3D/Vector2.h>

namespace RBX {

class GuiDrawImage
{
private:
	TextureId currentTexture;
	G3D::ReferenceCountedPointer<TextureProxyBase> normal;
	G3D::ReferenceCountedPointer<TextureProxyBase> disable;
	G3D::ReferenceCountedPointer<TextureProxyBase> hover;
	G3D::ReferenceCountedPointer<TextureProxyBase> down;
	mutable G3D::Vector2 size;

	void draw(
		Adorn* adorn,
		G3D::ReferenceCountedPointer<TextureProxyBase> texture,
		const Rect& rect,
		const G3D::Color4& behind,
		const G3D::Color4& inFront
	);
	void setImageFromName(Adorn*, const std::string&);

public:
	GuiDrawImage() {}
	GuiDrawImage(Adorn* p_unk0x04, const std::string& p_unk0x08) : size(0, 0)
	{
		setImageFromName(p_unk0x04, p_unk0x08);
	}

	void render2d(Adorn* adorn, bool enabled, Rect& rect, Widget::WidgetState state);

	void setImageSize(const G3D::Vector2& value) { size = value; }
	G3D::Vector2 getImageSize() const;

	bool setImage(Adorn* adorn, const TextureId& textureId);
};

} // namespace RBX

#endif // GUI_GUIDRAW_H
