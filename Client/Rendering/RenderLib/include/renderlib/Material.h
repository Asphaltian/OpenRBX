#ifndef RENDERLIB_MATERIAL_H
#define RENDERLIB_MATERIAL_H

#include "decomp.h"
#include "renderlib/TextureProxy.h"

#include <G3D/Array.h>
#include <G3D/Color3.h>
#include <G3D/ReferenceCount.h>
#include <GLG3D/RenderDevice.h>
#include <GLG3D/Texture.h>

namespace RBX {
namespace Render {

// VTABLE: WEBSERVICE 0x10259d8c
// SIZE 0x18
class Material : public G3D::ReferenceCountedObject
{
public:
	// SIZE 0x28
	class Level
	{
	private:
		bool emptyLevel;              // 0x00
		TextureProxyRef mMatte;       // 0x04
		TextureProxyRef mBaseTexture; // 0x08
		G3D::Color3 mColor;           // 0x0c
		float mSpecular;              // 0x18
		float mShiny;                 // 0x1c
		float mReflect;               // 0x20
		float mTransparent;           // 0x24

		Level(
			TextureProxyRef baseTexture,
			G3D::Color3 color,
			float specular,
			float shiny,
			float reflect,
			float transparent
		);

		bool isEmptyLevel() const;

	public:
		Level();
		~Level();

		// FUNCTION: WEBSERVICE 0x101efe80
		G3D::TextureRef baseTexture(G3D::RenderDevice* renderDevice) const
		{
			if (mBaseTexture.isNull()) {
				return NULL;
			}

			return mBaseTexture->resolve(renderDevice);
		}

		void setBaseTexture(TextureProxyRef baseTexture);
		G3D::TextureRef matte(G3D::RenderDevice* renderDevice) const
		{
			if (mMatte.isNull()) {
				return NULL;
			}

			return mMatte->resolve(renderDevice);
		}

		const G3D::Color3& color() const { return mColor; }

		float specular() const { return mSpecular; }

		float shiny() const { return mShiny; }

		float reflect() const { return mReflect; }

		float transparent() const { return mTransparent; }

		void configureRenderDevice(G3D::RenderDevice* renderDevice) const;

		friend class Material;
	};

private:
	G3D::Array<Level> levels; // 0x0c

public:
	Material();
	virtual ~Material();

	const Level& appendEmptyLevel();
	const Level& appendLevel(G3D::Color3 color, float specular, float shiny, float reflect, float transparent);
	const Level& appendLevel(
		TextureProxyRef baseTexture,
		G3D::Color3 color,
		float specular,
		float shiny,
		float reflect,
		float transparent
	);

	unsigned int levelCount() const;
	Level& getLevel(int index);
	const Level* detailLevel(float detail) const
	{
		if (levels.size() == 0) {
			return NULL;
		}

		int maxIndex = levels.size() - 1;
		int index = (int) detail;

		if (index <= 0) {
			index = 0;
		}
		else if (maxIndex <= index) {
			index = maxIndex;
		}

		const Level* level = &levels[index];

		return level->isEmptyLevel() ? NULL : level;
	}
	bool veryTransparent() const;
};

DECOMP_SIZE_ASSERT(Material, 0x18)
DECOMP_SIZE_ASSERT(Material::Level, 0x28)

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_MATERIAL_H
