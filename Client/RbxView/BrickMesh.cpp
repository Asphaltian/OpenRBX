#include "BrickMesh.h"

#include "v8datamodel/BrickColor.h"

namespace RBX {
namespace View {

const float BrickMesh::widthInset = 0.0f;

const float BrickMesh::normalPerturbation = 0.05f;

const float BrickMesh::bevel = 0.0f;

// STUB: WEBSERVICE 0x1017aaa0
G3D::ReferenceCountedPointer<Render::Mesh> BrickMesh::create(
	const G3D::Vector3& size,
	RenderSurfaceTypes surfaceTypes,
	BrickColor color
)
{
	STUB(0x1017aaa0);
	return G3D::ReferenceCountedPointer<Render::Mesh>();
}

} // namespace View
} // namespace RBX
