#ifndef RENDERLIBNEW_MATERIAL_H
#define RENDERLIBNEW_MATERIAL_H

#include "renderlibnew/TextureProxy.h"
#include "placeholder.h"

#include <G3D/Array.h>
#include <G3D/Color3.h>
#include <G3D/ReferenceCount.h>
#include <GLG3D/RenderDevice.h>
#include <GLG3D/Texture.h>
#include <OgreMaterial.h>

namespace RBX {
namespace RenderNew {

class Material : public G3D::ReferenceCountedObject
{
public:
	class Level
	{
	private:
		bool emptyLevel;
		TextureProxyRef mMatte;
		TextureProxyRef mBaseTexture;
		G3D::Color3 mColor;
		float mSpecular;
		float mShiny;
		float mReflect;
		float mTransparent;
		mutable Ogre::MaterialPtr m_unk0x28;

		Level(
			TextureProxyRef baseTexture,
			G3D::Color3 color,
			float specular,
			float shiny,
			float reflect,
			float transparent
		);

		bool isEmptyLevel() const { return emptyLevel; }

	public:
		Level();
		~Level();

		G3D::TextureRef baseTexture(G3D::RenderDevice* rd) const
		{
			if (mBaseTexture.isNull()) {
				return NULL;
			}

			return mBaseTexture->resolve(rd);
		}

		void setBaseTexture(TextureProxyRef baseTexture);
		G3D::TextureRef matte(G3D::RenderDevice* rd) const
		{
			if (mMatte.isNull()) {
				return NULL;
			}

			return mMatte->resolve(rd);
		}

		const G3D::Color3& color() const { return mColor; }

		float specular() const { return mSpecular; }

		float shiny() const { return mShiny; }

		float reflect() const { return mReflect; }

		float transparent() const { return mTransparent; }

		void configureRenderDevice(G3D::RenderDevice* renderDevice) const;

		friend class Material;
		friend class RenderScene;
	};

private:
	G3D::Array<Level> levels;

public:
	Material();
	virtual ~Material();

	const Level& appendEmptyLevel();
	const Level& appendLevel(G3D::Color3 color, float specular, float shiny, float reflect, float transparent);
	const Level& appendLevel(
		TextureProxyRef baseTexture, G3D::Color3 color, float specular, float shiny, float reflect, float transparent
	);

	unsigned int levelCount() const;
	Level& getLevel(int index);
	const Level* detailLevel(float lvl) const
	{
		if (levels.size() == 0) {
			return NULL;
		}

		const Level* level = &levels[G3D::iClamp((int) (levels.size() * lvl), 0, levels.size() - 1)];

		return level->isEmptyLevel() ? NULL : level;
	}
	bool veryTransparent() const;
};

} // namespace RenderNew
} // namespace RBX

#endif // RENDERLIBNEW_MATERIAL_H
