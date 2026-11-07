#include "BrickMesh.h"

#include "QuadVolume.h"
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

static G3D::Vector2 flatTex(1.0f / 64.0f, 8.0f / 9.0f);

template <int N>
void meshFace(
	const G3D::Vector3& halfSize,
	float inset,
	RenderSurfaceTypes surfaceTypes,
	float colorIndex,
	RBX::RenderNew::Mesh::LevelRef& level
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

	G3D::Vector2 perturbationMin;
	perturbationMin.x = G3D::uniformRandom(-BrickMesh::normalPerturbation, BrickMesh::normalPerturbation);
	perturbationMin.y = G3D::uniformRandom(-BrickMesh::normalPerturbation, BrickMesh::normalPerturbation);

	G3D::Vector2 perturbationMax;
	perturbationMax.x = G3D::uniformRandom() > 0.5f ? perturbationMin.x : -perturbationMin.x;
	perturbationMax.y = G3D::uniformRandom() > 0.5f ? perturbationMin.y : -perturbationMin.y;

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
	}
	else {

		G3D::Vector2 minCorner;
		minCorner.x = -faceCorner.x;

		G3D::Vector2 normalDelta = (perturbationMax - perturbationMin) * 0.5 / faceCorner.xy();

		G3D::Vector2 minNormal;
		minNormal.x = perturbationMin.x;

		while (minCorner.x < faceCorner.x) {
			G3D::Vector2 maxCorner;
			maxCorner.x = std::min(minCorner.x + studsPerTile.x, faceCorner.x);

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

				level->indexArray.append(i0, i1, i2, i3);

				minCorner.y = maxCorner.y;
				minNormal.y = maxNormal.y;
			}

			minCorner.x = maxCorner.x;
			minNormal.x = maxNormal.x;
		}
	}
}

BrickMesh::BrickMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes, BrickColor color)
{
	{
		RenderNew::Mesh::LevelRef level = new RenderNew::Mesh::Level(G3D::RenderDevice::QUADS);
		levels.append(level);

		G3D::Vector3 halfSize(size.x * 0.5 - widthInset, size.y * 0.5, size.z * 0.5 - widthInset);

		const float colorIndex = color.getClosestPaletteIndex();

		meshFace<0>(halfSize, bevel, surfaceTypes, colorIndex, level);
		meshFace<1>(halfSize, bevel, surfaceTypes, colorIndex, level);
		meshFace<2>(halfSize, bevel, surfaceTypes, colorIndex, level);
		meshFace<3>(halfSize, bevel, surfaceTypes, colorIndex, level);
		meshFace<4>(halfSize, bevel, surfaceTypes, colorIndex, level);
		meshFace<5>(halfSize, bevel, surfaceTypes, colorIndex, level);
	}
}

// FUNCTION: ROBLOX 0x0050eed0
void BrickMesh::flushCache()
{
	cache.clear();
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

	G3D::ReferenceCountedPointer<RenderNew::Mesh> result = variations.meshes[index];
	if (result.isNull()) {
		result = new BrickMesh(size, surfaceTypes, color);
		variations.meshes[index] = result;
		variations.count++;
	}

	return result;
}

} // namespace ViewNew
} // namespace RBX
