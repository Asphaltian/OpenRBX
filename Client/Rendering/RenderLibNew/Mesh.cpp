#include "renderlibnew/Mesh.h"

namespace RBX {
namespace RenderNew {

Mesh::VertexList Mesh::vertexRefCounts;

static int g_unk0x00958738;

// FUNCTION: ROBLOX 0x005322d0
unsigned int Mesh::allocVertex(unsigned int i, unsigned int count)
{
	vertexRefCounts[i] += count;
	g_unk0x00958738 += count;

	return i;
}

Mesh::Level::~Level()
{
}

} // namespace RenderNew
} // namespace RBX
