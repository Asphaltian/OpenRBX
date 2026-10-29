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

class ThumbnailGenerator;

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

		// STUB: WEBSERVICE 0x10167b50
		// STUB: ROBLOX 0x00484130
		Level(G3D::RenderDevice::Primitive primitive) : primitive(primitive) {}

		virtual ~Level();

		// clang-format off
		// SYNTHETIC: WEBSERVICE 0x101675e0
		// RBX::Render::Mesh::Level::`scalar deleting destructor'
		// clang-format on
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
	static void setShadowVertex(const G3D::Vector3&);

private:
	friend class ::ThumbnailGenerator;

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

	static void computeAABounds(
		const std::vector<G3D::ReferenceCountedPointer<Chunk> >& chunkArray, G3D::Vector3& lo, G3D::Vector3& hi
	);

	static G3D::Vector3 findScale;
	static VertexList findGrid[32][32][32];
	static G3D::Array<int>& getFindGridCell(const G3D::Vector3&);
	static unsigned int findVertex(
		const G3D::Array<int>&, const G3D::Vector3&, const G3D::Vector3&, const G3D::Vector2&
	);
	static void toFindGridCoords(const G3D::Vector3&, int&, int&, int&);

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
	static unsigned int findVertex(const G3D::Vector3&, const G3D::Vector3&, const G3D::Vector2&);

	virtual ~Mesh();

	float debugBoundingRadius; // 0x4c

	static void freeVertex(unsigned int index);

	static unsigned int allocVertex(unsigned int i, unsigned int count);

	static unsigned int allocVertex(
		const G3D::Vector3& vertex, const G3D::Vector3& normal, const G3D::Vector2& tex, bool share
	);

	static void beginRender(G3D::RenderDevice* rd, bool usetexCoords, bool useTangent);

	static void sendGeometry(const Level* lvl, G3D::RenderDevice* rd);

	// FUNCTION: WEBSERVICE 0x101eff00
	// FUNCTION: ROBLOX 0x0049e040
	LevelRef dropShadowMesh() { return shadowSurface.dropShadowGeometry; }

	static void endRender(G3D::RenderDevice* rd);

	const LevelRef detailLevel(float detail) const;

	void computeDirectionalShadowVolume(
		const G3D::CoordinateFrame& cframe,
		const G3D::Vector3& wsLightVector,
		G3D::Array<unsigned int>& indexArray,
		G3D::Array<G3D::Vector3>& shadowVertex,
		bool generateLightCap
	) const;

	// clang-format off
	// SYNTHETIC: WEBSERVICE 0x10165ee0
	// SYNTHETIC: ROBLOX 0x00499c30
	// RBX::Render::Mesh::ShadowSurface::~ShadowSurface
	// SYNTHETIC: WEBSERVICE 0x10167660
	// SYNTHETIC: ROBLOX 0x0049b080
	// RBX::Render::Mesh::ShadowSurface::ShadowSurface
	// SYNTHETIC: WEBSERVICE 0x1021eb80
	// `dynamic initializer for 'RBX::Render::Mesh::visibleGeometry''
	// SYNTHETIC: WEBSERVICE 0x1021ebc0
	// `dynamic initializer for 'RBX::Render::Mesh::texCoordArray''
	// SYNTHETIC: WEBSERVICE 0x1021ebd0
	// `dynamic initializer for 'RBX::Render::Mesh::tangentArray''
	// SYNTHETIC: WEBSERVICE 0x1021ebe0
	// `dynamic initializer for 'RBX::Render::Mesh::vertexVAR''
	// SYNTHETIC: WEBSERVICE 0x1021ec00
	// `dynamic initializer for 'RBX::Render::Mesh::normalVAR''
	// SYNTHETIC: WEBSERVICE 0x1021ec20
	// `dynamic initializer for 'RBX::Render::Mesh::texCoordVAR''
	// SYNTHETIC: WEBSERVICE 0x1021ec40
	// `dynamic initializer for 'RBX::Render::Mesh::tangentVAR''
	// SYNTHETIC: WEBSERVICE 0x1021ec60
	// `dynamic initializer for 'RBX::Render::Mesh::vertexRefCounts''
	// SYNTHETIC: WEBSERVICE 0x1021ec70
	// `dynamic initializer for 'RBX::Render::Mesh::freeList''
	// SYNTHETIC: WEBSERVICE 0x10225740
	// `dynamic atexit destructor for 'RBX::Render::Mesh::texCoordArray''
	// SYNTHETIC: WEBSERVICE 0x10225760
	// `dynamic atexit destructor for 'RBX::Render::Mesh::tangentArray''
	// SYNTHETIC: WEBSERVICE 0x10225780
	// `dynamic atexit destructor for 'RBX::Render::Mesh::vertexRefCounts''
	// SYNTHETIC: WEBSERVICE 0x102257a0
	// `dynamic atexit destructor for 'RBX::Render::Mesh::freeList''
	// SYNTHETIC: WEBSERVICE 0x102257c0
	// SYNTHETIC: ROBLOX 0x007f05f0
	// `RBX::Render::Mesh::computeDirectionalShadowVolume'::`2'::`dynamic atexit destructor for 'backface''
	// SYNTHETIC: WEBSERVICE 0x102257e0
	// `dynamic atexit destructor for 'RBX::Render::Mesh::visibleGeometry''
	// SYNTHETIC: WEBSERVICE 0x102257f0
	// `dynamic atexit destructor for 'RBX::Render::Mesh::normalVAR''
	// SYNTHETIC: WEBSERVICE 0x10225830
	// `dynamic atexit destructor for 'RBX::Render::Mesh::texCoordVAR''
	// SYNTHETIC: WEBSERVICE 0x10225870
	// `dynamic atexit destructor for 'RBX::Render::Mesh::tangentVAR''
	// SYNTHETIC: WEBSERVICE 0x102258b0
	// `dynamic atexit destructor for 'RBX::Render::Mesh::vertexVAR''
	// clang-format on
	static void flushVAR();
};

DECOMP_SIZE_ASSERT(Mesh, 0x50)
DECOMP_SIZE_ASSERT(Mesh::Level, 0x1c)
DECOMP_SIZE_ASSERT(Mesh::ShadowSurface, 0x34)

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_MESH_H
