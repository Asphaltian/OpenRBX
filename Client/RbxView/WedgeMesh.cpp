#include "WedgeMesh.h"

#include "BevelMesh.h"
#include "QuadVolume.h"

#include <G3D/Vector2int16.h>
#include <math.h>

const float bevel = 0.065f;

// SIZE 0x8
class DeformSide3
{
private:
	const float inset; // 0x00
	const float flip;  // 0x04

public:
	DeformSide3(float inset, bool flip) : inset(inset), flip(flip ? -1.0f : 1.0f) {}

	void operator()(
		G3D::Vector3& vertex,
		G3D::Vector3& normal,
		G3D::Vector2& tex,
		const G3D::Vector3& radius,
		const G3D::Vector2int16& ivertex
	)
	{
		if (vertex.y > 0) {
			vertex.y -= inset;
			vertex.x = (fabs(vertex.x) - inset) * flip;
		}
		else {
			vertex.y += inset;
			if (vertex.x > 0) {
				vertex.x -= inset;
			}
			else {
				vertex.x += inset;
			}
		}
	}
};

DECOMP_SIZE_ASSERT(DeformSide3, 0x8)

// SIZE 0x4
class DeformTop3
{
private:
	const float inset; // 0x00

public:
	DeformTop3(float inset) : inset(inset) {}

	void operator()(
		G3D::Vector3& vertex,
		G3D::Vector3& normal,
		G3D::Vector2& tex,
		const G3D::Vector3& radius,
		const G3D::Vector2int16& ivertex
	)
	{
		if (vertex.x > 0) {
			vertex.x -= 2 * inset;
		}
		else {
			vertex.x += inset;
		}
		vertex.y = (vertex.y > 0 ? vertex.y : -vertex.y) - inset;
	}
};

DECOMP_SIZE_ASSERT(DeformTop3, 0x4)

// SIZE 0x4
class DeformFront
{
private:
	const float inset; // 0x00

public:
	DeformFront(float inset) : inset(inset) {}

	void operator()(
		G3D::Vector3& vertex,
		G3D::Vector3& normal,
		G3D::Vector2& tex,
		const G3D::Vector3& radius,
		const G3D::Vector2int16& ivertex
	)
	{
		if (vertex.x > 0) {
			vertex.x -= inset;
		}
		else {
			vertex.x += inset;
		}
		if (vertex.y > 0) {
			vertex.z = inset - vertex.z;
		}
		else {
			vertex.y += inset;
		}
		normal = G3D::Vector3(0, radius.z, radius.y);
	}
};

DECOMP_SIZE_ASSERT(DeformFront, 0x4)

// SIZE 0x24
class WedgeBuilder : public RBX::View::BevelMesh::Builder
{
public:
	WedgeBuilder(
		G3D::ReferenceCountedPointer<RBX::Render::Mesh::Level>& level,
		G3D::Vector3 size,
		RBX::View::RenderSurfaceTypes surfaceTypes,
		float bevel
	)
		: RBX::View::BevelMesh::Builder(level, size, surfaceTypes, bevel)
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

DECOMP_SIZE_ASSERT(WedgeBuilder, 0x24)

// FUNCTION: WEBSERVICE 0x101800c0
// FUNCTION: ROBLOX 0x0048a4f0
inline void WedgeBuilder::buildRight(Purpose purpose)
{
	buildFace<RBX::NORM_X>(DeformSide3(bevel, true), 1, 1, purpose);
}

// FUNCTION: WEBSERVICE 0x10180100
// FUNCTION: ROBLOX 0x0048a530
inline void WedgeBuilder::buildTop(Purpose purpose)
{
	buildFace<RBX::NORM_Y>(DeformTop3(bevel), 1, 1, purpose);
}

// FUNCTION: WEBSERVICE 0x10180140
// FUNCTION: ROBLOX 0x0048a570
inline void WedgeBuilder::buildLeft(Purpose purpose)
{
	buildFace<RBX::NORM_X_NEG>(DeformSide3(bevel, false), 1, 1, purpose);
}

// FUNCTION: WEBSERVICE 0x10180180
// FUNCTION: ROBLOX 0x0048a5b0
inline void WedgeBuilder::buildFront(Purpose purpose)
{
	buildFace<RBX::NORM_Z_NEG>(DeformFront(bevel), 1, 1, purpose);
}

namespace RBX {
namespace View {

// FUNCTION: WEBSERVICE 0x10180a60
// FUNCTION: ROBLOX 0x0048ae90
WedgeMesh::WedgeMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes)
{
	levels.append(new Render::Mesh::Level(G3D::RenderDevice::QUADS));
	{
		WedgeBuilder builder(levels.last(), size, surfaceTypes, 0);
		builder.build(LevelBuilder::Surface);
	}
}

// FUNCTION: WEBSERVICE 0x10180be0
// FUNCTION: ROBLOX 0x0048b010
WedgeMesh::WedgeMesh(const G3D::Vector3& size, NormalId decalFace)
{
	Render::Mesh::LevelRef level = new Render::Mesh::Level(G3D::RenderDevice::QUADS);

	levels.append(level);

	WedgeBuilder builder(level, size, RenderSurfaceTypes(), bevel);

	builder.buildFace(decalFace, LevelBuilder::Decal);
}

// FUNCTION: WEBSERVICE 0x10180d60
// FUNCTION: ROBLOX 0x0048b190
WedgeMesh::WedgeMesh(const G3D::Vector3& size, NormalId textureFace, const G3D::Vector2& studsPerTile)
{
	Render::Mesh::LevelRef level = new Render::Mesh::Level(G3D::RenderDevice::QUADS);

	levels.append(level);

	WedgeBuilder builder(level, size, RenderSurfaceTypes(), bevel);

	builder.textureScale = mapToUvw_Legacy(size, textureFace).xy() * 2.0 / studsPerTile;

	builder.buildFace(textureFace, LevelBuilder::Decal);
}

template WedgeMesh* MeshFactory<WedgeMesh, 1>::create(const G3D::Vector3&, RenderSurfaceTypes);

template WedgeMesh* MeshFactory<WedgeMesh, 1>::createDecal(const G3D::Vector3&, NormalId);

template WedgeMesh* MeshFactory<WedgeMesh, 1>::createTexture(const G3D::Vector3&, NormalId, const G3D::Vector2&);

} // namespace View
} // namespace RBX

// FUNCTION: WEBSERVICE 0x10181100
// FUNCTION: ROBLOX 0x0048b530
inline void WedgeBuilder::buildBack(Purpose purpose)
{
	buildFace<RBX::NORM_Z>(*this, 1, 1, purpose);
}

// FUNCTION: WEBSERVICE 0x10181130
// FUNCTION: ROBLOX 0x0048b560
inline void WedgeBuilder::buildBottom(Purpose purpose)
{
	buildFace<RBX::NORM_Y_NEG>(*this, 1, 1, purpose);
}
