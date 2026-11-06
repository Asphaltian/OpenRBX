#ifndef RENDERLIBNEW_TEXTUREPROXY_H
#define RENDERLIBNEW_TEXTUREPROXY_H

#include "decomp.h"
#include "rbxgraphics/TextureProxyBase.h"

#include <G3D/Color3.h>
#include <G3D/Color3uint8.h>
#include <GLG3D/Texture.h>
#include <GLG3D/TextureManager.h>
#include <OgreTexture.h>

namespace G3D {
class RenderDevice;
}

namespace RBX {
namespace RenderNew {

class TextureProxy;

typedef G3D::ReferenceCountedPointer<TextureProxy> TextureProxyRef;

// SIZE 0x4c
class TextureProxy : public TextureProxyBase
{
protected:
	G3D::ReferenceCountedPointer<G3D::Texture> texture; // 0x28
	bool loaded;                                        // 0x2c
	G3D::TextureManager* textureManager;                // 0x30
	bool tiled;                                         // 0x34
	TextureProxyRef root;                               // 0x38
	TextureProxyRef shadeTexture;                       // 0x3c
	G3D::Color3 shadeColor;                             // 0x40
	Ogre::TexturePtr m_unk0x4c;                         // 0x4c

	// STUB: ROBLOX 0x005344e0
	TextureProxy(TextureProxyRef root) : loaded(false), textureManager(NULL), root(root) { filename = ""; }

public:
	// STUB: ROBLOX 0x00515260
	TextureProxy(G3D::TextureManager& textureManager, const std::string& filename, bool tiled)
		: loaded(false), textureManager(&textureManager), tiled(tiled)
	{
		this->filename = filename;
	}

	TextureProxyRef shade(const G3D::Color3& baseColor, const TextureProxyRef& baseTex);

	TextureProxyRef alphaOnlyVersion();

	virtual G3D::ReferenceCountedPointer<G3D::Texture> getIfResolved() const;          // vtable+0x08
	virtual G3D::ReferenceCountedPointer<G3D::Texture> resolve(G3D::RenderDevice* rd); // vtable+0x0c

	static void shadePixel(const G3D::Color3uint8& base, G3D::Color3uint8& lighting);

	virtual G3D::Vector2 getSize(); // vtable+0x04
};

DECOMP_SIZE_ASSERT(TextureProxy, 0x58)

} // namespace RenderNew
} // namespace RBX

#endif // RENDERLIBNEW_TEXTUREPROXY_H
