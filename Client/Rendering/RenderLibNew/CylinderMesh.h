#ifndef RENDERLIBNEW_CYLINDERMESH_H
#define RENDERLIBNEW_CYLINDERMESH_H

#include "MeshFactory.h"
#include "placeholder.h"
#include "renderlibnew/Mesh.h"
#include "util/NormalId.h"

#include <G3D/Vector2.h>
#include <G3D/Vector3.h>

namespace RBX {
namespace ViewNew {

class CylinderAlongXMesh : public RenderNew::Mesh, public MeshFactory<CylinderAlongXMesh, 1>
{
private:
	friend class MeshFactory<CylinderAlongXMesh, 1>;

	CylinderAlongXMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes);

	CylinderAlongXMesh(const G3D::Vector3& size, NormalId decalFace);

	CylinderAlongXMesh(const G3D::Vector3& size, NormalId textureFace, const G3D::Vector2& studsPerTile);
};

} // namespace ViewNew
} // namespace RBX

#endif // RENDERLIBNEW_CYLINDERMESH_H
