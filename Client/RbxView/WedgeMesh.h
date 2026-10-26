#ifndef RBXVIEW_WEDGEMESH_H
#define RBXVIEW_WEDGEMESH_H

#include "MeshFactory.h"
#include "decomp.h"
#include "renderlib/Mesh.h"
#include "util/NormalId.h"

#include <G3D/Vector2.h>
#include <G3D/Vector3.h>

namespace RBX {
namespace View {

// SIZE 0x50
class WedgeMesh : public Render::Mesh, public MeshFactory<WedgeMesh, 1>
{
private:
	friend class MeshFactory<WedgeMesh, 1>;

	WedgeMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes);

	WedgeMesh(const G3D::Vector3& size, NormalId decalFace);

	WedgeMesh(const G3D::Vector3& size, NormalId textureFace, const G3D::Vector2& studsPerTile);
};

DECOMP_SIZE_ASSERT(WedgeMesh, 0x50)

} // namespace View
} // namespace RBX

#endif // RBXVIEW_WEDGEMESH_H
