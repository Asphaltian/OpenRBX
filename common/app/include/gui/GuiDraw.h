#ifndef GUI_GUIDRAW_H
#define GUI_GUIDRAW_H

#include "decomp.h"
#include "rbxgraphics/TextureProxyBase.h"
#include "util/TextureId.h"

#include <G3D/ReferenceCount.h>
#include <G3D/Vector2.h>

namespace RBX {

class Adorn;

// SIZE 0x38
class GuiDrawImage
{
public:
	void setImage(const TextureId& textureId);

	void setImageSize(const G3D::Vector2& value) { size = value; }
	const G3D::Vector2& getImageSize() const { return size; }

	void render2d(Adorn* adorn);

private:
	void draw(Adorn* adorn);

	TextureId currentTexture;                               // 0x00
	G3D::ReferenceCountedPointer<TextureProxyBase> normal;  // 0x20
	G3D::ReferenceCountedPointer<TextureProxyBase> disable; // 0x24
	G3D::ReferenceCountedPointer<TextureProxyBase> hover;   // 0x28
	G3D::ReferenceCountedPointer<TextureProxyBase> down;    // 0x2c
	G3D::Vector2 size;                                      // 0x30
};

DECOMP_SIZE_ASSERT(GuiDrawImage, 0x38)

} // namespace RBX

#endif // GUI_GUIDRAW_H
