#include "SphereMesh.h"

#include "QuadVolume.h"

#include <G3D/Vector2int16.h>

namespace RBX {
namespace ViewNew {

static void SphereTransform(
	G3D::Vector3& vertex,
	G3D::Vector3& normal,
	G3D::Vector2& tex,
	const G3D::Vector3& radius,
	const G3D::Vector2int16& ivertex
)
{
	normal = vertex.unit();
	vertex = normal * radius;
}

// SIZE 0x24
class SphereBuilder : public LevelBuilder
{
private:
	const int elements; // 0x20

public:
	SphereBuilder(
		G3D::ReferenceCountedPointer<RBX::RenderNew::Mesh::Level>& level,
		G3D::Vector3 size,
		RenderSurfaceTypes surfaceTypes,
		int elements
	)
		: LevelBuilder(level, size, surfaceTypes), elements(elements)
	{
	}

protected:
	// FUNCTION: ROBLOX 0x005281c0
	virtual void buildRight(Purpose purpose) // vtable+0x10
	{
		buildFace<RBX::NORM_X>(SphereTransform, elements, elements, purpose);
	}

	// FUNCTION: ROBLOX 0x005281f0
	virtual void buildTop(Purpose purpose) // vtable+0x04
	{
		buildFace<RBX::NORM_Y>(SphereTransform, elements, elements, purpose);
	}

	// FUNCTION: ROBLOX 0x00528220
	virtual void buildBack(Purpose purpose) // vtable+0x18
	{
		buildFace<RBX::NORM_Z>(SphereTransform, elements, elements, purpose);
	}

	// FUNCTION: ROBLOX 0x00528250
	virtual void buildLeft(Purpose purpose) // vtable+0x0c
	{
		buildFace<RBX::NORM_X_NEG>(SphereTransform, elements, elements, purpose);
	}

	// FUNCTION: ROBLOX 0x00528280
	virtual void buildBottom(Purpose purpose) // vtable+0x08
	{
		buildFace<RBX::NORM_Y_NEG>(SphereTransform, elements, elements, purpose);
	}

	// FUNCTION: ROBLOX 0x005282b0
	virtual void buildFront(Purpose purpose) // vtable+0x14
	{
		buildFace<RBX::NORM_Z_NEG>(SphereTransform, elements, elements, purpose);
	}
};

DECOMP_SIZE_ASSERT(SphereBuilder, 0x24)

// STUB: ROBLOX 0x005282e0
SphereMesh::SphereMesh(const G3D::Vector3& diameter, RenderSurfaceTypes surfaceTypes)
{
	levels.append(new RenderNew::Mesh::Level(G3D::RenderDevice::QUADS));
	{
		RenderNew::Mesh::LevelRef& level = levels.last();
		SphereBuilder builder(level, diameter, surfaceTypes, 6);
		builder.build(LevelBuilder::Surface);
	}
}

// STUB: ROBLOX 0x00528470
SphereMesh::SphereMesh(const G3D::Vector3& size, NormalId decalFace)
{
	RenderNew::Mesh::LevelRef level = new RenderNew::Mesh::Level(G3D::RenderDevice::QUADS);

	levels.append(level);

	SphereBuilder builder(level, size, RenderSurfaceTypes(), 6);

	builder.buildFace(decalFace, LevelBuilder::Decal);
}

// FUNCTION: ROBLOX 0x005285f0
SphereMesh::SphereMesh(const G3D::Vector3& size, NormalId textureFace, const G3D::Vector2& studsPerTile)
{
	RenderNew::Mesh::LevelRef level = new RenderNew::Mesh::Level(G3D::RenderDevice::QUADS);

	levels.append(level);

	SphereBuilder builder(level, size, RenderSurfaceTypes(), 6);

	builder.textureScale = mapToUvw_Legacy(size, textureFace).xy() * 2.0 / studsPerTile;

	builder.buildFace(textureFace, LevelBuilder::Decal);
}

template void MeshFactory<SphereMesh, 1>::flushCache();

template SphereMesh* MeshFactory<SphereMesh, 1>::create(const G3D::Vector3&, RenderSurfaceTypes);

template SphereMesh* MeshFactory<SphereMesh, 1>::createDecal(const G3D::Vector3&, NormalId);

template SphereMesh* MeshFactory<SphereMesh, 1>::createTexture(const G3D::Vector3&, NormalId, const G3D::Vector2&);

} // namespace ViewNew
} // namespace RBX
