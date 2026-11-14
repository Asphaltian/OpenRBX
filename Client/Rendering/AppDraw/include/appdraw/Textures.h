#ifndef APPDRAW_TEXTURES_H
#define APPDRAW_TEXTURES_H

#include "rbxgraphics/Adorn.h"
#include "rbxgraphics/TextureProxyBase.h"

#include <G3D/ReferenceCount.h>

namespace RBX {

class ContentId;

class Textures
{
public:
	static G3D::ReferenceCountedPointer<TextureProxyBase> getTextureProxy(Adorn* adorn, const ContentId& id);
};

} // namespace RBX

#endif // APPDRAW_TEXTURES_H
