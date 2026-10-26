#include "HeadMesh.h"

#include "QuadVolume.h"

#include <G3D/Vector2int16.h>
#include <algorithm>
#include <math.h>

// SIZE 0x8
class CylinderTransform
{
private:
	const float bevel;         // 0x00
	const int halfVertexCount; // 0x04

public:
	CylinderTransform(float bevel, int vertexCount) : bevel(bevel), halfVertexCount((vertexCount + 1) / 2) {}

	void operator()(
		G3D::Vector3& vertex,
		G3D::Vector3& normal,
		G3D::Vector2& tex,
		const G3D::Vector3& radius,
		const G3D::Vector2int16& ivertex
	);
};

DECOMP_SIZE_ASSERT(CylinderTransform, 0x8)

// SIZE 0x4
class EndcapTransform
{
private:
	const float bevel; // 0x00

public:
	EndcapTransform(float bevel) : bevel(bevel) {}

	void operator()(
		G3D::Vector3& vertex,
		G3D::Vector3& normal,
		G3D::Vector2& tex,
		const G3D::Vector3& radius,
		const G3D::Vector2int16& ivertex
	);
};

DECOMP_SIZE_ASSERT(EndcapTransform, 0x4)

// SIZE 0x28
class HeadBuilder : public RBX::View::LevelBuilder
{
public:
	const float bevel;           // 0x20
	const unsigned int elements; // 0x24

	// FUNCTION: WEBSERVICE 0x101877c0
	HeadBuilder(
		G3D::ReferenceCountedPointer<RBX::Render::Mesh::Level> level,
		G3D::Vector3 size,
		RBX::View::RenderSurfaceTypes surfaceTypes,
		unsigned int elements,
		float bevel
	)
		: RBX::View::LevelBuilder(
			  level,
			  G3D::Vector3(std::min(size.x, size.z), size.y, std::min(size.x, size.z)),
			  surfaceTypes
		  ),
		  bevel(bevel), elements(elements)
	{
	}

protected:
	virtual void buildTop(Purpose purpose);    // vtable+0x04
	virtual void buildBottom(Purpose purpose); // vtable+0x08
	virtual void buildLeft(Purpose purpose);   // vtable+0x0c
	virtual void buildRight(Purpose purpose);  // vtable+0x10
	virtual void buildFront(Purpose purpose);  // vtable+0x14
	virtual void buildBack(Purpose purpose);   // vtable+0x18
};

DECOMP_SIZE_ASSERT(HeadBuilder, 0x28)

// FUNCTION: WEBSERVICE 0x101878b0
inline void CylinderTransform::operator()(
	G3D::Vector3& vertex,
	G3D::Vector3& normal,
	G3D::Vector2& tex,
	const G3D::Vector3& radius,
	const G3D::Vector2int16& ivertex
)
{
	const float yaw = vertex.x * 0.785398185f / radius.x;
	const float pitch = (ivertex.y - halfVertexCount + 1) * 1.57079637f / (halfVertexCount - 1);

	vertex.x = (radius.x - bevel * (1.0 - cos(pitch))) * sin(yaw);
	if (ivertex.y >= halfVertexCount) {
		vertex.y = radius.y - (1.0 - sin(pitch)) * bevel;
	}
	else {
		vertex.y = (sin(pitch) + 1.0) * bevel - radius.y;
	}
	vertex.z = (radius.z - bevel * (1.0 - cos(pitch))) * cos(yaw);

	normal.x = cos(pitch) * sin(yaw);
	normal.y = sin(pitch);
	normal.z = cos(pitch) * cos(yaw);

	const float bevelLength = bevel * 1.57079637f;
	const float squish = bevelLength / (radius.y + bevelLength - bevel);

	tex.y = ivertex.y >= halfVertexCount
				? tex.y * squish
				: 2 * radius.y - (1.0 - (float) ((tex.y - radius.y) / radius.y)) * squish * radius.y;
}

// FUNCTION: WEBSERVICE 0x10187a30
inline void EndcapTransform::operator()(
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
			ratio *= 1.0 - bevel / radius.y;
		}
	}
	else {
		if (vertex.x == 0) {
			ratio = 0;
		}
		else {
			ratio = fabs(vertex.x) / vertex.xy().length();
			ratio *= 1.0 - bevel / radius.x;
		}
	}
	vertex.x *= ratio;
	vertex.y *= ratio;
}

// FUNCTION: WEBSERVICE 0x1018a200
inline void HeadBuilder::buildTop(Purpose purpose)
{
	buildFace<RBX::NORM_Y>(EndcapTransform(bevel), elements, elements, purpose);
}

// FUNCTION: WEBSERVICE 0x1018a230
inline void HeadBuilder::buildBottom(Purpose purpose)
{
	buildFace<RBX::NORM_Y_NEG>(EndcapTransform(bevel), elements, elements, purpose);
}

// FUNCTION: WEBSERVICE 0x1018a260
inline void HeadBuilder::buildLeft(Purpose purpose)
{
	buildFace<RBX::NORM_X_NEG>(CylinderTransform(bevel, elements), elements, elements, purpose);
}

// FUNCTION: WEBSERVICE 0x1018a2a0
inline void HeadBuilder::buildRight(Purpose purpose)
{
	buildFace<RBX::NORM_X>(CylinderTransform(bevel, elements), elements, elements, purpose);
}

// FUNCTION: WEBSERVICE 0x1018a2e0
inline void HeadBuilder::buildFront(Purpose purpose)
{
	buildFace<RBX::NORM_Z_NEG>(CylinderTransform(bevel, elements), elements, elements, purpose);
}

// FUNCTION: WEBSERVICE 0x1018a320
inline void HeadBuilder::buildBack(Purpose purpose)
{
	buildFace<RBX::NORM_Z>(CylinderTransform(bevel, elements), elements, elements, purpose);
}

namespace RBX {
namespace View {

// FUNCTION: WEBSERVICE 0x1018a360
HeadMesh::HeadMesh(const G3D::Vector3& partSize, RenderSurfaceTypes surfaceTypes)
{
	const float bevel = std::min(partSize.x, partSize.z) * 0.25f;

	levels.append(new Render::Mesh::Level(G3D::RenderDevice::QUADS));
	{
		Render::Mesh::LevelRef level = levels.last();
		HeadBuilder builder(level, partSize, surfaceTypes, 9, bevel);
		builder.build(LevelBuilder::Surface);
	}
}

// FUNCTION: WEBSERVICE 0x1018a550
HeadMesh::HeadMesh(const G3D::Vector3& size, NormalId decalFace)
{
	const float bevel = std::min(size.x, size.z) * 0.25f;

	Render::Mesh::Level* level = new Render::Mesh::Level(G3D::RenderDevice::QUADS);

	levels.append(level);

	HeadBuilder builder(level, size, RenderSurfaceTypes(), 9, bevel);

	builder.buildFace(decalFace, LevelBuilder::Decal);
}

// FUNCTION: WEBSERVICE 0x1018a6e0
HeadMesh::HeadMesh(const G3D::Vector3& size, NormalId textureFace, const G3D::Vector2& studsPerTile)
{
	const float bevel = std::min(size.x, size.z) * 0.25f;

	Render::Mesh::Level* level = new Render::Mesh::Level(G3D::RenderDevice::QUADS);

	levels.append(level);

	HeadBuilder builder(level, size, RenderSurfaceTypes(), 9, bevel);

	builder.textureScale = mapToUvw_Legacy(size, textureFace).xy() * 2.0 / studsPerTile;

	builder.buildFace(textureFace, LevelBuilder::Decal);
}

} // namespace View
} // namespace RBX
