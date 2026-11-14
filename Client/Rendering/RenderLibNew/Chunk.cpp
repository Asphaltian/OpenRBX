#include "renderlibnew/Chunk.h"

#include "appdraw/RbxEntity.h"
#include "renderlib/RenderStats.h"

#include <G3D/g3dmath.h>

namespace RBX {
namespace RenderNew {

const float Chunk::DECAL_OFFSET = -0.2f;
const float Chunk::TEXTURE_OFFSET = -0.1f;

bool AggregateChunk::randomColors;
const G3D::CoordinateFrame AggregateChunk::identity;

void Chunk::VTable0x20()
{
	if (m_unk0x10) {
		m_unk0x0c->FUN_00675a70(m_unk0x10);
	}

	m_unk0x10 = NULL;
	m_unk0x0c = NULL;
}

void AggregateChunk::makeMesh()
{
	mesh = Mesh::aggregate(components, _cframe, radius);
}

Chunk::Chunk(float polygonOffset) : m_unk0x14(0.0f), polygonOffset(polygonOffset), radius(G3D::inf())
{
	RenderStats::chunkCount++;
	m_unk0x10 = NULL;
	m_unk0x0c = NULL;
}

Chunk::~Chunk()
{
	RenderStats::chunkCount--;

	if (m_unk0x10) {
		m_unk0x0c->FUN_00675a70(m_unk0x10);
	}
}

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
		material->appendLevel(NULL, (G3D::Color3::wheelRandom() + G3D::Color3::white()) / 2.0f, 0, 50, 0, 0);
	}
}

AggregateChunk::~AggregateChunk()
{
	RenderStats::chunkCount++;
	RenderStats::aggregateChunkCount--;
	RenderStats::aggregatedChunkCount -= components.size();
}

} // namespace RenderNew
} // namespace RBX
