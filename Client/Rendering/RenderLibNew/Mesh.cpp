#include "renderlibnew/Mesh.h"

#include "appdraw/VisualEngine.h"
#include "renderlibnew/Chunk.h"

#include <OgreLogManager.h>
#include <stdio.h>

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
bool Mesh::varDirty = true;
Ogre::MeshPtr Mesh::g_unk0x0095877c;

static int g_unk0x00958738;
static int g_unk0x0095873c;
static int g_unk0x00958798;
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

// FUNCTION: ROBLOX 0x00532380
void Mesh::FUN_00532380(Ogre::VisualEngine* engine)
{
	if (varDirty && visibleGeometry.vertexArray.size() != 0) {
		if (!g_unk0x0095877c.isNull()) {
			engine->FUN_00672e60(
				g_unk0x0095877c,
				visibleGeometry.vertexArray[0],
				visibleGeometry.vertexArray.size(),
				visibleGeometry.normalArray[0],
				NULL,
				texCoordArray[0],
				NULL,
				NULL
			);
			varDirty = false;
		}
		else {
			g_unk0x0095877c = engine->FUN_006731e0(
				"rbxvb",
				"General",
				visibleGeometry.vertexArray[0],
				visibleGeometry.vertexArray.size(),
				visibleGeometry.normalArray[0],
				NULL,
				texCoordArray[0],
				NULL,
				NULL
			);
			Ogre::LogManager::getSingleton().logMessage(
				std::string("create mesh=") + g_unk0x0095877c->getName() + "\n"
			);
			g_unk0x0095873c = 0;
			varDirty = false;
		}
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

// FUNCTION: ROBLOX 0x00532d90
void Mesh::Level::makeHWSubMesh(Ogre::VisualEngine* engine, Ogre::AxisAlignedBox& box)
{
	if (m_unk0x1c) {
		std::vector<unsigned int> triangles;
		Ogre::SubMesh* subMesh;
		if (primitive == G3D::RenderDevice::QUADS) {
			triangles.resize(indexArray.size() / 4 * 6);
			for (int i = 0; i < indexArray.size(); i += 4) {
				int quad = i / 4;
				triangles[quad * 6] = indexArray[quad * 4];
				triangles[quad * 6 + 1] = indexArray[quad * 4 + 1];
				triangles[quad * 6 + 2] = indexArray[quad * 4 + 3];
				triangles[quad * 6 + 3] = indexArray[quad * 4 + 3];
				triangles[quad * 6 + 4] = indexArray[quad * 4 + 1];
				triangles[quad * 6 + 5] = indexArray[quad * 4 + 2];
			}
			subMesh = engine->FUN_00671d90(g_unk0x0095877c, NULL, &triangles[0], triangles.size() / 3);
		}
		else {
			subMesh = engine->FUN_00671d90(g_unk0x0095877c, NULL, indexArray.getCArray(), indexArray.size() / 3);
		}

		if (subMesh) {
			m_unk0x20 = g_unk0x0095877c->getNumSubMeshes() - 1;
			m_unk0x1c = false;
		}

		Ogre::Vector3 lo(1e9f, 1e9f, 1e9f);
		Ogre::Vector3 hi(-1e9f, -1e9f, -1e9f);
		const float* positions = visibleGeometry.vertexArray[0];
		for (int i = 0; i < indexArray.size(); i++) {
			IndexType index = indexArray[i] * 3;
			if (lo.x > positions[index]) {
				lo.x = positions[index];
			}
			if (lo.y > positions[index + 1]) {
				lo.y = positions[index + 1];
			}
			if (lo.z > positions[index + 2]) {
				lo.z = positions[index + 2];
			}
			if (hi.x < positions[index]) {
				hi.x = positions[index];
			}
			if (hi.y < positions[index + 1]) {
				hi.y = positions[index + 1];
			}
			if (hi.z < positions[index + 2]) {
				hi.z = positions[index + 2];
			}
		}
		box.setExtents(lo, hi);
	}
	else {
		Ogre::LogManager::getSingleton().logMessage("RBX makeHWSubMesh: should not be here");
	}
}

// STUB: ROBLOX 0x00533160
void Mesh::Level::FUN_00533160(Ogre::VisualEngine* engine, Ogre::AxisAlignedBox& box)
{
	char name[8];
	sprintf(name, "m%d", g_unk0x00958798++);

	unsigned int minIndex = 9999999;
	unsigned int maxIndex = 0;
	for (int i = 0; i < indexArray.size(); i++) {
		if (minIndex > indexArray[i]) {
			minIndex = indexArray[i];
		}
		if (maxIndex < indexArray[i]) {
			maxIndex = indexArray[i];
		}
	}
	int vertexCount = maxIndex - minIndex + 1;

	std::vector<unsigned int> indices32;
	std::vector<unsigned short> indices16;
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> textureCoords;
	if (engine->m_unk0x7f4) {
		if (primitive == G3D::RenderDevice::QUADS) {
			indices32.resize(indexArray.size() / 4 * 6);
			for (int i = 0; i < indexArray.size(); i += 4) {
				int quad = i / 4;
				indices32[quad * 6] = indexArray[quad * 4] - minIndex;
				indices32[quad * 6 + 1] = indexArray[quad * 4 + 1] - minIndex;
				indices32[quad * 6 + 2] = indexArray[quad * 4 + 3] - minIndex;
				indices32[quad * 6 + 3] = indexArray[quad * 4 + 3] - minIndex;
				indices32[quad * 6 + 4] = indexArray[quad * 4 + 1] - minIndex;
				indices32[quad * 6 + 5] = indexArray[quad * 4 + 2] - minIndex;
			}
			positions.resize(vertexCount * 3);
			normals.resize(vertexCount * 3);
			textureCoords.resize(vertexCount * 3);
			for (int i = 0; i < (int) indices32.size(); i++) {
				for (int j = 0; j < 3; j++) {
					positions[indices32[i] * 3 + j] = visibleGeometry.vertexArray[indices32[i] + minIndex][j];
					normals[indices32[i] * 3 + j] = visibleGeometry.normalArray[indices32[i] + minIndex][j];
				}
				for (int j = 0; j < 2; j++) {
					textureCoords[indices32[i] * 2 + j] = texCoordArray[indices32[i] + minIndex][j];
				}
			}
		}
		else if (primitive == G3D::RenderDevice::TRIANGLES) {
			indices32.resize(indexArray.size());
			for (int i = 0; i < indexArray.size(); i++) {
				indices32[i] = indexArray[i] - minIndex;
			}
			positions.resize(vertexCount * 3);
			normals.resize(vertexCount * 3);
			textureCoords.resize(vertexCount * 3);
			for (int i = 0; i < (int) indices32.size(); i++) {
				for (int j = 0; j < 3; j++) {
					positions[indices32[i] * 3 + j] = visibleGeometry.vertexArray[indices32[i] + minIndex][j];
					normals[indices32[i] * 3 + j] = visibleGeometry.normalArray[indices32[i] + minIndex][j];
				}
				for (int j = 0; j < 2; j++) {
					textureCoords[indices32[i] * 2 + j] = texCoordArray[indices32[i] + minIndex][j];
				}
			}
		}
	}
	else {
		if (primitive == G3D::RenderDevice::QUADS) {
			indices16.resize(indexArray.size() / 4 * 6);
			for (int i = 0; i < indexArray.size(); i += 4) {
				int quad = i / 4;
				indices16[quad * 6] = indexArray[quad * 4] - minIndex;
				indices16[quad * 6 + 1] = indexArray[quad * 4 + 1] - minIndex;
				indices16[quad * 6 + 2] = indexArray[quad * 4 + 3] - minIndex;
				indices16[quad * 6 + 3] = indexArray[quad * 4 + 3] - minIndex;
				indices16[quad * 6 + 4] = indexArray[quad * 4 + 1] - minIndex;
				indices16[quad * 6 + 5] = indexArray[quad * 4 + 2] - minIndex;
			}
			positions.resize(vertexCount * 3);
			normals.resize(vertexCount * 3);
			textureCoords.resize(vertexCount * 3);
			for (int i = 0; i < (int) indices16.size(); i++) {
				for (int j = 0; j < 3; j++) {
					positions[indices16[i] * 3 + j] = visibleGeometry.vertexArray[indices16[i] + minIndex][j];
					normals[indices16[i] * 3 + j] = visibleGeometry.normalArray[indices16[i] + minIndex][j];
				}
				for (int j = 0; j < 2; j++) {
					textureCoords[indices16[i] * 2 + j] = texCoordArray[indices16[i] + minIndex][j];
				}
			}
		}
		else if (primitive == G3D::RenderDevice::TRIANGLES) {
			indices16.resize(indexArray.size());
			for (int i = 0; i < indexArray.size(); i++) {
				indices16[i] = indexArray[i] - minIndex;
			}
			positions.resize(vertexCount * 3);
			normals.resize(vertexCount * 3);
			textureCoords.resize(vertexCount * 3);
			for (int i = 0; i < (int) indices16.size(); i++) {
				for (int j = 0; j < 3; j++) {
					positions[indices16[i] * 3 + j] = visibleGeometry.vertexArray[indices16[i] + minIndex][j];
					normals[indices16[i] * 3 + j] = visibleGeometry.normalArray[indices16[i] + minIndex][j];
				}
				for (int j = 0; j < 2; j++) {
					textureCoords[indices16[i] * 2 + j] = texCoordArray[indices16[i] + minIndex][j];
				}
			}
		}
	}

	size_t triangleCount = (engine->m_unk0x7f4 ? indices32.size() : indices16.size()) / 3;
	Ogre::Vector3 minimum;
	Ogre::Vector3 maximum;
	m_unk0x24 = engine->FUN_006735b0(
		&positions[0],
		vertexCount,
		&normals[0],
		NULL,
		&textureCoords[0],
		engine->m_unk0x7f4 ? NULL : &indices16[0],
		engine->m_unk0x7f4 ? &indices32[0] : NULL,
		triangleCount,
		&minimum,
		&maximum
	);
	box.setMinimum(minimum.x, minimum.y, minimum.z);
	box.setMaximum(maximum.x, maximum.y, maximum.z);
	m_unk0x1c = false;
}

} // namespace RenderNew
} // namespace RBX
