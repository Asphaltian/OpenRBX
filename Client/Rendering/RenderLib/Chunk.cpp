#include "renderlib/Chunk.h"

namespace RBX {
namespace Render {

const float Chunk::DECAL_OFFSET = -0.2f;
const float Chunk::TEXTURE_OFFSET = -0.1f;

bool AggregateChunk::randomColors;
const G3D::CoordinateFrame AggregateChunk::identity;

// FUNCTION: WEBSERVICE 0x101f96a0
void AggregateChunk::makeMesh()
{
	mesh = Mesh::aggregate(components, _cframe, radius);
}

// STUB: WEBSERVICE 0x101f9740
void AggregateChunk::renderShadows(
	G3D::RenderDevice* renderDevice,
	const G3D::GLight& light,
	bool shadowsEnabled,
	float shadowAlpha
)
{
	STUB(0x101f9740);
}

// STUB: WEBSERVICE 0x101f9ad0
AggregateChunk::AggregateChunk(const G3D::ReferenceCountedPointer<Chunk>& chunk)
{
	STUB(0x101f9ad0);
}

// STUB: WEBSERVICE 0x101f9d70
AggregateChunk::~AggregateChunk()
{
	STUB(0x101f9d70);
}

} // namespace Render
} // namespace RBX
