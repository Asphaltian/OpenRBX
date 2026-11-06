#include "CylinderMesh.h"

#include "QuadVolume.h"

#include <G3D/Vector2int16.h>
#include <G3D/vectorMath.h>
#include <algorithm>
#include <math.h>

namespace RBX {
namespace ViewNew {

static void EndcapTransform(
	G3D::Vector3& vertex,
	G3D::Vector3& normal,
	G3D::Vector2& tex,
	const G3D::Vector3& radius,
	const G3D::Vector2int16& ivertex
);

static void CylinderTransform(
	G3D::Vector3& vertex,
	G3D::Vector3& normal,
	G3D::Vector2& tex,
	const G3D::Vector3& radius,
	const G3D::Vector2int16& ivertex
);

// SIZE 0x24
class CylinderBuilder : public LevelBuilder
{
public:
	const unsigned int elements; // 0x20

	// FUNCTION: ROBLOX 0x005287c0
	CylinderBuilder(
		G3D::ReferenceCountedPointer<RBX::RenderNew::Mesh::Level>& level,
		G3D::Vector3 size,
		RenderSurfaceTypes surfaceTypes,
		unsigned int slices
	)
		: LevelBuilder(level, size, surfaceTypes), elements(std::max(slices / 4, 1u))
	{
	}

protected:
	virtual void buildRight(Purpose purpose);  // vtable+0x10
	virtual void buildTop(Purpose purpose);    // vtable+0x04
	virtual void buildBack(Purpose purpose);   // vtable+0x18
	virtual void buildLeft(Purpose purpose);   // vtable+0x0c
	virtual void buildBottom(Purpose purpose); // vtable+0x08
	virtual void buildFront(Purpose purpose);  // vtable+0x14
};

DECOMP_SIZE_ASSERT(CylinderBuilder, 0x24)

static void EndcapTransform(
	G3D::Vector3& vertex,
	G3D::Vector3& normal,
	G3D::Vector2& tex,
	const G3D::Vector3& radius,
	const G3D::Vector2int16& ivertex
)
{
	float ratio;
	if (fabs(vertex.y) >= fabs(vertex.x)) {
		if (vertex.y == 0) {
			ratio = 0;
		}
		else {
			ratio = fabs(vertex.y) / vertex.xy().length();
		}
	}
	else {
		if (vertex.x == 0) {
			ratio = 0;
		}
		else {
			ratio = fabs(vertex.x) / vertex.xy().length();
		}
	}
	vertex.x *= ratio;
	vertex.y *= ratio;
}

static G3D::Vector2 unitVector(const G3D::Vector2& v)
{
	return v * (1.0f / v.length());
}

static void CylinderTransform(
	G3D::Vector3& vertex,
	G3D::Vector3& normal,
	G3D::Vector2& tex,
	const G3D::Vector3& radius,
	const G3D::Vector2int16& ivertex
)
{
	G3D::Vector2 v = unitVector(vertex.yz());
	vertex.y = v.x * radius.x;
	vertex.z = v.y * radius.y;
	normal = G3D::Vector3(0, v.x, v.y);
}

// FUNCTION: ROBLOX 0x00528920
inline void CylinderBuilder::buildRight(Purpose purpose)
{
	buildFace<RBX::NORM_X>(EndcapTransform, elements, elements, purpose);
}

// FUNCTION: ROBLOX 0x00528950
inline void CylinderBuilder::buildTop(Purpose purpose)
{
	buildFace<RBX::NORM_Y>(CylinderTransform, elements, 1, purpose);
}

// FUNCTION: ROBLOX 0x00528980
inline void CylinderBuilder::buildBack(Purpose purpose)
{
	buildFace<RBX::NORM_Z>(CylinderTransform, elements, 1, purpose);
}

// FUNCTION: ROBLOX 0x005289b0
inline void CylinderBuilder::buildLeft(Purpose purpose)
{
	buildFace<RBX::NORM_X_NEG>(EndcapTransform, elements, elements, purpose);
}

// FUNCTION: ROBLOX 0x005289e0
inline void CylinderBuilder::buildBottom(Purpose purpose)
{
	buildFace<RBX::NORM_Y_NEG>(CylinderTransform, elements, 1, purpose);
}

// FUNCTION: ROBLOX 0x00528a10
inline void CylinderBuilder::buildFront(Purpose purpose)
{
	buildFace<RBX::NORM_Z_NEG>(CylinderTransform, elements, 1, purpose);
}

CylinderAlongXMesh::CylinderAlongXMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes)
{
	levels.append(new RenderNew::Mesh::Level(G3D::RenderDevice::QUADS));
	{
		RenderNew::Mesh::LevelRef& level = levels.last();
		CylinderBuilder builder(level, size, surfaceTypes, 12);
		builder.build(LevelBuilder::Surface);
	}
}

CylinderAlongXMesh::CylinderAlongXMesh(const G3D::Vector3& size, NormalId decalFace)
{
	RenderNew::Mesh::LevelRef level = new RenderNew::Mesh::Level(G3D::RenderDevice::QUADS);

	levels.append(level);

	CylinderBuilder builder(level, size, RenderSurfaceTypes(), 12);

	builder.buildFace(decalFace, LevelBuilder::Decal);
}

CylinderAlongXMesh::CylinderAlongXMesh(const G3D::Vector3& size, NormalId textureFace, const G3D::Vector2& studsPerTile)
{
	RenderNew::Mesh::LevelRef level = new RenderNew::Mesh::Level(G3D::RenderDevice::QUADS);

	levels.append(level);

	CylinderBuilder builder(level, size, RenderSurfaceTypes(), 12);

	builder.textureScale = mapToUvw_Legacy(size, textureFace).xy() * 2.0 / studsPerTile;

	builder.buildFace(textureFace, LevelBuilder::Decal);
}

template void MeshFactory<CylinderAlongXMesh, 1>::flushCache();

template CylinderAlongXMesh* MeshFactory<CylinderAlongXMesh, 1>::create(const G3D::Vector3&, RenderSurfaceTypes);

template CylinderAlongXMesh* MeshFactory<CylinderAlongXMesh, 1>::createDecal(const G3D::Vector3&, NormalId);

template CylinderAlongXMesh* MeshFactory<CylinderAlongXMesh, 1>::createTexture(
	const G3D::Vector3&, NormalId, const G3D::Vector2&
);

} // namespace ViewNew
} // namespace RBX
