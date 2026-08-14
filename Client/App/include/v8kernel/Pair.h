#ifndef V8KERNEL_PAIR_H
#define V8KERNEL_PAIR_H

#include "decomp.h"
#include "util/NormalId.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/Vector3.h>

namespace RBX {

class Body;

using G3D::CoordinateFrame;
using G3D::Vector3;

enum GeoPairType
{
	BALL_BALL_PAIR = 0,
	BALL_POINT_PAIR = 1,
	BALL_EDGE_PAIR = 2,
	BALL_PLANE_PAIR = 3,
	POINT_PLANE_PAIR = 4,
	EDGE_EDGE_PLANE_PAIR = 5,
	EDGE_EDGE_PAIR = 6,
};

// SIZE 0x20
class PairParams
{
public:
	Vector3 normal; // 0x00

	union {
		float length;   // 0x0c
		float rotation; // 0x0c
	};

	Vector3 position; // 0x10
	float rotVel;     // 0x1c

	PairParams() {}
};

DECOMP_SIZE_ASSERT(PairParams, 0x20)

// SIZE 0x24
class GeoPair
{
public:
	GeoPair();

	bool match(Body* _b0, Body* _b1, GeoPairType _pairType, int param0, int param1);

	const Body* getBody(int index) const { return (&body0)[index]; }

	Body* getBody(int index) { return (&body0)[index]; }

	void computeLengthNormalPosition(PairParams& params)
	{
		switch (geoPairType) {
		case POINT_PLANE_PAIR:
			computePointPlane(params);
			break;
		case EDGE_EDGE_PLANE_PAIR:
			computeEdgeEdgePlane(params);
			break;
		case BALL_BALL_PAIR:
			computeBallBall(params);
			break;
		case BALL_PLANE_PAIR:
			computeBallPlane(params);
			break;
		case BALL_POINT_PAIR:
			computeBallPoint(params);
			break;
		case BALL_EDGE_PAIR:
			computeBallEdge(params);
			break;
		case EDGE_EDGE_PAIR:
			computeEdgeEdge(params);
			break;
		}
	}
	void computeNormalPerpVel(float& normalVel, Vector3& perpVel, const PairParams& params);
	void forceToBodies(const Vector3& force, const Vector3& position);

	void setBallBall(Body* body0, Body* body1, float radius0, float radiusSum);
	void setBallBlock(
		Body* body0,
		Body* body1,
		float radius0,
		const Vector3* offset1,
		NormalId normalID1,
		GeoPairType geoPairType
	);
	void setPointPlane(
		Body* body0,
		Body* body1,
		const Vector3* offset0,
		const Vector3* offset1,
		int point0ID,
		NormalId planeID
	);
	void setEdgeEdgePlane(
		Body* body0,
		Body* body1,
		const Vector3* offset0,
		const Vector3* offset1,
		NormalId normalID0,
		NormalId normalID1,
		NormalId planeID,
		float edgeLength0
	);
	void setEdgeEdge(
		Body* body0,
		Body* body1,
		const Vector3* offset0,
		const Vector3* offset1,
		NormalId normalID0,
		NormalId normalID1
	);

private:
	void computeBallBall(PairParams& params);
	void computeBallPoint(PairParams& params);
	void computeBallEdge(PairParams& params);
	void computeBallPlane(PairParams& params);
	void computePointPlane(PairParams& params);
	void computeEdgeEdgePlane(PairParams& params);
	void computeEdgeEdge(PairParams& params);

	GeoPairType geoPairType; // 0x00
	const Vector3* offset0;  // 0x04
	const Vector3* offset1;  // 0x08
	Body* body0;             // 0x0c
	Body* body1;             // 0x10
	float edgeLength0;       // 0x14

	// 0x18
	struct
	{
		union {
			NormalId normalID0;
			float radius0;
		};

		union {
			NormalId normalID1;
			float radiusSum;
		};

		union {
			NormalId planeID;
			int point0ID;
		};
	} pairData;
};

DECOMP_SIZE_ASSERT(GeoPair, 0x24)

// FUNCTION: WEBSERVICE 0x100d1ea0
inline bool GeoPair::match(Body* _b0, Body* _b1, GeoPairType _pairType, int param0, int param1)
{
	if (_pairType == POINT_PLANE_PAIR) {
		return _b0 == body0 && _b1 == body1 && param0 == pairData.point0ID && param1 == pairData.normalID1;
	}

	if (_pairType == EDGE_EDGE_PLANE_PAIR) {
		return _b0 == body0 && _b1 == body1 && param0 == pairData.normalID0 && param1 == pairData.normalID1;
	}

	return (_b0 == body0 && _b1 == body1 && param0 == pairData.normalID0 && param1 == pairData.normalID1) ||
		   (_b0 == body1 && _b1 == body0 && param0 == pairData.normalID1 && param1 == pairData.normalID0);
}

inline void GeoPair::setBallBall(Body* body0, Body* body1, float radius0, float radiusSum)
{
	this->body0 = body0;
	this->body1 = body1;

	pairData.radius0 = radius0;
	pairData.radiusSum = radiusSum;

	geoPairType = BALL_BALL_PAIR;
}

inline void GeoPair::setBallBlock(
	Body* body0,
	Body* body1,
	float radius0,
	const Vector3* offset1,
	NormalId normalID1,
	GeoPairType geoPairType
)
{
	this->body0 = body0;
	this->body1 = body1;

	this->offset1 = offset1;

	pairData.radius0 = radius0;
	pairData.normalID1 = normalID1;

	this->geoPairType = geoPairType;
}

inline void GeoPair::setPointPlane(
	Body* body0,
	Body* body1,
	const Vector3* offset0,
	const Vector3* offset1,
	int point0ID,
	NormalId planeID
)
{
	this->body0 = body0;
	this->body1 = body1;

	this->offset0 = offset0;
	this->offset1 = offset1;

	pairData.point0ID = point0ID;
	pairData.normalID1 = planeID;

	geoPairType = POINT_PLANE_PAIR;
}

inline void GeoPair::setEdgeEdgePlane(
	Body* body0,
	Body* body1,
	const Vector3* offset0,
	const Vector3* offset1,
	NormalId normalID0,
	NormalId normalID1,
	NormalId planeID,
	float edgeLength0
)
{
	this->body0 = body0;
	this->body1 = body1;

	this->offset0 = offset0;
	this->offset1 = offset1;

	this->edgeLength0 = edgeLength0;

	pairData.normalID0 = normalID0;
	pairData.normalID1 = normalID1;
	pairData.planeID = planeID;

	geoPairType = EDGE_EDGE_PLANE_PAIR;
}

inline void GeoPair::setEdgeEdge(
	Body* body0,
	Body* body1,
	const Vector3* offset0,
	const Vector3* offset1,
	NormalId normalID0,
	NormalId normalID1
)
{
	this->body0 = body0;
	this->body1 = body1;

	this->offset0 = offset0;
	this->offset1 = offset1;

	pairData.normalID0 = normalID0;
	pairData.normalID1 = normalID1;

	geoPairType = EDGE_EDGE_PAIR;
}

} // namespace RBX

#endif // V8KERNEL_PAIR_H
