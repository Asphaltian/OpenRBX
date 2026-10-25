#ifndef RBXVIEW_PBBMESH_H
#define RBXVIEW_PBBMESH_H

#include "MeshFactory.h"
#include "decomp.h"
#include "renderlib/Mesh.h"
#include "util/NormalId.h"

#include <G3D/Vector2.h>
#include <G3D/Vector3.h>

namespace RBX {
namespace View {

// SIZE 0x50
class PBBMesh : public Render::Mesh, public MeshFactory<PBBMesh, 4>
{
private:
	friend class MeshFactory<PBBMesh, 4>;

	PBBMesh(const G3D::Vector3& size, NormalId textureFace, const G3D::Vector2& studsPerTile);

	PBBMesh(const G3D::Vector3& size, NormalId decalFace);

	PBBMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes);
};

DECOMP_SIZE_ASSERT(PBBMesh, 0x50)

} // namespace View
} // namespace RBX

#endif // RBXVIEW_PBBMESH_H
