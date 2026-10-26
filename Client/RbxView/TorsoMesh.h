#ifndef RBXVIEW_TORSOMESH_H
#define RBXVIEW_TORSOMESH_H

#include "MeshFactory.h"
#include "decomp.h"
#include "renderlib/Mesh.h"
#include "util/NormalId.h"

#include <G3D/Vector2.h>
#include <G3D/Vector3.h>

namespace RBX {
namespace View {

// SIZE 0x50
class TorsoMesh : public Render::Mesh, public MeshFactory<TorsoMesh, 1>
{
private:
	friend class MeshFactory<TorsoMesh, 1>;

	TorsoMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes);

	TorsoMesh(const G3D::Vector3& size, NormalId decalFace);

	TorsoMesh(const G3D::Vector3& size, NormalId textureFace, const G3D::Vector2& studsPerTile);
};

DECOMP_SIZE_ASSERT(TorsoMesh, 0x50)

} // namespace View
} // namespace RBX

#endif // RBXVIEW_TORSOMESH_H
