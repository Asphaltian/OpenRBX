#ifndef RENDERLIBNEW_RENDERSURFACE_H
#define RENDERLIBNEW_RENDERSURFACE_H

#include "placeholder.h"
#include "renderlibnew/Material.h"
#include "renderlibnew/Mesh.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/ReferenceCount.h>

namespace RBX {
namespace RenderNew {

class RenderSurface
{
public:
	G3D::CoordinateFrame cframe;
	float z;
	const Material::Level* material;
	float polygonOffset;
	const Mesh::Level* mesh;
	G3D::ReferenceCountedPointer<Mesh> fullMesh;

	RenderSurface() {}
};

} // namespace RenderNew
} // namespace RBX

#endif // RENDERLIBNEW_RENDERSURFACE_H
