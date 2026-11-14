#include "WedgeMesh.h"

#include "BevelMesh.h"
#include "QuadVolume.h"

#include <G3D/Vector2int16.h>
#include <math.h>

const float bevel = 0.065f;

class DeformSide3
{
private:
	const float inset;
	const float flip;

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

class DeformTop3
{
private:
	const float inset;

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
		if (vertex.y > 0) {
			vertex.y -= inset;
		}
		else {
			vertex.y = -vertex.y - inset;
		}
	}
};

class DeformFront
{
private:
	const float inset;

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
		normal.y = radius.z;
		normal.z = radius.y;
	}
};

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
	virtual void buildRight(Purpose purpose);
	virtual void buildTop(Purpose purpose);
	virtual void buildBack(Purpose purpose);
	virtual void buildLeft(Purpose purpose);
	virtual void buildBottom(Purpose purpose);
	virtual void buildFront(Purpose purpose);
};

inline void WedgeBuilder::buildRight(Purpose purpose)
{
	buildFace<RBX::NORM_X>(DeformSide3(bevel, true), 1, 1, purpose);
}

inline void WedgeBuilder::buildTop(Purpose purpose)
{
	buildFace<RBX::NORM_Y>(DeformTop3(bevel), 1, 1, purpose);
}

inline void WedgeBuilder::buildLeft(Purpose purpose)
{
	buildFace<RBX::NORM_X_NEG>(DeformSide3(bevel, false), 1, 1, purpose);
}

inline void WedgeBuilder::buildFront(Purpose purpose)
{
	buildFace<RBX::NORM_Z_NEG>(DeformFront(bevel), 1, 1, purpose);
}

namespace RBX {
namespace View {

WedgeMesh::WedgeMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes)
{
	levels.append(new Render::Mesh::Level(G3D::RenderDevice::QUADS));
	{
		WedgeBuilder builder(levels.last(), size, surfaceTypes, 0);
		builder.build(LevelBuilder::Surface);
	}
}

WedgeMesh::WedgeMesh(const G3D::Vector3& size, NormalId decalFace)
{
	Render::Mesh::LevelRef level = new Render::Mesh::Level(G3D::RenderDevice::QUADS);

	levels.append(level);

	WedgeBuilder builder(level, size, RenderSurfaceTypes(), bevel);

	builder.buildFace(decalFace, LevelBuilder::Decal);
}

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

inline void WedgeBuilder::buildBack(Purpose purpose)
{
	buildFace<RBX::NORM_Z>(*this, 1, 1, purpose);
}

inline void WedgeBuilder::buildBottom(Purpose purpose)
{
	buildFace<RBX::NORM_Y_NEG>(*this, 1, 1, purpose);
}
