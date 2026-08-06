#ifndef RBXGRAPHICS_TEXTUREPROXYBASE_H
#define RBXGRAPHICS_TEXTUREPROXYBASE_H

#include "decomp.h"

#include <G3D/ReferenceCount.h>
#include <G3D/Vector2.h>
#include <string>

namespace RBX {

// SIZE 0x28
class __declspec(novtable) TextureProxyBase : public G3D::ReferenceCountedObject
{
public:
	virtual ~TextureProxyBase();

	virtual G3D::Vector2 getSize() const = 0; // vtable+0x04

	std::string filename; // 0x0c
};

DECOMP_SIZE_ASSERT(TextureProxyBase, 0x28)

} // namespace RBX

#endif // RBXGRAPHICS_TEXTUREPROXYBASE_H
