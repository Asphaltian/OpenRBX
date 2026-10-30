#include "TorsoMesh.h"

#include "BevelMesh.h"
#include "QuadVolume.h"

#include <G3D/Vector2int16.h>

const float bevel = 0.065f;

// SIZE 0x8
class DeformTop
{
private:
	const float inset;         // 0x00
	const float shoulderInset; // 0x04

public:
	DeformTop(float inset, float shoulderInset) : inset(inset), shoulderInset(shoulderInset) {}

	void operator()(
		G3D::Vector3& vertex,
		G3D::Vector3& normal,
		G3D::Vector2& tex,
		const G3D::Vector3& radius,
		const G3D::Vector2int16& ivertex
	)
	{
		if (vertex.x > 0) {
			vertex.x -= shoulderInset + inset;
			tex.x -= shoulderInset;
		}
		else {
			vertex.x += shoulderInset + inset;
			tex.x += shoulderInset;
		}
		if (vertex.y > 0) {
			vertex.y -= inset;
		}
		else {
			vertex.y += inset;
		}
	}
};

DECOMP_SIZE_ASSERT(DeformTop, 0x8)

// SIZE 0x8
class DeformSide
{
private:
	const float inset;         // 0x00
	const float shoulderInset; // 0x04

public:
	DeformSide(float inset, float shoulderInset) : inset(inset), shoulderInset(shoulderInset) {}

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
			vertex.y -= inset;
			vertex.z -= shoulderInset;
		}
		else {
			vertex.y += inset;
		}
	}
};

DECOMP_SIZE_ASSERT(DeformSide, 0x8)

// SIZE 0x8
class DeformFrontBack
{
private:
	const float inset;         // 0x00
	const float shoulderInset; // 0x04

public:
	DeformFrontBack(float inset, float shoulderInset) : inset(inset), shoulderInset(shoulderInset) {}

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
			vertex.y -= inset;
			if (vertex.x > 0) {
				vertex.x -= shoulderInset;
				tex.x -= shoulderInset;
			}
			else {
				vertex.x += shoulderInset;
				tex.x += shoulderInset;
			}
		}
		else {
			vertex.y += inset;
		}
	}
};

DECOMP_SIZE_ASSERT(DeformFrontBack, 0x8)

// SIZE 0x28
class TorsoBuilder : public RBX::View::BevelMesh::Builder
{
public:
	const float shoulderInset; // 0x24

	TorsoBuilder(
		G3D::ReferenceCountedPointer<RBX::Render::Mesh::Level>& level,
		G3D::Vector3 size,
		RBX::View::RenderSurfaceTypes surfaceTypes,
		float bevel,
		float shoulderInset
	)
		: RBX::View::BevelMesh::Builder(level, size, surfaceTypes, bevel), shoulderInset(shoulderInset)
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

DECOMP_SIZE_ASSERT(TorsoBuilder, 0x28)

// FUNCTION: WEBSERVICE 0x10183500
// FUNCTION: ROBLOX 0x0048d970
inline void TorsoBuilder::buildRight(Purpose purpose)
{
	buildFace<RBX::NORM_X>(DeformSide(bevel, shoulderInset), 1, 1, purpose);
}

// FUNCTION: WEBSERVICE 0x10183540
// FUNCTION: ROBLOX 0x0048d9b0
inline void TorsoBuilder::buildTop(Purpose purpose)
{
	buildFace<RBX::NORM_Y>(DeformTop(bevel, shoulderInset), 1, 1, purpose);
}

// FUNCTION: WEBSERVICE 0x10183580
// FUNCTION: ROBLOX 0x0048d9f0
inline void TorsoBuilder::buildBack(Purpose purpose)
{
	buildFace<RBX::NORM_Z>(DeformFrontBack(bevel, shoulderInset), 1, 1, purpose);
}

// FUNCTION: WEBSERVICE 0x101835c0
// FUNCTION: ROBLOX 0x0048da30
inline void TorsoBuilder::buildLeft(Purpose purpose)
{
	buildFace<RBX::NORM_X_NEG>(DeformSide(bevel, shoulderInset), 1, 1, purpose);
}

// FUNCTION: WEBSERVICE 0x10183600
// FUNCTION: ROBLOX 0x0048da70
inline void TorsoBuilder::buildFront(Purpose purpose)
{
	buildFace<RBX::NORM_Z_NEG>(DeformFrontBack(bevel, shoulderInset), 1, 1, purpose);
}

namespace RBX {
namespace View {

// FUNCTION: WEBSERVICE 0x10183a90
// FUNCTION: ROBLOX 0x0048df00
TorsoMesh::TorsoMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes)
{
	const float shoulderInset = size.z * 0.3f;

	levels.append(new Render::Mesh::Level(G3D::RenderDevice::QUADS));
	{
		TorsoBuilder builder(levels.last(), size, surfaceTypes, 0, shoulderInset);
		builder.build(LevelBuilder::Surface);
	}
}

// FUNCTION: WEBSERVICE 0x10183c20
// FUNCTION: ROBLOX 0x0048e090
TorsoMesh::TorsoMesh(const G3D::Vector3& size, NormalId decalFace)
{
	const float shoulderInset = size.z * 0.3f;

	Render::Mesh::LevelRef level = new Render::Mesh::Level(G3D::RenderDevice::QUADS);

	levels.append(level);

	TorsoBuilder builder(level, size, RenderSurfaceTypes(), bevel, shoulderInset);

	builder.buildFace(decalFace, LevelBuilder::Decal);
}

// FUNCTION: WEBSERVICE 0x10183db0
// FUNCTION: ROBLOX 0x0048e220
TorsoMesh::TorsoMesh(const G3D::Vector3& size, NormalId textureFace, const G3D::Vector2& studsPerTile)
{
	const float shoulderInset = size.z * 0.3f;

	Render::Mesh::LevelRef level = new Render::Mesh::Level(G3D::RenderDevice::QUADS);

	levels.append(level);

	TorsoBuilder builder(level, size, RenderSurfaceTypes(), bevel, shoulderInset);

	builder.textureScale = mapToUvw_Legacy(size, textureFace).xy() * 2.0 / studsPerTile;

	builder.buildFace(textureFace, LevelBuilder::Decal);
}

template TorsoMesh* MeshFactory<TorsoMesh, 1>::create(const G3D::Vector3&, RenderSurfaceTypes);

template TorsoMesh* MeshFactory<TorsoMesh, 1>::createDecal(const G3D::Vector3&, NormalId);

template TorsoMesh* MeshFactory<TorsoMesh, 1>::createTexture(const G3D::Vector3&, NormalId, const G3D::Vector2&);

} // namespace View
} // namespace RBX

// FUNCTION: WEBSERVICE 0x101840a0
// FUNCTION: ROBLOX 0x0048e510
inline void TorsoBuilder::buildBottom(Purpose purpose)
{
	buildFace<RBX::NORM_Y_NEG>(*this, 1, 1, purpose);
}
