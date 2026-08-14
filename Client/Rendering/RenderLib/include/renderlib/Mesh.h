#ifndef RENDERLIB_MESH_H
#define RENDERLIB_MESH_H

#include "decomp.h"

#include <G3D/Array.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/MeshAlg.h>
#include <G3D/ReferenceCount.h>
#include <G3D/Vector2.h>
#include <G3D/Vector3.h>
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

	class Level;

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
	static G3D::Vector3 findScale;
	static VertexList findGrid[32][32][32];

protected:
	G3D::Array<LevelRef> levels; // 0x0c
	ShadowSurface shadowSurface; // 0x18

public:
	void computeDirectionalShadowVolume(
		const G3D::CoordinateFrame& cframe,
		const G3D::Vector3& worldLight,
		G3D::Array<unsigned int>& indexArray,
		G3D::Array<G3D::Vector3>& vertexArray,
		bool caps
	) const;

	// clang-format off
	// SYNTHETIC: WEBSERVICE 0x102257c0
	// `RBX::Render::Mesh::computeDirectionalShadowVolume'::`2'::`dynamic atexit destructor for 'backface''
	// clang-format on

	static G3D::ReferenceCountedPointer<Mesh> aggregate(
		const std::vector<G3D::ReferenceCountedPointer<Chunk> >& chunkArray,
		G3D::CoordinateFrame& outCFrame,
		float& outBoundingRadius
	);

	float debugBoundingRadius; // 0x4c

	virtual ~Mesh();
};

DECOMP_SIZE_ASSERT(Mesh, 0x50)
DECOMP_SIZE_ASSERT(Mesh::ShadowSurface, 0x34)

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_MESH_H
