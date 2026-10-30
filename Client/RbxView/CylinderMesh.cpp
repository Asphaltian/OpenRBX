#include "CylinderMesh.h"

#include "QuadVolume.h"

#include <G3D/Vector2int16.h>
#include <G3D/vectorMath.h>
#include <algorithm>
#include <math.h>

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
class CylinderBuilder : public RBX::View::LevelBuilder
{
public:
	const unsigned int elements; // 0x20

	// FUNCTION: WEBSERVICE 0x10187030
	// FUNCTION: ROBLOX 0x00491510
	CylinderBuilder(
		G3D::ReferenceCountedPointer<RBX::Render::Mesh::Level>& level,
		G3D::Vector3 size,
		RBX::View::RenderSurfaceTypes surfaceTypes,
		unsigned int slices
	)
		: RBX::View::LevelBuilder(level, size, surfaceTypes), elements(std::max(slices / 4, 1u))
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

// FUNCTION: WEBSERVICE 0x101870a0
// FUNCTION: ROBLOX 0x00528830
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

// FUNCTION: WEBSERVICE 0x10187190
// FUNCTION: ROBLOX 0x00491580
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

// FUNCTION: WEBSERVICE 0x10187230
// FUNCTION: ROBLOX 0x00491620
inline void CylinderBuilder::buildRight(Purpose purpose)
{
	buildFace<RBX::NORM_X>(EndcapTransform, elements, elements, purpose);
}

// FUNCTION: WEBSERVICE 0x10187260
// FUNCTION: ROBLOX 0x00491650
inline void CylinderBuilder::buildTop(Purpose purpose)
{
	buildFace<RBX::NORM_Y>(CylinderTransform, elements, 1, purpose);
}

// FUNCTION: WEBSERVICE 0x10187290
// FUNCTION: ROBLOX 0x00491680
inline void CylinderBuilder::buildBack(Purpose purpose)
{
	buildFace<RBX::NORM_Z>(CylinderTransform, elements, 1, purpose);
}

// FUNCTION: WEBSERVICE 0x101872c0
// FUNCTION: ROBLOX 0x004916b0
inline void CylinderBuilder::buildLeft(Purpose purpose)
{
	buildFace<RBX::NORM_X_NEG>(EndcapTransform, elements, elements, purpose);
}

// FUNCTION: WEBSERVICE 0x101872f0
// FUNCTION: ROBLOX 0x004916e0
inline void CylinderBuilder::buildBottom(Purpose purpose)
{
	buildFace<RBX::NORM_Y_NEG>(CylinderTransform, elements, 1, purpose);
}

// FUNCTION: WEBSERVICE 0x10187320
// FUNCTION: ROBLOX 0x00491710
inline void CylinderBuilder::buildFront(Purpose purpose)
{
	buildFace<RBX::NORM_Z_NEG>(CylinderTransform, elements, 1, purpose);
}

namespace RBX {
namespace View {

// FUNCTION: WEBSERVICE 0x10187350
// FUNCTION: ROBLOX 0x00491740
CylinderAlongXMesh::CylinderAlongXMesh(const G3D::Vector3& size, RenderSurfaceTypes surfaceTypes)
{
	levels.append(new Render::Mesh::Level(G3D::RenderDevice::QUADS));
	{
		Render::Mesh::LevelRef& level = levels.last();
		CylinderBuilder builder(level, size, surfaceTypes, 12);
		builder.build(LevelBuilder::Surface);
	}
}

// FUNCTION: WEBSERVICE 0x101874a0
// FUNCTION: ROBLOX 0x00491890
CylinderAlongXMesh::CylinderAlongXMesh(const G3D::Vector3& size, NormalId decalFace)
{
	Render::Mesh::LevelRef level = new Render::Mesh::Level(G3D::RenderDevice::QUADS);

	levels.append(level);

	CylinderBuilder builder(level, size, RenderSurfaceTypes(), 12);

	builder.buildFace(decalFace, LevelBuilder::Decal);
}

// FUNCTION: WEBSERVICE 0x10187600
// FUNCTION: ROBLOX 0x004919f0
CylinderAlongXMesh::CylinderAlongXMesh(const G3D::Vector3& size, NormalId textureFace, const G3D::Vector2& studsPerTile)
{
	Render::Mesh::LevelRef level = new Render::Mesh::Level(G3D::RenderDevice::QUADS);

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

} // namespace View
} // namespace RBX
