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

// FUNCTION: ROBLOX 0x005367b0
void Chunk::VTable0x20()
{
	if (m_unk0x10) {
		m_unk0x0c->FUN_00675a70(m_unk0x10);
	}

	m_unk0x10 = NULL;
	m_unk0x0c = NULL;
}

// FUNCTION: ROBLOX 0x005367d0
void AggregateChunk::makeMesh()
{
	mesh = Mesh::aggregate(components, _cframe, radius);
}

// FUNCTION: ROBLOX 0x005368f0
Chunk::Chunk(float polygonOffset) : m_unk0x14(0.0f), polygonOffset(polygonOffset), radius(G3D::inf())
{
	RenderStats::chunkCount++;
	m_unk0x10 = NULL;
	m_unk0x0c = NULL;
}

// FUNCTION: ROBLOX 0x00536960
Chunk::~Chunk()
{
	RenderStats::chunkCount--;

	if (m_unk0x10) {
		m_unk0x0c->FUN_00675a70(m_unk0x10);
	}
}

// FUNCTION: ROBLOX 0x00536a00
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

// FUNCTION: ROBLOX 0x00536c20
AggregateChunk::~AggregateChunk()
{
	RenderStats::chunkCount++;
	RenderStats::aggregateChunkCount--;
	RenderStats::aggregatedChunkCount -= components.size();
}

} // namespace RenderNew
} // namespace RBX
