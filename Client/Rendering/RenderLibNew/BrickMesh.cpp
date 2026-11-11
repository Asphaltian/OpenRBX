#include "BrickMesh.h"

#include "QuadVolume.h"
#include "ViewBase.h"
#include "util/Math.h"
#include "v8datamodel/BrickColor.h"

#include <G3D/g3dmath.h>
#include <algorithm>
#include <map>

namespace RBX {
namespace ViewNew {

// GLOBAL: ROBLOX 0x0092a908
bool g_unk0x0092a908 = true;

static int orderedCompare(const G3D::Vector3& a, const G3D::Vector3& b)
{
	if (a.x < b.x) {
		return -1;
	}
	if (a.x > b.x) {
		return 1;
	}
	if (a.y < b.y) {
		return -1;
	}
	if (a.y > b.y) {
		return 1;
	}
	if (a.z < b.z) {
		return -1;
	}
	if (a.z > b.z) {
		return 1;
	}
	return 0;
}

struct Lookup
{
	G3D::Vector3 size;               // 0x00
	RenderSurfaceTypes surfaceTypes; // 0x0c
	RBX::BrickColor color;           // 0x10

	bool operator==(const Lookup& other) const;

	bool operator<(const Lookup& other) const
	{
		int c = orderedCompare(size, other.size);
		if (c < 0) {
			return true;
		}
		if (c > 0) {
			return false;
		}
		if (surfaceTypes < other.surfaceTypes) {
			return true;
		}
		if (surfaceTypes > other.surfaceTypes) {
			return false;
		}
		return color < other.color;
	}
};

struct Variations
{
	int count;                                                    // 0x00
	G3D::ReferenceCountedPointer<RBX::RenderNew::Mesh> meshes[4]; // 0x04

	Variations() : count(0) {}
};

static std::map<Lookup, Variations> cache;

const float BrickMesh::widthInset = 0.0f;

const float BrickMesh::normalPerturbation = 0.05f;

const float BrickMesh::bevel = 0.0f;

static bool g_unk0x00958541;

// GLOBAL: ROBLOX 0x00958544
float BrickMesh::g_unk0x00958544 = 0.0f;

static G3D::Vector2 flatTex(1.0f / 64.0f, 8.0f / 9.0f);

template <int N>
void meshFace(
	const G3D::Vector3& halfSize,
	float inset,
	RenderSurfaceTypes surfaceTypes,
	float colorIndex,
	RBX::RenderNew::Mesh::LevelRef& level,
	unsigned int* corners
)
{
	RenderSurfaceType surfaceType = surfaceTypes.getSurfaceType((RBX::NormalId) N);

	G3D::Vector3 faceCorner =
		G3D::abs(RBX::ViewNew::uvwToObject<N>(halfSize)) - G3D::Vector3(inset, inset, BrickMesh::bevel);

	G3D::Vector2 studsPerTile(0.0f, 0.0f);
	G3D::Vector2 textureOffset(0.0f, 0.0f);

	switch (surfaceType) {
	case FLAT:
		textureOffset = flatTex;
		studsPerTile = faceCorner.xy() * 2.0;
		break;
	case STUDS:
		studsPerTile = G3D::Vector2(2.0f, 4.0f);
		break;
	case INLET:
		studsPerTile = G3D::Vector2(2.0f, 4.0f);
		textureOffset.y = 1.0f / 3.0f;
		break;
	case WELD:
		studsPerTile = G3D::Vector2(2.0f, 2.0f);
		textureOffset.y = 2.0f / 3.0f;
		break;
	case GLUE:
		studsPerTile = G3D::Vector2(2.0f, 2.0f);
		textureOffset.y = 5.0f / 6.0f;
		break;
	}

	textureOffset.x += colorIndex / 32.0;

	static G3D::Vector2 lastPerturbationMin;
	static G3D::Vector2 lastPerturbationMax;

	G3D::Vector2 perturbationMin;
	G3D::Vector2 perturbationMax;
	if (!g_unk0x00958541) {
		perturbationMin.x = G3D::uniformRandom(-BrickMesh::normalPerturbation, BrickMesh::normalPerturbation);
		perturbationMin.y = G3D::uniformRandom(-BrickMesh::normalPerturbation, BrickMesh::normalPerturbation);
		perturbationMax.x = G3D::uniformRandom() > 0.5f ? perturbationMin.x : -perturbationMin.x;
		perturbationMax.y = G3D::uniformRandom() > 0.5f ? perturbationMin.y : -perturbationMin.y;
	}
	else {
		perturbationMin = lastPerturbationMin;
		perturbationMax = lastPerturbationMax;
	}

	lastPerturbationMax = perturbationMax;
	lastPerturbationMin = perturbationMin;

	if (surfaceType == FLAT) {
		G3D::Vector3 nxy(perturbationMin.x, perturbationMin.y, 1.0f);
		G3D::Vector3 nxY(perturbationMin.x, perturbationMax.y, 1.0f);
		G3D::Vector3 nXy(perturbationMax.x, perturbationMin.y, 1.0f);
		G3D::Vector3 nXY(perturbationMax.x, perturbationMax.y, 1.0f);

		unsigned int i0 = RBX::RenderNew::Mesh::allocVertex(
			RBX::ViewNew::uvwToObject<N>(G3D::Vector3(-faceCorner.x, -faceCorner.y, faceCorner.z)),
			RBX::ViewNew::uvwToObject<N>(nxy),
			textureOffset,
			true
		);
		unsigned int i1 = RBX::RenderNew::Mesh::allocVertex(
			RBX::ViewNew::uvwToObject<N>(G3D::Vector3(faceCorner.x, -faceCorner.y, faceCorner.z)),
			RBX::ViewNew::uvwToObject<N>(nXy),
			textureOffset,
			true
		);
		unsigned int i2 = RBX::RenderNew::Mesh::allocVertex(
			RBX::ViewNew::uvwToObject<N>(G3D::Vector3(faceCorner.x, faceCorner.y, faceCorner.z)),
			RBX::ViewNew::uvwToObject<N>(nXY),
			textureOffset,
			true
		);
		unsigned int i3 = RBX::RenderNew::Mesh::allocVertex(
			RBX::ViewNew::uvwToObject<N>(G3D::Vector3(-faceCorner.x, faceCorner.y, faceCorner.z)),
			RBX::ViewNew::uvwToObject<N>(nxY),
			textureOffset,
			true
		);

		level->indexArray.append(i0, i1, i2, i3);

		corners[0] = i0;
		corners[1] = i1;
		corners[2] = i2;
		corners[3] = i3;
	}
	else {
		G3D::Vector2 minCorner;
		minCorner.x = -faceCorner.x;

		G3D::Vector2 normalDelta = (perturbationMax - perturbationMin) * 0.5 / faceCorner.xy();
		int countX = 0;

		G3D::Vector2 minNormal;
		minNormal.x = perturbationMin.x;

		static std::vector<unsigned int> indices;
		indices.clear();

		int countY;
		while (minCorner.x < faceCorner.x) {
			G3D::Vector2 maxCorner;
			maxCorner.x = std::min(minCorner.x + studsPerTile.x, faceCorner.x);
			countY = 0;

			G3D::Vector2 maxNormal;
			maxNormal.x = minNormal.x + normalDelta.x;

			minCorner.y = -faceCorner.y;
			minNormal.y = perturbationMin.y;

			while (minCorner.y < faceCorner.y) {
				maxCorner.y = std::min(minCorner.y + studsPerTile.y, faceCorner.y);
				maxNormal.y = minNormal.y + normalDelta.y;

				G3D::Vector2 t0(
					textureOffset.x + 1.0 / 1024.0,
					textureOffset.y + (maxCorner.y - minCorner.y) / 12.0 - BrickMesh::bevel
				);
				G3D::Vector2 t1(textureOffset.x - 1.0 / 1024.0 + (maxCorner.x - minCorner.x) / 64.0, t0.y);
				G3D::Vector2 t2(t1.x, textureOffset.y - BrickMesh::bevel);
				G3D::Vector2 t3(textureOffset.x + 1.0 / 1024.0, textureOffset.y + BrickMesh::bevel);

				unsigned int i0 = RBX::RenderNew::Mesh::allocVertex(
					RBX::ViewNew::uvwToObject<N>(G3D::Vector3(minCorner.x, minCorner.y, faceCorner.z)),
					RBX::ViewNew::uvwToObject<N>(G3D::Vector3(minNormal.x, minNormal.y, 1.0f)),
					t0,
					true
				);
				unsigned int i1 = RBX::RenderNew::Mesh::allocVertex(
					RBX::ViewNew::uvwToObject<N>(G3D::Vector3(maxCorner.x, minCorner.y, faceCorner.z)),
					RBX::ViewNew::uvwToObject<N>(G3D::Vector3(maxNormal.x, minNormal.y, 1.0f)),
					t1,
					true
				);
				unsigned int i2 = RBX::RenderNew::Mesh::allocVertex(
					RBX::ViewNew::uvwToObject<N>(G3D::Vector3(maxCorner.x, maxCorner.y, faceCorner.z)),
					RBX::ViewNew::uvwToObject<N>(G3D::Vector3(maxNormal.x, maxNormal.y, 1.0f)),
					t2,
					true
				);
				unsigned int i3 = RBX::RenderNew::Mesh::allocVertex(
					RBX::ViewNew::uvwToObject<N>(G3D::Vector3(minCorner.x, maxCorner.y, faceCorner.z)),
					RBX::ViewNew::uvwToObject<N>(G3D::Vector3(minNormal.x, maxNormal.y, 1.0f)),
					t3,
					true
				);

				indices.push_back(i0);
				indices.push_back(i1);
				indices.push_back(i2);
				indices.push_back(i3);

				level->indexArray.append(i0, i1, i2, i3);
				countY++;

				minCorner.y = maxCorner.y;
				minNormal.y = maxNormal.y;
			}

			countX++;
			minCorner.x = maxCorner.x;
			minNormal.x = maxNormal.x;
		}

		corners[0] = indices[0];
		corners[1] = indices[(countX - 1) * countY * 4 + 1];
		corners[2] = indices[countY * countX * 4 - 2];
		corners[3] = indices[countY * 4 - 1];
	}
}

// FUNCTION: ROBLOX 0x0050eed0
void BrickMesh::flushCache()
{
	cache.clear();
}

// FUNCTION: ROBLOX 0x0050f030
static void appendTriangles(std::vector<unsigned short>& indices, int v0, int v1, int v2, int v3)
{
	indices.push_back(v0);
	indices.push_back(v1);
	indices.push_back(v2);
	indices.push_back(v0);
	indices.push_back(v2);
	indices.push_back(v3);
}

// FUNCTION: ROBLOX 0x00514070
void BrickMesh::FUN_00514070(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes, BrickColor color)
{
	RenderNew::Mesh::LevelRef level = new RenderNew::Mesh::Level(G3D::RenderDevice::QUADS);
	levels.append(level);

	G3D::Vector3 halfSize(size.x * 0.5 - widthInset, size.y * 0.5, size.z * 0.5 - widthInset);

	const float colorIndex = color.getClosestPaletteIndex();

	unsigned int corners[24];
	meshFace<0>(halfSize, g_unk0x00958544, surfaceTypes, colorIndex, level, &corners[0]);
	meshFace<1>(halfSize, g_unk0x00958544, surfaceTypes, colorIndex, level, &corners[4]);
	meshFace<2>(halfSize, g_unk0x00958544, surfaceTypes, colorIndex, level, &corners[8]);
	meshFace<3>(halfSize, g_unk0x00958544, surfaceTypes, colorIndex, level, &corners[12]);
	meshFace<4>(halfSize, g_unk0x00958544, surfaceTypes, colorIndex, level, &corners[16]);
	meshFace<5>(halfSize, g_unk0x00958544, surfaceTypes, colorIndex, level, &corners[20]);

	if (g_unk0x00958544 > 0.0f) {
		G3D::Vector2 tex = flatTex;
		tex.x += colorIndex / 32.0;

		unsigned int vertices[24];
		for (int i = 0; i < 24; i++) {
			vertices[i] = allocVertex(
				visibleGeometry.vertexArray[corners[i]], visibleGeometry.normalArray[corners[i]], tex, true
			);
		}

		level->indexArray.append(vertices[3], vertices[2], vertices[4], vertices[7]);
		level->indexArray.append(vertices[11], vertices[10], vertices[7], vertices[6]);
		level->indexArray.append(vertices[6], vertices[5], vertices[15], vertices[14]);
		level->indexArray.append(vertices[5], vertices[4], vertices[23], vertices[22]);
		level->indexArray.append(vertices[10], vertices[9], vertices[0], vertices[3]);
		level->indexArray.append(vertices[2], vertices[1], vertices[20], vertices[23]);
		level->indexArray.append(vertices[12], vertices[15], vertices[22], vertices[21]);
		level->indexArray.append(vertices[14], vertices[13], vertices[8], vertices[11]);
		level->indexArray.append(vertices[17], vertices[16], vertices[21], vertices[20]);
		level->indexArray.append(vertices[13], vertices[12], vertices[16], vertices[19]);
		level->indexArray.append(vertices[9], vertices[8], vertices[19], vertices[18]);
		level->indexArray.append(vertices[1], vertices[0], vertices[18], vertices[17]);
		level->indexArray.append(vertices[7], vertices[10], vertices[3], vertices[3]);
		level->indexArray.append(vertices[4], vertices[2], vertices[23], vertices[23]);
		level->indexArray.append(vertices[5], vertices[22], vertices[15], vertices[15]);
		level->indexArray.append(vertices[11], vertices[6], vertices[14], vertices[14]);
		level->indexArray.append(vertices[16], vertices[12], vertices[21], vertices[21]);
		level->indexArray.append(vertices[13], vertices[19], vertices[8], vertices[8]);
		level->indexArray.append(vertices[0], vertices[9], vertices[18], vertices[18]);
		level->indexArray.append(vertices[17], vertices[20], vertices[1], vertices[1]);

		allocVertex(vertices[3], 1);
		allocVertex(vertices[2], 1);
		allocVertex(vertices[4], 1);
		allocVertex(vertices[7], 1);
		allocVertex(vertices[11], 1);
		allocVertex(vertices[10], 1);
		allocVertex(vertices[7], 1);
		allocVertex(vertices[6], 1);
		allocVertex(vertices[6], 1);
		allocVertex(vertices[5], 1);
		allocVertex(vertices[15], 1);
		allocVertex(vertices[14], 1);
		allocVertex(vertices[5], 1);
		allocVertex(vertices[4], 1);
		allocVertex(vertices[23], 1);
		allocVertex(vertices[22], 1);
		allocVertex(vertices[10], 1);
		allocVertex(vertices[9], 1);
		allocVertex(vertices[0], 1);
		allocVertex(vertices[3], 1);
		allocVertex(vertices[2], 1);
		allocVertex(vertices[1], 1);
		allocVertex(vertices[20], 1);
		allocVertex(vertices[23], 1);
		allocVertex(vertices[12], 1);
		allocVertex(vertices[15], 1);
		allocVertex(vertices[22], 1);
		allocVertex(vertices[21], 1);
		allocVertex(vertices[14], 1);
		allocVertex(vertices[13], 1);
		allocVertex(vertices[8], 1);
		allocVertex(vertices[11], 1);
		allocVertex(vertices[17], 1);
		allocVertex(vertices[16], 1);
		allocVertex(vertices[21], 1);
		allocVertex(vertices[20], 1);
		allocVertex(vertices[13], 1);
		allocVertex(vertices[12], 1);
		allocVertex(vertices[16], 1);
		allocVertex(vertices[19], 1);
		allocVertex(vertices[9], 1);
		allocVertex(vertices[8], 1);
		allocVertex(vertices[19], 1);
		allocVertex(vertices[18], 1);
		allocVertex(vertices[1], 1);
		allocVertex(vertices[0], 1);
		allocVertex(vertices[18], 1);
		allocVertex(vertices[17], 1);
		allocVertex(vertices[7], 1);
		allocVertex(vertices[10], 1);
		allocVertex(vertices[3], 1);
		allocVertex(vertices[3], 1);
		allocVertex(vertices[4], 1);
		allocVertex(vertices[2], 1);
		allocVertex(vertices[23], 1);
		allocVertex(vertices[23], 1);
		allocVertex(vertices[5], 1);
		allocVertex(vertices[22], 1);
		allocVertex(vertices[15], 1);
		allocVertex(vertices[15], 1);
		allocVertex(vertices[11], 1);
		allocVertex(vertices[6], 1);
		allocVertex(vertices[14], 1);
		allocVertex(vertices[14], 1);
		allocVertex(vertices[16], 1);
		allocVertex(vertices[12], 1);
		allocVertex(vertices[21], 1);
		allocVertex(vertices[21], 1);
		allocVertex(vertices[13], 1);
		allocVertex(vertices[19], 1);
		allocVertex(vertices[8], 1);
		allocVertex(vertices[8], 1);
		allocVertex(vertices[0], 1);
		allocVertex(vertices[9], 1);
		allocVertex(vertices[18], 1);
		allocVertex(vertices[18], 1);
		allocVertex(vertices[17], 1);
		allocVertex(vertices[20], 1);
		allocVertex(vertices[1], 1);
		allocVertex(vertices[1], 1);

		for (int i = 0; i < 24; i++) {
			allocVertex(vertices[i], -1);
		}
	}
}

// FUNCTION: ROBLOX 0x00514960
void BrickMesh::FUN_00514960(
	const G3D::Vector3& halfSize, std::vector<G3D::Vector3>& vertices, std::vector<unsigned short>& indices
)
{
	vertices.reserve(8);
	indices.reserve(36);

	vertices.push_back(G3D::Vector3(-halfSize.x, -halfSize.y, -halfSize.z));
	vertices.push_back(G3D::Vector3(-halfSize.x, -halfSize.y, halfSize.z));
	vertices.push_back(G3D::Vector3(-halfSize.x, halfSize.y, -halfSize.z));
	vertices.push_back(G3D::Vector3(-halfSize.x, halfSize.y, halfSize.z));
	vertices.push_back(G3D::Vector3(halfSize.x, -halfSize.y, -halfSize.z));
	vertices.push_back(G3D::Vector3(halfSize.x, -halfSize.y, halfSize.z));
	vertices.push_back(G3D::Vector3(halfSize.x, halfSize.y, -halfSize.z));
	vertices.push_back(G3D::Vector3(halfSize.x, halfSize.y, halfSize.z));

	appendTriangles(indices, 7, 5, 4, 6);
	appendTriangles(indices, 2, 0, 1, 3);
	appendTriangles(indices, 7, 6, 2, 3);
	appendTriangles(indices, 1, 0, 4, 5);
	appendTriangles(indices, 3, 1, 5, 7);
	appendTriangles(indices, 6, 4, 0, 2);
}

// FUNCTION: ROBLOX 0x00514b10
BrickMesh::BrickMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes, BrickColor color)
{
	m_unk0x3c = 1;
	FUN_00514070(size, surfaceTypes, color);

	if (ViewBase::getShadowsEnabled()) {
		G3D::Vector3 halfSize =
			G3D::Vector3(size.x * 0.5, size.y * 0.5, size.z * 0.5) -
			G3D::Vector3(widthInset + g_unk0x00958544, g_unk0x00958544, widthInset + g_unk0x00958544);
		FUN_00514960(halfSize, m_unk0x28, m_unk0x18);
	}
}

// STUB: ROBLOX 0x00514c00
G3D::ReferenceCountedPointer<RenderNew::Mesh> BrickMesh::create(
	const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes, BrickColor color
)
{
	Lookup lookup = {size, surfaceTypes, color};
	Variations& variations = cache[lookup];

	int index;
	if (variations.count == 4) {
		index = G3D::iRandom(0, 3);
	}
	else {
		index = std::min(3, G3D::iRandom(0, 8));
	}

	G3D::ReferenceCountedPointer<RenderNew::Mesh>& slot = variations.meshes[index];
	G3D::ReferenceCountedPointer<RenderNew::Mesh> result = slot;
	if (result.isNull()) {
		g_unk0x00958541 = false;
		if (g_unk0x0092a908) {
			g_unk0x00958544 = 0.065f;
			result = new BrickMesh(size, surfaceTypes, color);

			g_unk0x00958544 = 0.0f;
			g_unk0x00958541 = true;
			dynamic_cast<BrickMesh*>(result.pointer())->FUN_00514070(size, surfaceTypes, color);
			g_unk0x00958541 = false;
		}
		else {
			g_unk0x00958544 = 0.0f;
			result = new BrickMesh(size, surfaceTypes, color);
		}

		slot = result;
		variations.count++;
	}

	return result;
}

} // namespace ViewNew
} // namespace RBX
