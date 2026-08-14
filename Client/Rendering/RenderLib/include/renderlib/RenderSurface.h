#ifndef RENDERLIB_RENDERSURFACE_H
#define RENDERLIB_RENDERSURFACE_H

#include "decomp.h"
#include "renderlib/Material.h"
#include "renderlib/Mesh.h"

#include <G3D/Array.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/ReferenceCount.h>

namespace RBX {
namespace Render {

// SIZE 0x44
class RenderSurface
{
public:
	G3D::CoordinateFrame cframe;                 // 0x00
	float z;                                     // 0x30
	const Material::Level* material;             // 0x34
	float polygonOffset;                         // 0x38
	const Mesh::Level* mesh;                     // 0x3c
	G3D::ReferenceCountedPointer<Mesh> fullMesh; // 0x40

	RenderSurface();
};

DECOMP_SIZE_ASSERT(RenderSurface, 0x44)

void sortByMaterialAndDepth(G3D::Array<RenderSurface*>& array);
void sortByMaterial(G3D::Array<RenderSurface*>& array);
void sortByDepth(G3D::Array<RenderSurface*>& array);

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_RENDERSURFACE_H
