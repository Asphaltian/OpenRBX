#ifndef RENDERLIBNEW_SPHEREMESH_H
#define RENDERLIBNEW_SPHEREMESH_H

#include "MeshFactory.h"
#include "decomp.h"
#include "renderlibnew/Mesh.h"
#include "util/NormalId.h"

#include <G3D/Vector2.h>
#include <G3D/Vector3.h>

namespace RBX {
namespace ViewNew {

// SIZE 0x50
class SphereMesh : public RenderNew::Mesh, public MeshFactory<SphereMesh, 1>
{
private:
	friend class MeshFactory<SphereMesh, 1>;

	SphereMesh(const G3D::Vector3& size, NormalId textureFace, const G3D::Vector2& studsPerTile);

	SphereMesh(const G3D::Vector3& size, NormalId decalFace);

	SphereMesh(const G3D::Vector3& diameter, RenderSurfaceTypes surfaceTypes);
};

DECOMP_SIZE_ASSERT(SphereMesh, 0x50)

} // namespace ViewNew
} // namespace RBX

#endif // RENDERLIBNEW_SPHEREMESH_H
