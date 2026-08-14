#include "renderlib/Material.h"

namespace RBX {
namespace Render {

// FUNCTION: WEBSERVICE 0x101f6640
bool Material::veryTransparent() const
{
	return levels.size() > 0 && levels[0].transparent() > 0.25f;
}

// FUNCTION: WEBSERVICE 0x101f6770
Material::Level::Level()
	: emptyLevel(true), mColor(G3D::Color3::white()), mSpecular(0), mShiny(0), mReflect(0), mTransparent(0)
{
}

// FUNCTION: WEBSERVICE 0x101f67f0
Material::Level::Level(
	TextureProxyRef baseTexture,
	G3D::Color3 color,
	float specular,
	float shiny,
	float reflect,
	float transparent
)
	: emptyLevel(false), mBaseTexture(baseTexture), mColor(color), mSpecular(specular), mShiny(shiny),
	  mReflect(reflect), mTransparent(transparent)
{
	if (!baseTexture.isNull()) {
		mMatte = baseTexture->alphaOnlyVersion();
	}

	if (mTransparent > 0) {
		mReflect = 0;
	}
}

// FUNCTION: WEBSERVICE 0x101f6930
Material::Level::~Level()
{
}

// FUNCTION: WEBSERVICE 0x101f69e0
void Material::Level::configureRenderDevice(G3D::RenderDevice* renderDevice) const
{
	renderDevice->setColor(mColor);
	renderDevice->setSpecularCoefficient(mSpecular);
	renderDevice->setShininess(mShiny);
	renderDevice->setTexture(0, baseTexture(renderDevice));
}

// FUNCTION: WEBSERVICE 0x101f6d40
Material::~Material()
{
}

// FUNCTION: WEBSERVICE 0x101f6ec0
const Material::Level& Material::appendEmptyLevel()
{
	levels.resize(levels.size() + 1, false);
	return levels.last();
}

// FUNCTION: WEBSERVICE 0x101f6ef0
const Material::Level& Material::appendLevel(
	TextureProxyRef baseTexture,
	G3D::Color3 color,
	float specular,
	float shiny,
	float reflect,
	float transparent
)
{
	levels.append(Level(baseTexture, color, specular, shiny, reflect, transparent));

	return levels.last();
}

// FUNCTION: WEBSERVICE 0x101f6ff0
Material::Material()
{
}

} // namespace Render
} // namespace RBX
