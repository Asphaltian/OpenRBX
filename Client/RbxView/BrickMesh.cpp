#include "BrickMesh.h"

#include "QuadVolume.h"
#include "ViewBase.h"
#include "util/Math.h"
#include "v8datamodel/BrickColor.h"

#include <G3D/g3dmath.h>
#include <algorithm>
#include <map>

// FUNCTION: WEBSERVICE 0x10175d10
// FUNCTION: ROBLOX 0x004800f0
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
	G3D::Vector3 size;                          // 0x00
	RBX::View::RenderSurfaceTypes surfaceTypes; // 0x0c
	RBX::BrickColor color;                      // 0x10

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
	int count;                                                 // 0x00
	G3D::ReferenceCountedPointer<RBX::Render::Mesh> meshes[4]; // 0x04

	Variations() : count(0) {}
};

static std::map<Lookup, Variations> cache;

const float RBX::View::BrickMesh::widthInset = 0.0f;

const float RBX::View::BrickMesh::normalPerturbation = 0.05f;

const float RBX::View::BrickMesh::bevel = 0.0f;

static G3D::Vector2 flatTex(1.0f / 64.0f, 8.0f / 9.0f);

template <int N>
void meshFace(
	const G3D::Vector3& halfSize,
	float inset,
	RBX::View::RenderSurfaceTypes surfaceTypes,
	float colorIndex,
	RBX::Render::Mesh::LevelRef& level
)
{
	G3D::Vector3 faceCorner =
		G3D::abs(RBX::uvwToObject<N>(halfSize)) - G3D::Vector3(inset, inset, RBX::View::BrickMesh::bevel);

	G3D::Vector2 studsPerTile(0.0f, 0.0f);
	G3D::Vector2 textureOffset(0.0f, 0.0f);

	RBX::View::RenderSurfaceType surfaceType = surfaceTypes.getSurfaceType((RBX::NormalId) N);
	switch (surfaceType) {
	case RBX::View::FLAT:
		textureOffset = flatTex;
		studsPerTile = faceCorner.xy() * 2.0;
		break;
	case RBX::View::STUDS:
		studsPerTile = G3D::Vector2(2.0f, 4.0f);
		break;
	case RBX::View::INLET:
		studsPerTile = G3D::Vector2(2.0f, 4.0f);
		textureOffset.y = 1.0f / 3.0f;
		break;
	case RBX::View::WELD:
		studsPerTile = G3D::Vector2(2.0f, 2.0f);
		textureOffset.y = 2.0f / 3.0f;
		break;
	case RBX::View::GLUE:
		studsPerTile = G3D::Vector2(2.0f, 2.0f);
		textureOffset.y = 5.0f / 6.0f;
		break;
	}

	textureOffset.x += colorIndex / 32.0;

	G3D::Vector2 perturbationMin;
	perturbationMin.x =
		G3D::uniformRandom(-RBX::View::BrickMesh::normalPerturbation, RBX::View::BrickMesh::normalPerturbation);
	perturbationMin.y =
		G3D::uniformRandom(-RBX::View::BrickMesh::normalPerturbation, RBX::View::BrickMesh::normalPerturbation);

	G3D::Vector2 perturbationMax;
	perturbationMax.x = G3D::uniformRandom() > 0.5f ? perturbationMin.x : -perturbationMin.x;
	perturbationMax.y = G3D::uniformRandom() > 0.5f ? perturbationMin.y : -perturbationMin.y;

	if (surfaceType == RBX::View::FLAT) {
		G3D::Vector3 nxy(perturbationMin.x, perturbationMin.y, 1.0f);
		G3D::Vector3 nxY(perturbationMin.x, perturbationMax.y, 1.0f);
		G3D::Vector3 nXy(perturbationMax.x, perturbationMin.y, 1.0f);
		G3D::Vector3 nXY(perturbationMax.x, perturbationMax.y, 1.0f);

		unsigned int i0 = RBX::Render::Mesh::allocVertex(
			RBX::uvwToObject<N>(G3D::Vector3(-faceCorner.x, -faceCorner.y, faceCorner.z)),
			RBX::uvwToObject<N>(nxy),
			textureOffset,
			true
		);
		unsigned int i1 = RBX::Render::Mesh::allocVertex(
			RBX::uvwToObject<N>(G3D::Vector3(faceCorner.x, -faceCorner.y, faceCorner.z)),
			RBX::uvwToObject<N>(nXy),
			textureOffset,
			true
		);
		unsigned int i2 = RBX::Render::Mesh::allocVertex(
			RBX::uvwToObject<N>(G3D::Vector3(faceCorner.x, faceCorner.y, faceCorner.z)),
			RBX::uvwToObject<N>(nXY),
			textureOffset,
			true
		);
		unsigned int i3 = RBX::Render::Mesh::allocVertex(
			RBX::uvwToObject<N>(G3D::Vector3(-faceCorner.x, faceCorner.y, faceCorner.z)),
			RBX::uvwToObject<N>(nxY),
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
					textureOffset.y + (maxCorner.y - minCorner.y) / 12.0 - RBX::View::BrickMesh::bevel
				);
				G3D::Vector2 t1(
					textureOffset.x - 1.0 / 1024.0 + (maxCorner.x - minCorner.x) / 64.0,
					textureOffset.y + (maxCorner.y - minCorner.y) / 12.0 - RBX::View::BrickMesh::bevel
				);
				G3D::Vector2 t2(
					textureOffset.x - 1.0 / 1024.0 + (maxCorner.x - minCorner.x) / 64.0,
					textureOffset.y - RBX::View::BrickMesh::bevel
				);
				G3D::Vector2 t3(textureOffset.x + 1.0 / 1024.0, textureOffset.y + RBX::View::BrickMesh::bevel);

				unsigned int i0 = RBX::Render::Mesh::allocVertex(
					RBX::uvwToObject<N>(G3D::Vector3(minCorner.x, minCorner.y, faceCorner.z)),
					RBX::uvwToObject<N>(G3D::Vector3(minNormal.x, minNormal.y, 1.0f)),
					t0,
					true
				);
				unsigned int i1 = RBX::Render::Mesh::allocVertex(
					RBX::uvwToObject<N>(G3D::Vector3(maxCorner.x, minCorner.y, faceCorner.z)),
					RBX::uvwToObject<N>(G3D::Vector3(maxNormal.x, minNormal.y, 1.0f)),
					t1,
					true
				);
				unsigned int i2 = RBX::Render::Mesh::allocVertex(
					RBX::uvwToObject<N>(G3D::Vector3(maxCorner.x, maxCorner.y, faceCorner.z)),
					RBX::uvwToObject<N>(G3D::Vector3(maxNormal.x, maxNormal.y, 1.0f)),
					t2,
					true
				);
				unsigned int i3 = RBX::Render::Mesh::allocVertex(
					RBX::uvwToObject<N>(G3D::Vector3(minCorner.x, maxCorner.y, faceCorner.z)),
					RBX::uvwToObject<N>(G3D::Vector3(minNormal.x, maxNormal.y, 1.0f)),
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

// FUNCTION: WEBSERVICE 0x10179710
// FUNCTION: ROBLOX 0x00483cd0
static void appendTriangles(RBX::Render::Mesh::Level& level, int a, int b, int c, int d)
{
	level.indexArray.append(
		RBX::Render::Mesh::allocVertex(a, 1), RBX::Render::Mesh::allocVertex(b, 1), RBX::Render::Mesh::allocVertex(c, 1)
	);
	level.indexArray.append(
		RBX::Render::Mesh::allocVertex(a, 1), RBX::Render::Mesh::allocVertex(c, 1), RBX::Render::Mesh::allocVertex(d, 1)
	);
}

namespace RBX {
namespace View {

// FUNCTION: WEBSERVICE 0x10179d10
// FUNCTION: ROBLOX 0x004841a0
BrickMesh::BrickMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes, BrickColor color)
{
	{
		Render::Mesh::LevelRef level = new Render::Mesh::Level(G3D::RenderDevice::QUADS);
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

	if (ViewBase::getShadowsEnabled()) {
		Render::Mesh::LevelRef shadowLevel = new Render::Mesh::Level(G3D::RenderDevice::TRIANGLES);

		G3D::Vector3 halfSize = G3D::Vector3(size.x * 0.5, size.y * 0.5, size.z * 0.5) -
								G3D::Vector3(widthInset + bevel, bevel, widthInset + bevel);

		unsigned int xyz = allocVertex(
			G3D::Vector3(-halfSize.x, -halfSize.y, -halfSize.z), G3D::Vector3::zero(), G3D::Vector2::zero(), true
		);
		unsigned int xyZ = allocVertex(
			G3D::Vector3(-halfSize.x, -halfSize.y, halfSize.z), G3D::Vector3::zero(), G3D::Vector2::zero(), true
		);
		unsigned int xYz = allocVertex(
			G3D::Vector3(-halfSize.x, halfSize.y, -halfSize.z), G3D::Vector3::zero(), G3D::Vector2::zero(), true
		);
		unsigned int xYZ = allocVertex(
			G3D::Vector3(-halfSize.x, halfSize.y, halfSize.z), G3D::Vector3::zero(), G3D::Vector2::zero(), true
		);
		unsigned int Xyz = allocVertex(
			G3D::Vector3(halfSize.x, -halfSize.y, -halfSize.z), G3D::Vector3::zero(), G3D::Vector2::zero(), true
		);
		unsigned int XyZ = allocVertex(
			G3D::Vector3(halfSize.x, -halfSize.y, halfSize.z), G3D::Vector3::zero(), G3D::Vector2::zero(), true
		);
		unsigned int XYz = allocVertex(
			G3D::Vector3(halfSize.x, halfSize.y, -halfSize.z), G3D::Vector3::zero(), G3D::Vector2::zero(), true
		);
		unsigned int XYZ = allocVertex(
			G3D::Vector3(halfSize.x, halfSize.y, halfSize.z), G3D::Vector3::zero(), G3D::Vector2::zero(), true
		);

		appendTriangles(*shadowLevel, XYZ, XyZ, Xyz, XYz);
		appendTriangles(*shadowLevel, xYz, xyz, xyZ, xYZ);
		appendTriangles(*shadowLevel, XYZ, XYz, xYz, xYZ);
		appendTriangles(*shadowLevel, xyZ, xyz, Xyz, XyZ);
		appendTriangles(*shadowLevel, xYZ, xyZ, XyZ, XYZ);
		appendTriangles(*shadowLevel, XYz, Xyz, xyz, xYz);

		freeVertex(xyz);
		freeVertex(xyZ);
		freeVertex(xYz);
		freeVertex(xYZ);
		freeVertex(Xyz);
		freeVertex(XyZ);
		freeVertex(XYz);
		freeVertex(XYZ);

		computeShadowSurface(shadowLevel);
	}
}

// FUNCTION: WEBSERVICE 0x1017a900
// FUNCTION: ROBLOX 0x00484c90
void BrickMesh::flushCache()
{
	cache.clear();
}

// FUNCTION: WEBSERVICE 0x1017aaa0
// FUNCTION: ROBLOX 0x00484e30
G3D::ReferenceCountedPointer<Render::Mesh> BrickMesh::create(
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

	G3D::ReferenceCountedPointer<Render::Mesh> result = variations.meshes[index];
	if (result.isNull()) {
		result = new BrickMesh(size, surfaceTypes, color);
		variations.meshes[index] = result;
		variations.count++;
	}

	return result;
}

} // namespace View
} // namespace RBX
