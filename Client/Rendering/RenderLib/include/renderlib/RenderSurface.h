#ifndef RENDERLIB_RENDERSURFACE_H
#define RENDERLIB_RENDERSURFACE_H

#include "placeholder.h"
#include "renderlib/Material.h"
#include "renderlib/Mesh.h"

#include <G3D/Array.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/ReferenceCount.h>

namespace RBX {
namespace Render {

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

void sortByMaterialAndDepth(G3D::Array<RenderSurface*>& array);
void sortByMaterial(G3D::Array<RenderSurface*>& array);
void sortByDepth(G3D::Array<RenderSurface*>& array);

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_RENDERSURFACE_H
