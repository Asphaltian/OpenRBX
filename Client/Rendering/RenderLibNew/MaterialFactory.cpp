#include "MaterialFactory.h"

#include "util/ContentProvider.h"

#include <G3D/GImage.h>
#include <OgreImage.h>
#include <OgreLogManager.h>
#include <OgreTextureManager.h>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <stdio.h>

namespace RBX {
namespace ViewNew {

static float g_unk0x0092b2d0 = 0.9f;
static float g_unk0x0092b2d4 = 50.0f;

class MegaTextureProxy : public RenderNew::TextureProxy
{
	friend class MaterialFactory;

private:
	float _stripWidth;

	MegaTextureProxy(G3D::TextureManager& textureManager, const std::string& filename)
		: TextureProxy(textureManager, filename, false), _stripWidth(0.0f)
	{
	}

public:
	static const unsigned int numStrips = 32;

	static float stripWidth();

	virtual G3D::ReferenceCountedPointer<G3D::Texture> resolve(G3D::RenderDevice* rd);
};

static int g_unk0x009586f4 = 0;

class BadConversion : public std::runtime_error
{
public:
	BadConversion(const std::string& s) : std::runtime_error(s) {}
};

std::string stringify(double x)
{
	std::ostringstream o;
	if (!(o << x)) {
		throw BadConversion("stringify(double)");
	}
	return o.str();
}

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
		while (true) {
			try {
				m_unk0x4c = Ogre::TextureManager::getSingleton().create(name, "general");
				m_unk0x4c->loadImage(ogreImage);
				break;
			}
			catch (...) {
				if (ogreImage.getWidth() <= 512 && ogreImage.getHeight() <= 512) {
					Ogre::LogManager::getSingleton().logMessage(std::string("Give up on loading texture ") + name);
					break;
				}

				Ogre::TextureManager::getSingleton().remove(m_unk0x4c->getHandle());
				ogreImage.resize(ogreImage.getWidth() / 2, ogreImage.getHeight() / 2);
				Ogre::LogManager::getSingleton().logMessage(
					std::string("Error loading texture. Reduce size to ") + stringify(strip.width) + "x" +
					stringify(strip.height) + " and try again\n"
				);
			}
		}

		root = NULL;
		loaded = true;
	}

	return texture;
}

G3D::ReferenceCountedPointer<RenderNew::TextureProxy> MaterialFactory::getSurfacesTexture(G3D::Color3 tint)
{
	return surfacesTexture->shade(tint, RenderNew::TextureProxyRef());
}

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
