#ifndef RBXVIEW_MATERIALFACTORY_H
#define RBXVIEW_MATERIALFACTORY_H

#include "placeholder.h"
#include "renderlib/Material.h"
#include "renderlib/TextureProxy.h"
#include "v8datamodel/BrickColor.h"

#include <G3D/ReferenceCount.h>
#include <map>

namespace RBX {
namespace View {

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

	G3D::ReferenceCountedPointer<Render::Material> getMegaMaterial(Attributes attributes);

	G3D::ReferenceCountedPointer<Render::Material> getFileMeshMaterial() { return fileMeshMaterial; }

	G3D::ReferenceCountedPointer<Render::Material> getMaterial(Attributes attributes);

	MaterialFactory(G3D::TextureManager* textureManager);

private:
	G3D::ReferenceCountedPointer<Render::TextureProxy> megaTexture;
	G3D::ReferenceCountedPointer<Render::TextureProxy> surfacesTexture;
	G3D::ReferenceCountedPointer<Render::TextureProxy> fileMeshTexture;
	G3D::ReferenceCountedPointer<Render::Material> fileMeshMaterial;

	G3D::ReferenceCountedPointer<Render::TextureProxy> getSurfacesTexture(G3D::Color3 tint);

	typedef std::map<Attributes, G3D::WeakReferenceCountedPointer<Render::Material> > Database;

	Database database;
	Database megaDatabase;
};

} // namespace View
} // namespace RBX

#endif // RBXVIEW_MATERIALFACTORY_H
