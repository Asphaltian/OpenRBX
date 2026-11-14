#ifndef RENDERLIBNEW_MATERIALFACTORY_H
#define RENDERLIBNEW_MATERIALFACTORY_H

#include "renderlibnew/Material.h"
#include "renderlibnew/TextureProxy.h"
#include "placeholder.h"
#include "v8datamodel/BrickColor.h"

#include <G3D/ReferenceCount.h>
#include <map>

namespace RBX {
namespace ViewNew {

class MaterialFactory
{
public:
	struct Attributes
	{
		const BrickColor color;
		float transparency;
		float reflectance;

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
	G3D::ReferenceCountedPointer<RenderNew::TextureProxy> megaTexture;
	G3D::ReferenceCountedPointer<RenderNew::TextureProxy> surfacesTexture;
	G3D::ReferenceCountedPointer<RenderNew::TextureProxy> fileMeshTexture;
	G3D::ReferenceCountedPointer<RenderNew::Material> fileMeshMaterial;

	G3D::ReferenceCountedPointer<RenderNew::TextureProxy> getSurfacesTexture(G3D::Color3 tint);

	typedef std::map<Attributes, G3D::WeakReferenceCountedPointer<RenderNew::Material> > Database;

	Database database;
	Database megaDatabase;
};

} // namespace ViewNew
} // namespace RBX

#endif // RENDERLIBNEW_MATERIALFACTORY_H
