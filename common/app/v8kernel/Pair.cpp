#include "v8kernel/Pair.h"

#include "decomp.h"
#include "util/Math.h"
#include "util/Velocity.h"
#include "v8kernel/Body.h"

#include <cstddef>

namespace RBX {

// FUNCTION: WEBSERVICE 0x1011cfc0
GeoPair::GeoPair() : body0(NULL), body1(NULL), offset0(NULL), offset1(NULL)
{
}

// FUNCTION: WEBSERVICE 0x1011cfe0
void GeoPair::forceToBodies(const Vector3& force, const Vector3& position)
{
	body0->accumulateForce(-force, position);
	body1->accumulateForce(force, position);
}

// FUNCTION: WEBSERVICE 0x1011d040
void GeoPair::computeNormalPerpVel(float& normalVel, Vector3& perpVel, const PairParams& params)
{
	Vector3 v0 = body0->getPV().linearVelocityAtPoint(params.position);
	Vector3 v1 = body1->getPV().linearVelocityAtPoint(params.position);

	Vector3 vel = v1 - v0;

	normalVel = params.normal.dot(vel);

	perpVel = vel - params.normal * normalVel;
}

// FUNCTION: WEBSERVICE 0x1011d1a0
void GeoPair::computeBallBall(PairParams& params)
{
	const Vector3& pos0 = body0->getPos();
	const Vector3& pos1 = body1->getPos();

	params.normal = pos1 - pos0;
	params.length = params.normal.unitize() - pairData.radiusSum;
	params.position = pos0 + params.normal * pairData.radius0;
}

// FUNCTION: WEBSERVICE 0x1011d240
void GeoPair::computeBallPoint(PairParams& params)
{
	const Vector3& pos0 = body0->getPos();
	const CoordinateFrame& coord1 = body1->getCoordinateFrame();

	params.position = coord1.pointToWorldSpace(*offset1);
	params.normal = params.position - pos0;
	params.length = params.normal.unitize() - pairData.radius0;
}

// FUNCTION: WEBSERVICE 0x1011d320
void GeoPair::computeBallEdge(PairParams& params)
{
	const Vector3& pos0 = body0->getPos();
	const CoordinateFrame& coord1 = body1->getCoordinateFrame();

	Vector3 edgePoint = coord1.pointToWorldSpace(*offset1);

	Vector3 edge = Math::getWorldNormal(pairData.normalID1, coord1);

	Vector3 delta = edgePoint - pos0;

	params.normal = delta - edge * delta.dot(edge);

	params.position = pos0 + params.normal;
	params.length = params.normal.unitize() - pairData.radius0;
}

// FUNCTION: WEBSERVICE 0x1011d480
void GeoPair::computeBallPlane(PairParams& params)
{
	const Vector3& pos0 = body0->getPos();
	const CoordinateFrame& coord1 = body1->getCoordinateFrame();
	Vector3 planePoint = coord1.pointToWorldSpace(*offset1);

	params.normal = -Math::getWorldNormal(pairData.normalID1, coord1);

	Vector3 delta = planePoint - pos0;

	Vector3 toPlane = params.normal * delta.dot(params.normal);

	params.position = pos0 + toPlane;
	params.length = toPlane.magnitude() - pairData.radius0;
}

// FUNCTION: WEBSERVICE 0x1011d5e0
void GeoPair::computePointPlane(PairParams& params)
{
	params.position = body0->getCoordinateFrame().pointToWorldSpace(*offset0);

	const CoordinateFrame& coord1 = body1->getCoordinateFrame();

	Vector3 planePoint = coord1.pointToWorldSpace(*offset1);

	params.normal = -Math::getWorldNormal(pairData.normalID1, coord1);

	params.length = (planePoint - params.position).dot(params.normal);
}

// FUNCTION: WEBSERVICE 0x1011d770
void GeoPair::computeEdgeEdgePlane(PairParams& params)
{
	const CoordinateFrame& coord0 = body0->getCoordinateFrame();
	const CoordinateFrame& coord1 = body1->getCoordinateFrame();

	Vector3 p0 = coord0.pointToWorldSpace(*offset0);
	Vector3 p1 = coord1.pointToWorldSpace(*offset1);

	Vector3 e0 = Math::getWorldNormal(pairData.normalID0, coord0);
	Vector3 e1 = Math::getWorldNormal(pairData.normalID1, coord1);

	Vector3 delta = p1 - p0;

	float dot = e1.dot(e0);
	float denom = 1.0f - dot * dot;

	params.normal = -Math::getWorldNormal(pairData.planeID, coord1);

	if (denom > 1e-05) {
		float t0 = delta.dot(e0);
		float t1 = -delta.dot(e1);

		float t = (t1 * dot + t0) / denom;

		if (6.0f < fabs(t)) {
			t = 6.0f * Math::sign(t);
		}

		params.position = p0 + e0 * t;
		params.length = (p1 - params.position).dot(params.normal);
	}
	else {
		params.position = p0;
		params.length = 0.0f;
	}
}

// FUNCTION: WEBSERVICE 0x1011daf0
void GeoPair::computeEdgeEdge(PairParams& params)
{
	const CoordinateFrame& coord0 = body0->getCoordinateFrame();
	const CoordinateFrame& coord1 = body1->getCoordinateFrame();

	Vector3 p0 = coord0.pointToWorldSpace(*offset0);
	Vector3 p1 = coord1.pointToWorldSpace(*offset1);

	Vector3 e0 = Math::getWorldNormal(pairData.normalID0, coord0);
	Vector3 e1 = Math::getWorldNormal(pairData.normalID1, coord1);

	Vector3 delta = p1 - p0;

	float dot = e1.dot(e0);
	float t0 = delta.dot(e0);
	float t1 = -e1.dot(delta);

	float denom = 1.0f - dot * dot;

	if (denom > 1e-06f) {
		float recip = 1.0f / denom;

		float s0 = (t1 * dot + t0) * recip;
		float s1 = (t0 * dot + t1) * recip;

		Vector3 c0 = p0 + e0 * s0;
		Vector3 c1 = p1 + e1 * s1;

		params.position = (c1 + c0) * 0.5f;
		params.normal = c0 - c1;
		params.length = -params.normal.unitize();
	}
	else {
		params.position = p0;
		params.normal = e0;
		params.length = 0.0f;
	}
}

} // namespace RBX
