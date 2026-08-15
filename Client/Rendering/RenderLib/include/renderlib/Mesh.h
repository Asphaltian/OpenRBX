#ifndef RENDERLIB_MESH_H
#define RENDERLIB_MESH_H

#include "decomp.h"

#include <G3D/Array.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/MeshAlg.h>
#include <G3D/ReferenceCount.h>
#include <G3D/Vector2.h>
#include <G3D/Vector3.h>
#include <GLG3D/RenderDevice.h>
#include <GLG3D/VAR.h>
#include <vector>

namespace RBX {
namespace Render {

class Chunk;

// SIZE 0x50
class Mesh : public G3D::ReferenceCountedObject
{
public:
	typedef unsigned int IndexType;
	typedef G3D::Array<int> VertexList;

	// SIZE 0x1c
	class Level : public G3D::ReferenceCountedObject
	{
	public:
		G3D::Array<IndexType> indexArray;       // 0x0c
		G3D::RenderDevice::Primitive primitive; // 0x18

		Level();
		Level(G3D::RenderDevice::Primitive primitive);

		virtual ~Level();
	};

	typedef G3D::ReferenceCountedPointer<Level> LevelRef;

	// SIZE 0x34
	class ShadowSurface
	{
	public:
		G3D::Array<G3D::MeshAlg::Edge> edgeArray; // 0x00
		G3D::Array<G3D::Vector3> faceNormalArray; // 0x0c
		G3D::Array<G3D::MeshAlg::Face> faceArray; // 0x18
		G3D::Array<G3D::Vector3> geometry;        // 0x24
		LevelRef dropShadowGeometry;              // 0x30
	};

private:
	static G3D::MeshAlg::Geometry visibleGeometry;
	static G3D::Array<G3D::Vector2> texCoordArray;
	static G3D::Array<G3D::Vector3> tangentArray;
	static VertexList vertexRefCounts;
	static VertexList freeList;
	static G3D::VAR vertexVAR;
	static G3D::VAR normalVAR;
	static G3D::VAR texCoordVAR;
	static G3D::VAR tangentVAR;
	static bool varDirty;

	static void makeVAR();

	static G3D::Vector3 findScale;
	static VertexList findGrid[32][32][32];

protected:
	static void initStatics();

	Mesh();

	void computeShadowSurface(const LevelRef& level);

	G3D::Array<LevelRef> levels; // 0x0c

	int detailIndex(double detail) const { return G3D::iClamp((int) detail, 0, levels.size() - 1); }

	ShadowSurface shadowSurface; // 0x18

public:
	static G3D::ReferenceCountedPointer<Mesh> aggregate(
		const std::vector<G3D::ReferenceCountedPointer<Chunk> >& chunkArray,
		G3D::CoordinateFrame& outCFrame,
		float& outBoundingRadius
	);

	virtual ~Mesh();

	float debugBoundingRadius; // 0x4c

	static void freeVertex(unsigned int i);

	static unsigned int allocVertex(unsigned int i, unsigned int count);

	static unsigned int allocVertex(
		const G3D::Vector3& vertex,
		const G3D::Vector3& normal,
		const G3D::Vector2& tex,
		bool share
	);

	static void beginRender(G3D::RenderDevice* rd, bool usetexCoords, bool useTangent);

	static void sendGeometry(const Level* lvl, G3D::RenderDevice* rd);

	// FUNCTION: WEBSERVICE 0x101eff00
	LevelRef dropShadowMesh() { return shadowSurface.dropShadowGeometry; }

	static void endRender(G3D::RenderDevice* rd);

	const LevelRef detailLevel(float detail) const;

	void computeDirectionalShadowVolume(
		const G3D::CoordinateFrame& cframe,
		const G3D::Vector3& worldLight,
		G3D::Array<unsigned int>& indexArray,
		G3D::Array<G3D::Vector3>& vertexArray,
		bool caps
	) const;

	// clang-format off
	// SYNTHETIC: WEBSERVICE 0x10165ee0
	// RBX::Render::Mesh::ShadowSurface::~ShadowSurface
	// SYNTHETIC: WEBSERVICE 0x10167660
	// RBX::Render::Mesh::ShadowSurface::ShadowSurface
	// SYNTHETIC: WEBSERVICE 0x102257c0
	// `RBX::Render::Mesh::computeDirectionalShadowVolume'::`2'::`dynamic atexit destructor for 'backface''
	// clang-format on
};

DECOMP_SIZE_ASSERT(Mesh, 0x50)
DECOMP_SIZE_ASSERT(Mesh::Level, 0x1c)
DECOMP_SIZE_ASSERT(Mesh::ShadowSurface, 0x34)

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_MESH_H
