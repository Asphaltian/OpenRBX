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

class SphereBuilder : public LevelBuilder
{
private:
	const int elements;

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
	virtual void buildRight(Purpose purpose) { buildFace<RBX::NORM_X>(SphereTransform, elements, elements, purpose); }

	virtual void buildTop(Purpose purpose) { buildFace<RBX::NORM_Y>(SphereTransform, elements, elements, purpose); }

	virtual void buildBack(Purpose purpose) { buildFace<RBX::NORM_Z>(SphereTransform, elements, elements, purpose); }

	virtual void buildLeft(Purpose purpose)
	{
		buildFace<RBX::NORM_X_NEG>(SphereTransform, elements, elements, purpose);
	}

	virtual void buildBottom(Purpose purpose)
	{
		buildFace<RBX::NORM_Y_NEG>(SphereTransform, elements, elements, purpose);
	}

	virtual void buildFront(Purpose purpose)
	{
		buildFace<RBX::NORM_Z_NEG>(SphereTransform, elements, elements, purpose);
	}
};

SphereMesh::SphereMesh(const G3D::Vector3& diameter, RenderSurfaceTypes surfaceTypes)
{
	m_unk0x3c = 2;

	levels.append(new RenderNew::Mesh::Level(G3D::RenderDevice::QUADS));
	{
		RenderNew::Mesh::LevelRef& level = levels.last();
		SphereBuilder builder(level, diameter, surfaceTypes, 6);
		builder.build(LevelBuilder::Surface);
	}
}

SphereMesh::SphereMesh(const G3D::Vector3& size, NormalId decalFace)
{
	if (false) {
		RenderNew::Mesh::LevelRef tempShadowLevel = new RenderNew::Mesh::Level(G3D::RenderDevice::TRIANGLES);
		SphereBuilder builder(tempShadowLevel, size, RenderSurfaceTypes(), 3);
		builder.build(LevelBuilder::Shadow);
	}

	RenderNew::Mesh::LevelRef level = new RenderNew::Mesh::Level(G3D::RenderDevice::QUADS);

	levels.append(level);

	SphereBuilder builder(level, size, RenderSurfaceTypes(), 6);

	builder.buildFace(decalFace, LevelBuilder::Decal);
}

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
