#ifndef RENDERLIBNEW_RENDERSURFACE_H
#define RENDERLIBNEW_RENDERSURFACE_H

#include "decomp.h"
#include "renderlibnew/Material.h"
#include "renderlibnew/Mesh.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/ReferenceCount.h>

namespace RBX {
namespace RenderNew {

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

	RenderSurface() {}
};

DECOMP_SIZE_ASSERT(RenderSurface, 0x44)

} // namespace RenderNew
} // namespace RBX

#endif // RENDERLIBNEW_RENDERSURFACE_H
