#ifndef RBXVIEW_CYLINDERMESH_H
#define RBXVIEW_CYLINDERMESH_H

#include "MeshFactory.h"
#include "decomp.h"
#include "renderlib/Mesh.h"
#include "util/NormalId.h"

#include <G3D/Vector2.h>
#include <G3D/Vector3.h>

namespace RBX {
namespace View {

// SIZE 0x50
class CylinderAlongXMesh : public Render::Mesh, public MeshFactory<CylinderAlongXMesh, 1>
{
private:
	friend class MeshFactory<CylinderAlongXMesh, 1>;

	CylinderAlongXMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes);

	CylinderAlongXMesh(const G3D::Vector3& size, NormalId decalFace);

	CylinderAlongXMesh(const G3D::Vector3& size, NormalId textureFace, const G3D::Vector2& studsPerTile);

	// clang-format off
	// SYNTHETIC: WEBSERVICE 0x1018a8d0
	// RBX::View::CylinderAlongXMesh::`scalar deleting destructor'
	// clang-format on
};

DECOMP_SIZE_ASSERT(CylinderAlongXMesh, 0x50)

} // namespace View
} // namespace RBX

#endif // RBXVIEW_CYLINDERMESH_H
