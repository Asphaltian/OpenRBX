#include "BrickMesh.h"

#include "v8datamodel/BrickColor.h"

#include <map>

// FUNCTION: WEBSERVICE 0x10175d10
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

namespace RBX {
namespace View {

const float BrickMesh::widthInset = 0.0f;

const float BrickMesh::normalPerturbation = 0.05f;

const float BrickMesh::bevel = 0.0f;

// STUB: WEBSERVICE 0x10179d10
BrickMesh::BrickMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes, BrickColor color)
{
	STUB(0x10179d10);
}

// FUNCTION: WEBSERVICE 0x1017a900
void BrickMesh::flushCache()
{
	cache.clear();
}

// STUB: WEBSERVICE 0x1017aaa0
G3D::ReferenceCountedPointer<Render::Mesh> BrickMesh::create(
	const G3D::Vector3& size,
	RenderSurfaceTypes surfaceTypes,
	BrickColor color
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
