#ifndef RENDERLIBNEW_PBBMESH_H
#define RENDERLIBNEW_PBBMESH_H

#include "MeshFactory.h"
#include "decomp.h"
#include "renderlibnew/Mesh.h"
#include "util/NormalId.h"

#include <G3D/Vector2.h>
#include <G3D/Vector3.h>

namespace RBX {
namespace ViewNew {

// SIZE 0x50
class PBBMesh : public RenderNew::Mesh, public MeshFactory<PBBMesh, 4>
{
private:
	friend class MeshFactory<PBBMesh, 4>;

	PBBMesh(const G3D::Vector3& size, NormalId textureFace, const G3D::Vector2& studsPerTile);

	PBBMesh(const G3D::Vector3& size, NormalId decalFace);

	PBBMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes);
};

DECOMP_SIZE_ASSERT(PBBMesh, 0x50)

} // namespace ViewNew
} // namespace RBX

#endif // RENDERLIBNEW_PBBMESH_H
