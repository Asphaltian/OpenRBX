#include "renderlib/Mesh.h"

#include "renderlib/Chunk.h"

namespace RBX {
namespace Render {

// STUB: WEBSERVICE 0x101676c0
void Mesh::computeDirectionalShadowVolume(
	const G3D::CoordinateFrame& cframe,
	const G3D::Vector3& worldLight,
	G3D::Array<unsigned int>& indexArray,
	G3D::Array<G3D::Vector3>& vertexArray,
	bool caps
) const
{
	STUB(0x101676c0);
}

// STUB: WEBSERVICE 0x10167d70
G3D::ReferenceCountedPointer<Mesh> Mesh::aggregate(
	const std::vector<G3D::ReferenceCountedPointer<Chunk> >& chunkArray,
	G3D::CoordinateFrame& outCFrame,
	float& outBoundingRadius
)
{
	STUB(0x10167d70);
	return NULL;
}

} // namespace Render
} // namespace RBX
