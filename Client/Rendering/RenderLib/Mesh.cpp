#include "renderlib/Mesh.h"

#include "renderlib/Chunk.h"

namespace RBX {
namespace Render {

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
