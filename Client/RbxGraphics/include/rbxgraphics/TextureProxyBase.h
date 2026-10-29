#ifndef RBXGRAPHICS_TEXTUREPROXYBASE_H
#define RBXGRAPHICS_TEXTUREPROXYBASE_H

#include "decomp.h"

#include <G3D/ReferenceCount.h>
#include <G3D/Vector2.h>
#include <string>

namespace RBX {

// SIZE 0x28
class TextureProxyBase : public G3D::ReferenceCountedObject
{
public:
	std::string filename; // 0x0c

	// FUNCTION: WEBSERVICE 0x10030b90
	// FUNCTION: ROBLOX 0x00477790
	TextureProxyBase() {}

	// FUNCTION: WEBSERVICE 0x10030be0
	// FUNCTION: ROBLOX 0x004777e0
	virtual ~TextureProxyBase() {}

	// clang-format off
	// SYNTHETIC: WEBSERVICE 0x10030c30
	// SYNTHETIC: ROBLOX 0x00477830
	// RBX::TextureProxyBase::`scalar deleting destructor'
	// clang-format on

	virtual G3D::Vector2 getSize() = 0; // vtable+0x04
};

DECOMP_SIZE_ASSERT(TextureProxyBase, 0x28)

} // namespace RBX

#endif // RBXGRAPHICS_TEXTUREPROXYBASE_H
