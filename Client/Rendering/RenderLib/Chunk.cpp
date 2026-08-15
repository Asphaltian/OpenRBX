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

// FUNCTION: WEBSERVICE 0x101f96a0
void AggregateChunk::makeMesh()
{
	mesh = Mesh::aggregate(components, _cframe, radius);
}

// FUNCTION: WEBSERVICE 0x101f9740
void AggregateChunk::renderShadows(
	G3D::RenderDevice* rd,
	const G3D::GLight& light,
	bool caps,
	float shadowVertexDistance
)
{
	if (light != shadowSource) {
		shadowSource = light;
		shadowIndexArray.resize(0, true);
		shadowIndexArray16.resize(0, true);

		if (material->veryTransparent()) {
			return;
		}

		static G3D::Array<G3D::Vector3> shadowVertex;
		shadowVertex.resize(0, true);

		shadowVertex.append(-light.position.xyz().direction() * shadowVertexDistance);

		G3D::Vector3 worldLight = light.position.xyz().direction();

		mesh->computeDirectionalShadowVolume(cframe(), worldLight, shadowIndexArray, shadowVertex, caps);

		if (shadowVertex.size() < 65536) {
			int numIndices = shadowIndexArray.size();
			shadowIndexArray16.resize(numIndices, true);

			for (int i = 0; i < numIndices; i++) {
				shadowIndexArray16[i] = (unsigned short) shadowIndexArray[i];
			}

			shadowIndexArray.resize(0, true);
		}

		G3D::VARAreaRef area =
			G3D::VARArea::create(shadowVertex.size() * sizeof(G3D::Vector3), G3D::VARArea::WRITE_EVERY_FEW_FRAMES);
		shadowVAR = G3D::VAR(shadowVertex, area);
	}

	if (shadowIndexArray.size() > 0) {
		rd->beginIndexedPrimitives();
		rd->setVertexArray(shadowVAR);
		rd->sendIndices(G3D::RenderDevice::TRIANGLES, shadowIndexArray);
		rd->endIndexedPrimitives();
	}
	else if (shadowIndexArray16.size() > 0) {
		rd->beginIndexedPrimitives();
		rd->setVertexArray(shadowVAR);
		rd->sendIndices(G3D::RenderDevice::TRIANGLES, shadowIndexArray16);
		rd->endIndexedPrimitives();
	}
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
