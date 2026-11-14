#ifndef RBXVIEW_TORSOMESH_H
#define RBXVIEW_TORSOMESH_H

#include "BevelMesh.h"
#include "MeshFactory.h"
#include "renderlib/Mesh.h"
#include "placeholder.h"
#include "util/NormalId.h"

#include <G3D/Vector2.h>
#include <G3D/Vector3.h>

namespace RBX {
namespace View {

class TorsoMesh : public BevelMesh, public MeshFactory<TorsoMesh, 1>
{
private:
	friend class MeshFactory<TorsoMesh, 1>;

	Render::Mesh::Level& addTorso(float, float) const;

	TorsoMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes);

	TorsoMesh(const G3D::Vector3& size, NormalId decalFace);

	TorsoMesh(const G3D::Vector3& size, NormalId textureFace, const G3D::Vector2& studsPerTile);
};

} // namespace View
} // namespace RBX

#endif // RBXVIEW_TORSOMESH_H
