#ifndef RBXVIEW_FILEMESH_H
#define RBXVIEW_FILEMESH_H

#include "placeholder.h"
#include "renderlib/Mesh.h"
#include "util/MeshId.h"

#include <G3D/ReferenceCount.h>
#include <G3D/Vector3.h>

namespace RBX {
namespace View {

class FileMesh : public Render::Mesh
{
public:
	FileMesh() {}

	bool loadFromMeshFile(const G3D::Vector3& scale, const MeshId meshFile);

	static G3D::ReferenceCountedPointer<Render::Mesh> create(const G3D::Vector3& scale, const MeshId meshFile);
};

} // namespace View
} // namespace RBX

#endif // RBXVIEW_FILEMESH_H
