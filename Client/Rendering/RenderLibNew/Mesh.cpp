#include "renderlibnew/Mesh.h"

#include "appdraw/VisualEngine.h"
#include "renderlibnew/Chunk.h"

namespace RBX {
namespace RenderNew {

G3D::MeshAlg::Geometry Mesh::visibleGeometry;
G3D::Array<G3D::Vector2> Mesh::texCoordArray;
G3D::Array<G3D::Vector3> Mesh::tangentArray;
G3D::VAR Mesh::vertexVAR;
G3D::VAR Mesh::normalVAR;
G3D::VAR Mesh::texCoordVAR;
G3D::VAR Mesh::tangentVAR;
Mesh::VertexList Mesh::vertexRefCounts;
Mesh::VertexList Mesh::freeList;
bool Mesh::varDirty;

static int g_unk0x00958738;
static bool g_unk0x00958794;

void Mesh::endRender(G3D::RenderDevice* rd)
{
	rd->endIndexedPrimitives();
}

// FUNCTION: ROBLOX 0x005322d0
unsigned int Mesh::allocVertex(unsigned int i, unsigned int count)
{
	vertexRefCounts[i] += count;
	g_unk0x00958738 += count;

	return i;
}

const Mesh::LevelRef Mesh::detailLevel(float meshLOD) const
{
	return levels[detailIndex(levels.size() * meshLOD)].getPointer();
}

void Mesh::sendGeometry(const Level* lvl, G3D::RenderDevice* rd)
{
	if (lvl != NULL && lvl->indexArray.size() > 0) {
		rd->sendIndices(lvl->primitive, lvl->indexArray);
	}
}

// FUNCTION: ROBLOX 0x005324e0
void Mesh::FUN_005324e0(Ogre::VisualEngine* engine)
{
	if (!m_unk0x28.empty() && !m_unk0x18.empty()) {
		Ogre::Vector3 minimum;
		Ogre::Vector3 maximum;
		m_unk0x38 = engine->FUN_006735b0(
			m_unk0x28[0],
			m_unk0x28.size(),
			NULL,
			NULL,
			NULL,
			&m_unk0x18[0],
			NULL,
			m_unk0x18.size() / 3,
			&minimum,
			&maximum
		);
	}
}

// FUNCTION: ROBLOX 0x005325c0
G3D::ReferenceCountedPointer<Mesh> Mesh::aggregate(
	const std::vector<G3D::ReferenceCountedPointer<Chunk> >& chunkArray,
	G3D::CoordinateFrame& outCFrame,
	float& outBoundingRadius
)
{
	return NULL;
}

// STUB: ROBLOX 0x00532600
unsigned int Mesh::allocVertex(
	const G3D::Vector3& vertex, const G3D::Vector3& normal, const G3D::Vector2& tex, bool share
)
{
	if (!varDirty) {
		varDirty = true;
	}

	if (freeList.size() == 0) {
		int index = visibleGeometry.vertexArray.size();

		visibleGeometry.vertexArray.append(vertex);
		visibleGeometry.normalArray.append(normal);
		texCoordArray.append(tex);
		tangentArray.append(G3D::Vector3::zero());
		vertexRefCounts.append(1);
		g_unk0x00958738++;

		return index;
	}

	unsigned int index = freeList.last();
	freeList.resize(freeList.size() - 1, false);

	visibleGeometry.vertexArray[index] = vertex;
	visibleGeometry.normalArray[index] = normal;
	texCoordArray[index] = tex;
	tangentArray[index] = G3D::Vector3::zero();
	vertexRefCounts[index] = 1;
	g_unk0x00958738++;

	return index;
}

// STUB: ROBLOX 0x00532700
void Mesh::freeVertex(unsigned int index)
{
	if (vertexRefCounts.getCArray() != NULL) {
		int refCount = vertexRefCounts[index];
		g_unk0x00958738--;

		if (refCount == 1) {
			vertexRefCounts[index] = 0;
			freeList.append(index);
		}
		else {
			vertexRefCounts[index] = refCount - 1;
		}
	}
}

// STUB: ROBLOX 0x00532770
Mesh::Level::~Level()
{
	for (int i = 0; i < indexArray.size(); i++) {
		freeVertex(indexArray[i]);
	}
}

void Mesh::makeVAR()
{
	if (varDirty) {
		G3D::VARAreaRef area =
			G3D::VARArea::create(visibleGeometry.vertexArray.size() * 44 + 1024, G3D::VARArea::WRITE_ONCE);

		vertexVAR = G3D::VAR(visibleGeometry.vertexArray, area);
		normalVAR = G3D::VAR(visibleGeometry.normalArray, area);
		texCoordVAR = G3D::VAR(texCoordArray, area);
		tangentVAR = G3D::VAR(tangentArray, area);

		varDirty = false;
	}
}

void Mesh::beginRender(G3D::RenderDevice* rd, bool usetexCoords, bool useTangent)
{
	makeVAR();
	rd->beginIndexedPrimitives();

	if (vertexVAR.valid()) {
		rd->setVertexArray(vertexVAR);
	}

	if (normalVAR.valid()) {
		rd->setNormalArray(normalVAR);
	}

	if (usetexCoords) {
		if (texCoordVAR.valid()) {
			rd->setTexCoordArray(0, texCoordVAR);
		}
	}

	if (useTangent) {
		if (tangentVAR.valid()) {
			rd->setTexCoordArray(2, tangentVAR);
		}
	}
}

// FUNCTION: ROBLOX 0x00532880
void Mesh::initStatics()
{
	visibleGeometry.vertexArray.resize(1);
	visibleGeometry.normalArray.resize(1);
	texCoordArray.resize(1);
	tangentArray.resize(1);
	vertexRefCounts.resize(1);
	vertexRefCounts[0] = 1;
	freeList.resize(0);
}

// FUNCTION: ROBLOX 0x00532ad0
Mesh::Mesh()
{
	if (!g_unk0x00958794) {
		initStatics();
		g_unk0x00958794 = true;
	}

	m_unk0x38 = NULL;
	m_unk0x3c = 0;
}

// STUB: ROBLOX 0x00532b60
Mesh::~Mesh()
{
	STUB(0x532b60);
}

} // namespace RenderNew
} // namespace RBX
