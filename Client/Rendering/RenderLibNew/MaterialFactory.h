#ifndef RENDERLIBNEW_MATERIALFACTORY_H
#define RENDERLIBNEW_MATERIALFACTORY_H

#include "decomp.h"
#include "renderlibnew/Material.h"
#include "renderlibnew/TextureProxy.h"
#include "v8datamodel/BrickColor.h"

#include <G3D/ReferenceCount.h>
#include <map>

namespace RBX {
namespace ViewNew {

// SIZE 0x28
class MaterialFactory
{
public:
	// SIZE 0xc
	struct Attributes
	{
		const BrickColor color; // 0x00
		float transparency;     // 0x04
		float reflectance;      // 0x08

		bool operator<(const Attributes& other) const
		{
			if (color < other.color) {
				return true;
			}
			if (color > other.color) {
				return false;
			}
			if (transparency < other.transparency) {
				return true;
			}
			if (transparency > other.transparency) {
				return false;
			}
			return reflectance < other.reflectance;
		}
	};

	G3D::ReferenceCountedPointer<RenderNew::Material> getMegaMaterial(Attributes attributes);

	G3D::ReferenceCountedPointer<RenderNew::Material> getFileMeshMaterial() { return fileMeshMaterial; }

	G3D::ReferenceCountedPointer<RenderNew::Material> getMaterial(Attributes attributes);

	MaterialFactory(G3D::TextureManager* textureManager);

private:
	G3D::ReferenceCountedPointer<RenderNew::TextureProxy> megaTexture;     // 0x00
	G3D::ReferenceCountedPointer<RenderNew::TextureProxy> surfacesTexture; // 0x04
	G3D::ReferenceCountedPointer<RenderNew::TextureProxy> fileMeshTexture; // 0x08
	G3D::ReferenceCountedPointer<RenderNew::Material> fileMeshMaterial;    // 0x0c

	G3D::ReferenceCountedPointer<RenderNew::TextureProxy> getSurfacesTexture(G3D::Color3 tint);

	typedef std::map<Attributes, G3D::WeakReferenceCountedPointer<RenderNew::Material> > Database;

	Database database;     // 0x10
	Database megaDatabase; // 0x1c
};

DECOMP_SIZE_ASSERT(MaterialFactory, 0x28)
DECOMP_SIZE_ASSERT(MaterialFactory::Attributes, 0xc)

// SYNTHETIC: ROBLOX 0x005152e0
// RBX::ViewNew::MegaTextureProxy::`scalar deleting destructor'

} // namespace ViewNew
} // namespace RBX

#endif // RENDERLIBNEW_MATERIALFACTORY_H
