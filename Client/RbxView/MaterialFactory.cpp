#include "MaterialFactory.h"

namespace RBX {
namespace View {

// STUB: WEBSERVICE 0x10175620
G3D::ReferenceCountedPointer<Render::Material> MaterialFactory::getMegaMaterial(Attributes attributes)
{
	STUB(0x10175620);
	return fileMeshMaterial;
}

// STUB: WEBSERVICE 0x10175800
G3D::ReferenceCountedPointer<Render::Material> MaterialFactory::getMaterial(Attributes attributes)
{
	STUB(0x10175800);
	return fileMeshMaterial;
}

} // namespace View
} // namespace RBX
