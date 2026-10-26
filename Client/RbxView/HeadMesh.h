#ifndef RBXVIEW_HEADMESH_H
#define RBXVIEW_HEADMESH_H

#include "MeshFactory.h"
#include "decomp.h"
#include "renderlib/Mesh.h"
#include "util/NormalId.h"

#include <G3D/Vector2.h>
#include <G3D/Vector3.h>

namespace RBX {
namespace View {

// SIZE 0x50
class HeadMesh : public Render::Mesh, public MeshFactory<HeadMesh, 1>
{
private:
	friend class MeshFactory<HeadMesh, 1>;

	HeadMesh(const G3D::Vector3& partSize, RenderSurfaceTypes surfaceTypes);

	HeadMesh(const G3D::Vector3& size, NormalId decalFace);

	HeadMesh(const G3D::Vector3& size, NormalId textureFace, const G3D::Vector2& studsPerTile);
};

DECOMP_SIZE_ASSERT(HeadMesh, 0x50)

} // namespace View
} // namespace RBX

#endif // RBXVIEW_HEADMESH_H
