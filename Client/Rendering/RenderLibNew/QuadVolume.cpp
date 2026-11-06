#include "QuadVolume.h"

namespace RBX {
namespace ViewNew {

void LevelBuilder::buildFace(NormalId normalId, Purpose purpose)
{
	switch (normalId) {
	case NORM_X:
		buildRight(purpose);
		break;
	case NORM_Y:
		buildTop(purpose);
		break;
	case NORM_Z:
		buildBack(purpose);
		break;
	case NORM_X_NEG:
		buildLeft(purpose);
		break;
	case NORM_Y_NEG:
		buildBottom(purpose);
		break;
	case NORM_Z_NEG:
		buildFront(purpose);
		break;
	}
}

void LevelBuilder::build(Purpose purpose)
{
	buildTop(purpose);
	buildBottom(purpose);
	buildLeft(purpose);
	buildRight(purpose);
	buildFront(purpose);
	buildBack(purpose);
}

// STUB: ROBLOX 0x0052c4b0
LevelBuilder::LevelBuilder(
	G3D::ReferenceCountedPointer<RenderNew::Mesh::Level>& level, G3D::Vector3 size, RenderSurfaceTypes surfaceTypes
)
	: halfSize(size * 0.5f), surfaceTypes(surfaceTypes), level(level), textureScale(2.0f, 2.0f)
{
}

// FUNCTION: ROBLOX 0x0052c510
void LevelBuilder::appendQuadFromVertexIndices(unsigned int a, unsigned int b, unsigned int c)
{
	switch (level->primitive) {
	case G3D::RenderDevice::QUADS:
		level->indexArray.append(
			RenderNew::Mesh::allocVertex(a, 1),
			RenderNew::Mesh::allocVertex(a, 1),
			RenderNew::Mesh::allocVertex(b, 1),
			RenderNew::Mesh::allocVertex(c, 1)
		);
		break;
	case G3D::RenderDevice::TRIANGLES:
		level->indexArray.append(
			RenderNew::Mesh::allocVertex(a, 1), RenderNew::Mesh::allocVertex(b, 1), RenderNew::Mesh::allocVertex(c, 1)
		);
		break;
	}
}

// FUNCTION: ROBLOX 0x0052c5e0
void LevelBuilder::appendQuadFromVertexIndices(unsigned int a, unsigned int b, unsigned int c, unsigned int d)
{
	switch (level->primitive) {
	case G3D::RenderDevice::QUADS:
		level->indexArray.append(
			RenderNew::Mesh::allocVertex(a, 1),
			RenderNew::Mesh::allocVertex(b, 1),
			RenderNew::Mesh::allocVertex(c, 1),
			RenderNew::Mesh::allocVertex(d, 1)
		);
		break;
	case G3D::RenderDevice::TRIANGLES:
		level->indexArray.append(
			RenderNew::Mesh::allocVertex(a, 1), RenderNew::Mesh::allocVertex(b, 1), RenderNew::Mesh::allocVertex(c, 1)
		);
		level->indexArray.append(
			RenderNew::Mesh::allocVertex(c, 1), RenderNew::Mesh::allocVertex(d, 1), RenderNew::Mesh::allocVertex(a, 1)
		);
		break;
	}
}

// FUNCTION: ROBLOX 0x0052c700
void LevelBuilder::appendQuadFromIndexArray(int a, int b, int c)
{
	appendQuadFromVertexIndices(level->indexArray[a], level->indexArray[b], level->indexArray[c]);
}

// FUNCTION: ROBLOX 0x0052c730
void LevelBuilder::appendQuadFromIndexArray(int a, int b, int c, int d)
{
	appendQuadFromVertexIndices(level->indexArray[a], level->indexArray[b], level->indexArray[c], level->indexArray[d]);
}

} // namespace ViewNew
} // namespace RBX
