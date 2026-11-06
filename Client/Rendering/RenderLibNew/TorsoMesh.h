#ifndef RENDERLIBNEW_TORSOMESH_H
#define RENDERLIBNEW_TORSOMESH_H

#include "BevelMesh.h"
#include "MeshFactory.h"
#include "decomp.h"
#include "renderlibnew/Mesh.h"
#include "util/NormalId.h"

#include <G3D/Vector2.h>
#include <G3D/Vector3.h>

namespace RBX {
namespace ViewNew {

// SIZE 0x50
class TorsoMesh : public BevelMesh, public MeshFactory<TorsoMesh, 1>
{
private:
	friend class MeshFactory<TorsoMesh, 1>;

	RenderNew::Mesh::Level& addTorso(float, float) const;

	TorsoMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes);

	TorsoMesh(const G3D::Vector3& size, NormalId decalFace);

	TorsoMesh(const G3D::Vector3& size, NormalId textureFace, const G3D::Vector2& studsPerTile);
};

DECOMP_SIZE_ASSERT(TorsoMesh, 0x50)

} // namespace ViewNew
} // namespace RBX

#endif // RENDERLIBNEW_TORSOMESH_H
