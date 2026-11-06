#ifndef RENDERLIBNEW_MESH_H
#define RENDERLIBNEW_MESH_H

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

namespace Ogre {
class SubMesh;
}

namespace RBX {
namespace RenderNew {

class Chunk;

// SIZE 0x40
class Mesh : public G3D::ReferenceCountedObject
{
public:
	typedef unsigned int IndexType;
	typedef G3D::Array<int> VertexList;

	// SIZE 0x28
	class Level : public G3D::ReferenceCountedObject
	{
	public:
		G3D::Array<IndexType> indexArray;       // 0x0c
		G3D::RenderDevice::Primitive primitive; // 0x18
		bool m_unk0x1c;                         // 0x1c
		int m_unk0x20;                          // 0x20
		Ogre::SubMesh* m_unk0x24;               // 0x24

		Level();

		// FUNCTION: ROBLOX 0x0050e300
		Level(G3D::RenderDevice::Primitive primitive)
			: primitive(primitive), m_unk0x1c(true), m_unk0x20(-1), m_unk0x24(NULL)
		{
		}

		virtual ~Level();

		// SYNTHETIC: ROBLOX 0x0050e360
		// RBX::RenderNew::Mesh::Level::`scalar deleting destructor'
	};

	typedef G3D::ReferenceCountedPointer<Level> LevelRef;

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

	G3D::Array<LevelRef> levels; // 0x0c

	int detailIndex(double detail) const { return G3D::iClamp((int) detail, 0, levels.size() - 1); }

	std::vector<undefined4> m_unk0x18; // 0x18
	std::vector<undefined4> m_unk0x28; // 0x28
	undefined4* m_unk0x38;             // 0x38
	undefined4 m_unk0x3c;              // 0x3c

public:
	static G3D::ReferenceCountedPointer<Mesh> aggregate(
		const std::vector<G3D::ReferenceCountedPointer<Chunk> >& chunkArray,
		G3D::CoordinateFrame& outCFrame,
		float& outBoundingRadius
	);
	static unsigned int findVertex(const G3D::Vector3&, const G3D::Vector3&, const G3D::Vector2&);

	virtual ~Mesh();

	static void freeVertex(unsigned int index);

	static unsigned int allocVertex(unsigned int i, unsigned int count);

	static unsigned int allocVertex(
		const G3D::Vector3& vertex, const G3D::Vector3& normal, const G3D::Vector2& tex, bool share
	);

	static void beginRender(G3D::RenderDevice* rd, bool usetexCoords, bool useTangent);

	static void sendGeometry(const Level* lvl, G3D::RenderDevice* rd);

	static void endRender(G3D::RenderDevice* rd);

	const LevelRef detailLevel(float detail) const;

	static void flushVAR();
};

DECOMP_SIZE_ASSERT(Mesh, 0x40)
DECOMP_SIZE_ASSERT(Mesh::Level, 0x28)

} // namespace RenderNew
} // namespace RBX

#endif // RENDERLIBNEW_MESH_H
