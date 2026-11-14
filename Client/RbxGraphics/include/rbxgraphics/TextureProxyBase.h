#ifndef RBXGRAPHICS_TEXTUREPROXYBASE_H
#define RBXGRAPHICS_TEXTUREPROXYBASE_H

#include "placeholder.h"

#include <G3D/ReferenceCount.h>
#include <G3D/Vector2.h>
#include <string>

namespace RBX {

class TextureProxyBase : public G3D::ReferenceCountedObject
{
public:
	std::string filename;

	TextureProxyBase() {}

	virtual ~TextureProxyBase() {}

	virtual G3D::Vector2 getSize() = 0;
};

} // namespace RBX

#endif // RBXGRAPHICS_TEXTUREPROXYBASE_H
