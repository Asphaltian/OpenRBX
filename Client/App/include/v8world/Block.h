#ifndef V8WORLD_BLOCK_H
#define V8WORLD_BLOCK_H

#include "decomp.h"
#include "v8kernel/Pair.h"
#include "v8world/Geometry.h"

#include <G3D/Vector2.h>
#include <G3D/Vector3int16.h>

namespace RBX {

using G3D::Vector2;
using G3D::Vector3int16;

// VTABLE: WEBSERVICE 0x10237654
// SIZE 0x18
class Block : public Geometry
{
private:
	const Vector3* getCornerPoint(const Vector3int16& clip) const
	{
		return &vertices[((clip.x <= 0) * 2 + (clip.y <= 0)) * 2 + (clip.z <= 0)];
	}

	const Vector3* getEdgePoint(const Vector3int16& clip, NormalId& normalID) const;
	const Vector3* getPlanePoint(const Vector3int16& clip, NormalId& normalID) const;

	Matrix3 getMomentHollow(float mass) const;

	virtual void onSetSize();

public:
	// SYNTHETIC: WEBSERVICE 0x100a76b0 FOLDED
	// RBX::Block::`scalar deleting destructor'

	virtual bool hitTest(const Ray& rayInMe, Vector3& localHitPoint, bool& inside);

	// FUNCTION: WEBSERVICE 0x100a7740 FOLDED
	virtual GeometryType getGeometryType() const { return GEOMETRY_BLOCK; }

	// FUNCTION: WEBSERVICE 0x100a7750
	virtual float getRadius() const { return cornerRadius; }

	virtual Vector3 getCenterToCorner(const Matrix3& rotation) const;

	// FUNCTION: WEBSERVICE 0x100a7760
	virtual Matrix3 getMoment(float mass) const { return getMomentHollow(mass); }

	virtual float getGridVolume() const;

	void projectToFace(Vector3& ray, Vector3int16& clip, int& onBorder);

	GeoPairType getBallInsideInfo(const Vector3& ray, const Vector3*& offset, NormalId& normalID);
	GeoPairType getBallBlockInfo(int onBorder, Vector3int16 clip, const Vector3*& offset, NormalId& normalID);

	const Vector3& getExtent() const { return *vertices; }

	const Vector3* getFaceVertex(NormalId faceId, int vertexId) const;

	int getClosestEdge(const Matrix3& rotation, NormalId normalID, Vector3& crossAxis);

	int faceVertexToEdge(NormalId faceId, int vertexId);

	const Vector3* getEdgeVertex(int edgeId) const;
	NormalId getEdgeNormal(int edgeId);

	Vector2 getProjectedVertex(const Vector3& vertex, NormalId normalID);

	Block() : vertices(NULL), cornerRadius(0) {}

private:
	const Vector3* vertices; // 0x10
	float cornerRadius;      // 0x14
};

DECOMP_SIZE_ASSERT(Block, 0x18)

// clang-format off
static const int BLOCK_FACE_TO_VERTEX[6][4] = {
	{0, 2, 3, 1},
	{0, 1, 5, 4},
	{0, 4, 6, 2},
	{4, 5, 7, 6},
	{2, 6, 7, 3},
	{1, 3, 7, 5},
};

static const int BLOCK_FACE_VERTEX_TO_EDGE[6][4] = {
	{4, 11, 5, 8},
	{8, 3, 9, 0},
	{0, 7, 1, 4},
	{9, 6, 10, 7},
	{1, 10, 2, 11},
	{5, 2, 6, 3},
};
// clang-format on

inline const Vector3* Block::getFaceVertex(NormalId faceId, int vertexId) const
{
	return &vertices[BLOCK_FACE_TO_VERTEX[faceId][vertexId]];
}

// FUNCTION: WEBSERVICE 0x100d1f50
inline const Vector3* Block::getEdgeVertex(int edgeId) const
{
	if (edgeId < 12) {
		return getFaceVertex((NormalId) (edgeId / 4), edgeId % 4);
	}

	int otherEdgeId = edgeId - 12;

	return getFaceVertex((NormalId) (otherEdgeId / 4), otherEdgeId + 1);
}

inline NormalId Block::getEdgeNormal(int edgeId)
{
	int normalId = (edgeId % 2) * 3 + edgeId / 4;

	if (edgeId > 12) {
		normalId = (normalId + 3) % 6;
	}

	return (NormalId) normalId;
}

inline int Block::faceVertexToEdge(NormalId faceId, int vertexId)
{
	return BLOCK_FACE_VERTEX_TO_EDGE[faceId][vertexId];
}

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021e3e0
// `dynamic initializer for 'RBX::BlockTemplate::blockTemplates''
// SYNTHETIC: WEBSERVICE 0x10225140
// `dynamic atexit destructor for 'RBX::BlockTemplate::blockTemplates''
// clang-format on

} // namespace RBX

#endif // V8WORLD_BLOCK_H
