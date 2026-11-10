#include "MaterialFactory.h"

#include "util/ContentProvider.h"

#include <G3D/GImage.h>
#include <OgreImage.h>
#include <OgreTextureManager.h>
#include <algorithm>
#include <stdio.h>

namespace RBX {
namespace ViewNew {

static float g_unk0x0092b2d0 = 0.9f;
static float g_unk0x0092b2d4 = 50.0f;

// SIZE 0x50
class MegaTextureProxy : public RenderNew::TextureProxy
{
	friend class MaterialFactory;

private:
	float _stripWidth; // 0x4c

	MegaTextureProxy(G3D::TextureManager& textureManager, const std::string& filename)
		: TextureProxy(textureManager, filename, false), _stripWidth(0.0f)
	{
	}

public:
	static const unsigned int numStrips;

	static float stripWidth();

	virtual G3D::ReferenceCountedPointer<G3D::Texture> resolve(G3D::RenderDevice* rd); // vtable+0x0c
};

DECOMP_SIZE_ASSERT(MegaTextureProxy, 0x5c)

const unsigned int MegaTextureProxy::numStrips = 32;

// GLOBAL: ROBLOX 0x009586f4
static int g_unk0x009586f4 = 0;

// STUB: ROBLOX 0x0051b2e0
G3D::ReferenceCountedPointer<G3D::Texture> MegaTextureProxy::resolve(G3D::RenderDevice* rd)
{
	if (!loaded) {
		unsigned int maxColors = std::min(numStrips, BrickColor::colorPalette().size());

		G3D::GImage image(filename, G3D::GImage::AUTODETECT);
		G3D::GImage strip(image.width * numStrips, image.height, 3);

		for (unsigned int i = 0; i < maxColors; ++i) {
			G3D::Color3uint8 tint = BrickColor::colorPalette()[i].color3();
			G3D::Color3uint8 table[256];
			for (unsigned int j = 0; j < 256; ++j) {
				table[j] = G3D::Color3uint8(j, j, j);
				shadePixel(tint, table[j]);
			}

			for (int x = 0; x < image.width; ++x) {
				for (int y = 0; y < image.height; ++y) {
					const G3D::Color3uint8& source = image.pixel3(x, y);
					G3D::Color3uint8& target = strip.pixel3(i * image.width + x, y);
					target = table[source.r];
				}
			}
		}

		char name[8];
		_snprintf(name, sizeof(name), "ta%d", g_unk0x009586f4++);

		Ogre::Image ogreImage;
		ogreImage.loadDynamicImage(strip.byte(), strip.width, strip.height, 1, Ogre::PF_B8G8R8, false, 1, 0);
		m_unk0x4c = Ogre::TextureManager::getSingleton().create(name, "general");
		m_unk0x4c->loadImage(ogreImage);

		root = NULL;
		loaded = true;
	}

	return texture;
}

// FUNCTION: ROBLOX 0x0051b810
G3D::ReferenceCountedPointer<RenderNew::TextureProxy> MaterialFactory::getSurfacesTexture(G3D::Color3 tint)
{
	return surfacesTexture->shade(tint, RenderNew::TextureProxyRef());
}

// FUNCTION: ROBLOX 0x0051c0d0
G3D::ReferenceCountedPointer<RenderNew::Material> MaterialFactory::getMegaMaterial(Attributes attributes)
{
	Attributes filteredAttributes = {BrickColor::legoGray(), attributes.transparency, attributes.reflectance};

	G3D::ReferenceCountedPointer<RenderNew::Material> result;
	Database::iterator iter = megaDatabase.find(filteredAttributes);
	if (iter != megaDatabase.end()) {
		result = iter->second.createStrongPtr();
	}

	if (result.isNull()) {
		result = new RenderNew::Material();
		if (attributes.transparency == 1.0f) {
			result->appendEmptyLevel();
		}
		else {
			result->appendLevel(
				megaTexture,
				G3D::Color3::white(),
				g_unk0x0092b2d0,
				g_unk0x0092b2d4,
				attributes.reflectance,
				attributes.transparency
			);
		}
		megaDatabase[filteredAttributes] = result;
	}

	return result;
}

// FUNCTION: ROBLOX 0x0051c2b0
G3D::ReferenceCountedPointer<RenderNew::Material> MaterialFactory::getMaterial(Attributes attributes)
{
	G3D::ReferenceCountedPointer<RenderNew::Material> result;
	Database::iterator iter = database.find(attributes);
	if (iter != database.end()) {
		result = iter->second.createStrongPtr();
	}

	if (result.isNull()) {
		G3D::Color3 color3 = attributes.color.color3();
		G3D::ReferenceCountedPointer<RenderNew::TextureProxy> texture = getSurfacesTexture(color3);

		result = new RenderNew::Material();
		if (attributes.transparency == 1.0f) {
			result->appendEmptyLevel();
		}
		else {
			result->appendLevel(
				texture,
				G3D::Color3::WHITE,
				g_unk0x0092b2d0,
				g_unk0x0092b2d4,
				attributes.reflectance,
				attributes.transparency
			);
		}
		database[attributes] = result;
	}

	return result;
}

// FUNCTION: ROBLOX 0x0051c4e0
MaterialFactory::MaterialFactory(G3D::TextureManager* textureManager)
{
	surfacesTexture = new RenderNew::TextureProxy(
		*textureManager, ContentProvider::singleton().getAssetFile("textures\\surfaces.png"), true
	);
	fileMeshTexture = new RenderNew::TextureProxy(
		*textureManager, ContentProvider::singleton().getAssetFile("textures\\JohnTex.png"), false
	);
	megaTexture =
		new MegaTextureProxy(*textureManager, ContentProvider::singleton().getAssetFile("textures\\SurfacesStrip.png"));
}

} // namespace ViewNew
} // namespace RBX
