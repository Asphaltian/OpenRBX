#include "renderlib/Chunk.h"

#include "renderlib/RenderStats.h"

#include <G3D/g3dmath.h>

namespace RBX {
namespace Render {

const float Chunk::DECAL_OFFSET = -0.2f;
const float Chunk::TEXTURE_OFFSET = -0.1f;

bool AggregateChunk::randomColors;
const G3D::CoordinateFrame AggregateChunk::identity;

// FUNCTION: WEBSERVICE 0x1016f140
Chunk::Chunk(float polygonOffset) : polygonOffset(polygonOffset), radius(G3D::inf())
{
	RenderStats::chunkCount++;
}

// STUB: WEBSERVICE 0x1016f1a0
Chunk::~Chunk()
{
	RenderStats::chunkCount--;
}

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

// FUNCTION: WEBSERVICE 0x101f9ad0
AggregateChunk::AggregateChunk(const G3D::ReferenceCountedPointer<Chunk>& firstChunk)
	: Chunk(firstChunk->polygonOffset), _castsShadows(firstChunk->castsShadows()), _cullable(firstChunk->cullable())
{
	if (!randomColors) {
		material = firstChunk->getMaterial();
	}

	RenderStats::aggregateChunkCount++;
	RenderStats::chunkCount--;

	if (randomColors) {
		material = new Material();
		material->appendLevel(NULL, (G3D::Color3::white() + G3D::Color3::wheelRandom()) / 2.0f, 0, 50, 0, 0);
	}
}

// FUNCTION: WEBSERVICE 0x101f9d70
AggregateChunk::~AggregateChunk()
{
	RenderStats::chunkCount++;
	RenderStats::aggregateChunkCount--;
	RenderStats::aggregatedChunkCount -= components.size();
}

} // namespace Render
} // namespace RBX
