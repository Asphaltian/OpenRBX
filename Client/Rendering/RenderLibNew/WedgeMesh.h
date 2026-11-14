#ifndef RENDERLIBNEW_WEDGEMESH_H
#define RENDERLIBNEW_WEDGEMESH_H

#include "BevelMesh.h"
#include "MeshFactory.h"
#include "renderlibnew/Mesh.h"
#include "placeholder.h"
#include "util/NormalId.h"

#include <G3D/Vector2.h>
#include <G3D/Vector3.h>

namespace RBX {
namespace ViewNew {

class WedgeMesh : public BevelMesh, public MeshFactory<WedgeMesh, 1>
{
private:
	friend class MeshFactory<WedgeMesh, 1>;

	WedgeMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes);

	WedgeMesh(const G3D::Vector3& size, NormalId decalFace);

	WedgeMesh(const G3D::Vector3& size, NormalId textureFace, const G3D::Vector2& studsPerTile);
};

} // namespace ViewNew
} // namespace RBX

#endif // RENDERLIBNEW_WEDGEMESH_H
