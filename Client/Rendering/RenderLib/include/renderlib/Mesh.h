#ifndef RENDERLIB_MESH_H
#define RENDERLIB_MESH_H

#include "placeholder.h"

#include <G3D/Array.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/MeshAlg.h>
#include <G3D/ReferenceCount.h>
#include <G3D/Vector2.h>
#include <G3D/Vector3.h>
#include <GLG3D/RenderDevice.h>
#include <GLG3D/VAR.h>
#include <vector>

class CRobloxWnd;
class ThumbnailGenerator;

namespace RBX {
namespace Render {

class Chunk;

class Mesh : public G3D::ReferenceCountedObject
{
public:
	typedef unsigned int IndexType;
	typedef G3D::Array<int> VertexList;

	class Level : public G3D::ReferenceCountedObject
	{
	public:
		G3D::Array<IndexType> indexArray;
		G3D::RenderDevice::Primitive primitive;

		Level();

		Level(G3D::RenderDevice::Primitive primitive) : primitive(primitive) {}

		virtual ~Level();
	};

	typedef G3D::ReferenceCountedPointer<Level> LevelRef;

	class ShadowSurface
	{
	public:
		G3D::Array<G3D::MeshAlg::Edge> edgeArray;
		G3D::Array<G3D::Vector3> faceNormalArray;
		G3D::Array<G3D::MeshAlg::Face> faceArray;
		G3D::Array<G3D::Vector3> geometry;
		LevelRef dropShadowGeometry;
	};
	static void setShadowVertex(const G3D::Vector3&);

private:
	friend class ::CRobloxWnd;
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

	G3D::Array<LevelRef> levels;

	int detailIndex(double detail) const { return G3D::iClamp((int) detail, 0, levels.size() - 1); }

	ShadowSurface shadowSurface;

public:
	static G3D::ReferenceCountedPointer<Mesh> aggregate(
		const std::vector<G3D::ReferenceCountedPointer<Chunk> >& chunkArray,
		G3D::CoordinateFrame& outCFrame,
		float& outBoundingRadius
	);
	static unsigned int findVertex(const G3D::Vector3&, const G3D::Vector3&, const G3D::Vector2&);

	virtual ~Mesh();

	float debugBoundingRadius;

	static void freeVertex(unsigned int index);

	static unsigned int allocVertex(unsigned int i, unsigned int count);

	static unsigned int allocVertex(
		const G3D::Vector3& vertex, const G3D::Vector3& normal, const G3D::Vector2& tex, bool share
	);

	static void beginRender(G3D::RenderDevice* rd, bool usetexCoords, bool useTangent);

	static void sendGeometry(const Level* lvl, G3D::RenderDevice* rd);

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

	static void flushVAR();
};

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_MESH_H
