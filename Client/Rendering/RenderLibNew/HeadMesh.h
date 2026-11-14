#ifndef RENDERLIBNEW_HEADMESH_H
#define RENDERLIBNEW_HEADMESH_H

#include "MeshFactory.h"
#include "renderlibnew/Mesh.h"
#include "placeholder.h"
#include "util/NormalId.h"

#include <G3D/Vector2.h>
#include <G3D/Vector3.h>

namespace RBX {
namespace ViewNew {

class HeadMesh : public RenderNew::Mesh, public MeshFactory<HeadMesh, 1>
{
private:
	friend class MeshFactory<HeadMesh, 1>;

	HeadMesh(const G3D::Vector3& partSize, RenderSurfaceTypes surfaceTypes);

	HeadMesh(const G3D::Vector3& size, NormalId decalFace);

	HeadMesh(const G3D::Vector3& size, NormalId textureFace, const G3D::Vector2& studsPerTile);
};

} // namespace ViewNew
} // namespace RBX

#endif // RENDERLIBNEW_HEADMESH_H
