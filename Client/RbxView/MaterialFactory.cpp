#include "MaterialFactory.h"

#include "util/ContentProvider.h"

#include <G3D/GImage.h>
#include <algorithm>

namespace RBX {
namespace View {

static float specular = 0.9f;
static float shiny = 50.0f;

class MegaTextureProxy : public Render::TextureProxy
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

		G3D::Texture::Settings params;
		params.wrapMode = G3D::Texture::CLAMP;

		texture =
			G3D::Texture::fromGImage("MegaTexture", strip, G3D::TextureFormat::RGB8, G3D::Texture::DIM_2D, params);
		root = NULL;
		loaded = true;
	}

	return texture;
}

G3D::ReferenceCountedPointer<Render::TextureProxy> MaterialFactory::getSurfacesTexture(G3D::Color3 tint)
{
	return surfacesTexture->shade(tint, Render::TextureProxyRef());
}

G3D::ReferenceCountedPointer<Render::Material> MaterialFactory::getMegaMaterial(Attributes attributes)
{
	Attributes filteredAttributes = {BrickColor::legoGray(), attributes.transparency, attributes.reflectance};

	G3D::ReferenceCountedPointer<Render::Material> result;
	Database::iterator iter = megaDatabase.find(filteredAttributes);
	if (iter != megaDatabase.end()) {
		result = iter->second.createStrongPtr();
	}

	if (result.isNull()) {
		result = new Render::Material();
		if (attributes.transparency == 1.0f) {
			result->appendEmptyLevel();
		}
		else {
			result->appendLevel(
				megaTexture, G3D::Color3::white(), specular, shiny, attributes.reflectance, attributes.transparency
			);
		}
		megaDatabase[filteredAttributes] = result;
	}

	return result;
}

G3D::ReferenceCountedPointer<Render::Material> MaterialFactory::getMaterial(Attributes attributes)
{
	G3D::ReferenceCountedPointer<Render::Material> result;
	Database::iterator iter = database.find(attributes);
	if (iter != database.end()) {
		result = iter->second.createStrongPtr();
	}

	if (result.isNull()) {
		G3D::Color3 color3 = attributes.color.color3();
		G3D::ReferenceCountedPointer<Render::TextureProxy> texture = getSurfacesTexture(color3);

		result = new Render::Material();
		if (attributes.transparency == 1.0f) {
			result->appendEmptyLevel();
		}
		else {
			result->appendLevel(
				texture, G3D::Color3::WHITE, specular, shiny, attributes.reflectance, attributes.transparency
			);
		}
		database[attributes] = result;
	}

	return result;
}

MaterialFactory::MaterialFactory(G3D::TextureManager* textureManager)
{
	surfacesTexture = new Render::TextureProxy(
		*textureManager, ContentProvider::singleton().getAssetFile("textures\\surfaces.png"), true
	);
	fileMeshTexture = new Render::TextureProxy(
		*textureManager, ContentProvider::singleton().getAssetFile("textures\\JohnTex.png"), false
	);
	megaTexture =
		new MegaTextureProxy(*textureManager, ContentProvider::singleton().getAssetFile("textures\\SurfacesStrip.png"));
}

} // namespace View
} // namespace RBX
