#ifndef RENDERLIBNEW_FILEMESH_H
#define RENDERLIBNEW_FILEMESH_H

#include "renderlibnew/Mesh.h"
#include "placeholder.h"
#include "util/MeshId.h"

#include <G3D/ReferenceCount.h>
#include <G3D/Vector3.h>

namespace RBX {
namespace ViewNew {

class FileMesh : public RenderNew::Mesh
{
public:
	FileMesh() {}

	bool loadFromMeshFile(const G3D::Vector3& scale, const MeshId meshFile);

	static G3D::ReferenceCountedPointer<RenderNew::Mesh> create(const G3D::Vector3& scale, const MeshId meshFile);
};

} // namespace ViewNew
} // namespace RBX

#endif // RENDERLIBNEW_FILEMESH_H
