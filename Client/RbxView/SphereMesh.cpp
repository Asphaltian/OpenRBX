#include "SphereMesh.h"

#include "QuadVolume.h"
#include "ViewBase.h"

#include <G3D/Vector2int16.h>

// FUNCTION: WEBSERVICE 0x101840e0
// FUNCTION: ROBLOX 0x0048e600
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
class SphereBuilder : public RBX::View::LevelBuilder
{
private:
	const int elements; // 0x20

public:
	SphereBuilder(
		G3D::ReferenceCountedPointer<RBX::Render::Mesh::Level>& level,
		G3D::Vector3 size,
		RBX::View::RenderSurfaceTypes surfaceTypes,
		int elements
	)
		: RBX::View::LevelBuilder(level, size, surfaceTypes), elements(elements)
	{
	}

protected:
	// FUNCTION: WEBSERVICE 0x10186850
	virtual void buildRight(Purpose purpose) // vtable+0x10
	{
		buildFace<RBX::NORM_X>(SphereTransform, elements, elements, purpose);
	}

	// FUNCTION: WEBSERVICE 0x10186880
	virtual void buildTop(Purpose purpose) // vtable+0x04
	{
		buildFace<RBX::NORM_Y>(SphereTransform, elements, elements, purpose);
	}

	// FUNCTION: WEBSERVICE 0x101868b0
	virtual void buildBack(Purpose purpose) // vtable+0x18
	{
		buildFace<RBX::NORM_Z>(SphereTransform, elements, elements, purpose);
	}

	// FUNCTION: WEBSERVICE 0x101868e0
	virtual void buildLeft(Purpose purpose) // vtable+0x0c
	{
		buildFace<RBX::NORM_X_NEG>(SphereTransform, elements, elements, purpose);
	}

	// FUNCTION: WEBSERVICE 0x10186910
	virtual void buildBottom(Purpose purpose) // vtable+0x08
	{
		buildFace<RBX::NORM_Y_NEG>(SphereTransform, elements, elements, purpose);
	}

	// FUNCTION: WEBSERVICE 0x10186940
	virtual void buildFront(Purpose purpose) // vtable+0x14
	{
		buildFace<RBX::NORM_Z_NEG>(SphereTransform, elements, elements, purpose);
	}
};

DECOMP_SIZE_ASSERT(SphereBuilder, 0x24)

namespace RBX {
namespace View {

// FUNCTION: WEBSERVICE 0x10186970
// FUNCTION: ROBLOX 0x00490e90
SphereMesh::SphereMesh(const G3D::Vector3& diameter, RenderSurfaceTypes surfaceTypes)
{
	levels.append(new Render::Mesh::Level(G3D::RenderDevice::QUADS));
	{
		Render::Mesh::LevelRef& level = levels.last();
		SphereBuilder builder(level, diameter, surfaceTypes, 6);
		builder.build(LevelBuilder::Surface);
	}

	if (ViewBase::getShadowsEnabled()) {
		int elements = (diameter.x < 40.0f && diameter.y < 40.0f && diameter.z < 40.0f) ? 3 : 4;

		Render::Mesh::LevelRef tempShadowLevel = new Render::Mesh::Level(G3D::RenderDevice::TRIANGLES);
		SphereBuilder builder(tempShadowLevel, diameter, surfaceTypes, elements);
		builder.build(LevelBuilder::Shadow);
		computeShadowSurface(tempShadowLevel);
	}
}

// FUNCTION: WEBSERVICE 0x10186c90
// FUNCTION: ROBLOX 0x004911b0
SphereMesh::SphereMesh(const G3D::Vector3& size, NormalId decalFace)
{
	if (false) {
		Render::Mesh::LevelRef tempShadowLevel = new Render::Mesh::Level(G3D::RenderDevice::TRIANGLES);
		SphereBuilder builder(tempShadowLevel, size, RenderSurfaceTypes(), 3);
		builder.build(LevelBuilder::Shadow);
		computeShadowSurface(tempShadowLevel);
	}

	Render::Mesh::LevelRef level = new Render::Mesh::Level(G3D::RenderDevice::QUADS);

	levels.append(level);

	SphereBuilder builder(level, size, RenderSurfaceTypes(), 6);

	builder.buildFace(decalFace, LevelBuilder::Decal);
}

// FUNCTION: WEBSERVICE 0x10186e10
// FUNCTION: ROBLOX 0x00491330
SphereMesh::SphereMesh(const G3D::Vector3& size, NormalId textureFace, const G3D::Vector2& studsPerTile)
{
	Render::Mesh::LevelRef level = new Render::Mesh::Level(G3D::RenderDevice::QUADS);

	levels.append(level);

	SphereBuilder builder(level, size, RenderSurfaceTypes(), 6);

	builder.textureScale = mapToUvw_Legacy(size, textureFace).xy() * 2.0 / studsPerTile;

	builder.buildFace(textureFace, LevelBuilder::Decal);
}

template void MeshFactory<SphereMesh, 1>::flushCache();

template SphereMesh* MeshFactory<SphereMesh, 1>::create(const G3D::Vector3&, RenderSurfaceTypes);

template SphereMesh* MeshFactory<SphereMesh, 1>::createDecal(const G3D::Vector3&, NormalId);

template SphereMesh* MeshFactory<SphereMesh, 1>::createTexture(const G3D::Vector3&, NormalId, const G3D::Vector2&);

} // namespace View
} // namespace RBX
