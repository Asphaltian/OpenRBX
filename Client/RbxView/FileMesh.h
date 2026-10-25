#ifndef RBXVIEW_FILEMESH_H
#define RBXVIEW_FILEMESH_H

#include "decomp.h"
#include "renderlib/Mesh.h"
#include "util/MeshId.h"

#include <G3D/ReferenceCount.h>
#include <G3D/Vector3.h>

namespace RBX {
namespace View {

// SIZE 0x50
class FileMesh : public Render::Mesh
{
public:
	bool loadFromMeshFile(const G3D::Vector3& scale, const MeshId meshFile);

	static G3D::ReferenceCountedPointer<Render::Mesh> create(const G3D::Vector3& scale, const MeshId meshFile);

	// clang-format off
	// SYNTHETIC: WEBSERVICE 0x1018a8d0
	// RBX::View::FileMesh::`scalar deleting destructor'
	// clang-format on
};

DECOMP_SIZE_ASSERT(FileMesh, 0x50)

} // namespace View
} // namespace RBX

#endif // RBXVIEW_FILEMESH_H
