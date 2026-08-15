#ifndef RBXVIEW_MATERIALFACTORY_H
#define RBXVIEW_MATERIALFACTORY_H

#include "decomp.h"
#include "renderlib/Material.h"
#include "renderlib/TextureProxy.h"
#include "v8datamodel/BrickColor.h"

#include <G3D/ReferenceCount.h>
#include <map>

namespace RBX {
namespace View {

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

		bool operator<(const Attributes& other) const;
	};

	G3D::ReferenceCountedPointer<Render::Material> getMegaMaterial(Attributes attributes);

	G3D::ReferenceCountedPointer<Render::Material> getMaterial(Attributes attributes);

private:
	G3D::ReferenceCountedPointer<Render::TextureProxy> megaTexture;     // 0x00
	G3D::ReferenceCountedPointer<Render::TextureProxy> surfacesTexture; // 0x04
	G3D::ReferenceCountedPointer<Render::TextureProxy> fileMeshTexture; // 0x08
	G3D::ReferenceCountedPointer<Render::Material> fileMeshMaterial;    // 0x0c

	typedef std::map<Attributes, G3D::WeakReferenceCountedPointer<Render::Material> > Database;

	Database database;     // 0x10
	Database megaDatabase; // 0x1c
};

DECOMP_SIZE_ASSERT(MaterialFactory, 0x28)
DECOMP_SIZE_ASSERT(MaterialFactory::Attributes, 0xc)

} // namespace View
} // namespace RBX

#endif // RBXVIEW_MATERIALFACTORY_H
