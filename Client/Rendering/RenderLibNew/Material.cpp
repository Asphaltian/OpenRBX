#include "renderlibnew/Material.h"

#include <OgreLogManager.h>
#include <OgreMaterialManager.h>

namespace RBX {
namespace RenderNew {

bool Material::veryTransparent() const
{
	return levels.size() > 0 && levels[0].transparent() > 0.25f;
}

Material::Level::Level()
	: emptyLevel(true), mColor(G3D::Color3::white()), mSpecular(0), mShiny(0), mReflect(0), mTransparent(0)
{
}

// FUNCTION: ROBLOX 0x00535f50
Material::Level::Level(
	TextureProxyRef baseTexture, G3D::Color3 color, float specular, float shiny, float reflect, float transparent
)
	: emptyLevel(false), mBaseTexture(baseTexture), mColor(color), mSpecular(specular), mShiny(shiny),
	  mReflect(reflect), mTransparent(transparent), m_unk0x28(NULL)
{
	if (!baseTexture.isNull()) {
		mMatte = baseTexture->alphaOnlyVersion();
	}
}

// FUNCTION: ROBLOX 0x00536090
Material::Level::~Level()
{
	if (!m_unk0x28.isNull()) {
		std::string name = m_unk0x28->getName();
		Ogre::MaterialManager::getSingleton().remove(m_unk0x28->getHandle());
		Ogre::LogManager::getSingleton().logMessage("RBX removed " + name);
	}
}

void Material::Level::configureRenderDevice(G3D::RenderDevice* renderDevice) const
{
	renderDevice->setColor(mColor);
	renderDevice->setSpecularCoefficient(mSpecular);
	renderDevice->setShininess(mShiny);
	renderDevice->setTexture(0, baseTexture(renderDevice));
}

// FUNCTION: ROBLOX 0x005364a0
Material::~Material()
{
}

// FUNCTION: ROBLOX 0x00536620
const Material::Level& Material::appendEmptyLevel()
{
	levels.resize(levels.size() + 1, false);
	return levels.last();
}

// FUNCTION: ROBLOX 0x00536650
const Material::Level& Material::appendLevel(
	TextureProxyRef baseTexture, G3D::Color3 color, float specular, float shiny, float reflect, float transparent
)
{
	levels.append(Level(baseTexture, color, specular, shiny, reflect, transparent));

	return levels.last();
}

// FUNCTION: ROBLOX 0x00536760
Material::Material()
{
}

} // namespace RenderNew
} // namespace RBX
