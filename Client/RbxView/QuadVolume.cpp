#include "QuadVolume.h"

namespace RBX {
namespace View {

LevelBuilder::LevelBuilder(
	G3D::ReferenceCountedPointer<Render::Mesh::Level>& level,
	G3D::Vector3 size,
	RenderSurfaceTypes surfaceTypes
)
	: halfSize(size * 0.5f), surfaceTypes(surfaceTypes), level(level), textureScale(1.0f)
{
}

// STUB: WEBSERVICE 0x1018ae60
void LevelBuilder::buildFace(NormalId, Purpose)
{
	STUB(0x1018ae60);
}

void LevelBuilder::build(Purpose)
{
}

} // namespace View
} // namespace RBX
