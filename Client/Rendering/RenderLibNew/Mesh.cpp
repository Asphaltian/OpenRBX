#include "renderlibnew/Mesh.h"

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

void Mesh::computeAABounds(
	const std::vector<G3D::ReferenceCountedPointer<Chunk> >& chunkArray, G3D::Vector3& lo, G3D::Vector3& hi
)
{
	lo = G3D::Vector3::inf();
	hi = -G3D::Vector3::inf();

	std::vector<G3D::ReferenceCountedPointer<Chunk> >::const_iterator end = chunkArray.end();

	for (std::vector<G3D::ReferenceCountedPointer<Chunk> >::const_iterator iter = chunkArray.begin(); iter != end;
		 ++iter) {
		const G3D::ReferenceCountedPointer<Chunk>& chunk = *iter;
		const G3D::ReferenceCountedPointer<Mesh>& srcMesh = chunk->getMesh();

		if (srcMesh.isNull()) {
			continue;
		}

		const G3D::CoordinateFrame& cframe = chunk->cframe();
		LevelRef srcLevel = srcMesh->detailLevel(0.0f);
		const G3D::Array<IndexType>& indexArray = srcLevel->indexArray;

		for (int i = 0; i < indexArray.size(); i++) {
			IndexType index = indexArray[i];

			G3D::Vector3 v = cframe.pointToWorldSpace(visibleGeometry.vertexArray[index]);

			hi = hi.max(v);
			lo = lo.min(v);
		}
	}
}

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

G3D::ReferenceCountedPointer<Mesh> Mesh::aggregate(
	const std::vector<G3D::ReferenceCountedPointer<Chunk> >& chunkArray,
	G3D::CoordinateFrame& outCFrame,
	float& outBoundingRadius
)
{
	G3D::ReferenceCountedPointer<Mesh> mesh = new Mesh();

	std::vector<G3D::ReferenceCountedPointer<Chunk> >::const_iterator end = chunkArray.end();

	{
		G3D::Vector3 lo, hi;
		computeAABounds(chunkArray, lo, hi);

		outCFrame.translation = (hi + lo) / 2.0f;
		outCFrame.rotation = G3D::Matrix3::identity();
		outBoundingRadius = (hi - lo).magnitude() * 0.5;
	}

	mesh->levels.append(new Level(G3D::RenderDevice::QUADS));

	{
		LevelRef dstLevel = mesh->levels.last();

		for (std::vector<G3D::ReferenceCountedPointer<Chunk> >::const_iterator iter = chunkArray.begin(); iter != end;
			 ++iter) {
			const G3D::ReferenceCountedPointer<Chunk>& chunk = *iter;
			const G3D::ReferenceCountedPointer<Mesh>& srcMesh = chunk->getMesh();

			if (srcMesh.isNull()) {
				continue;
			}

			const G3D::CoordinateFrame& cframe = chunk->cframe();
			LevelRef srcLevel = srcMesh->detailLevel(0.3f);
			const G3D::Array<IndexType>& indexArray = srcLevel->indexArray;

			G3D::CoordinateFrame netTransform = outCFrame.inverse() * cframe;

			for (int i = 0; i < indexArray.size(); i++) {
				IndexType index = indexArray[i];
				G3D::Vector3 v = netTransform.pointToWorldSpace(visibleGeometry.vertexArray[index]);
				G3D::Vector3 n = netTransform.vectorToWorldSpace(visibleGeometry.normalArray[index]);
				G3D::Vector2 t = texCoordArray[index];

				dstLevel->indexArray.append(allocVertex(v, n, t, false));
			}
		}
	}

	return mesh;
}

} // namespace RenderNew
} // namespace RBX
