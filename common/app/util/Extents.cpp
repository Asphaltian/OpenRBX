#include "util/Extents.h"

#include "util/Math.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x100a59a0
NormalId Extents::closestFace(const Vector3& point) const
{
	const float xHigh = fabsf(point.x - high.x);
	const float yHigh = fabsf(point.y - high.y);
	const float zHigh = fabsf(point.z - high.z);
	const float xLow = fabsf(point.x - low.x);
	const float yLow = fabsf(point.y - low.y);
	const float zLow = fabsf(point.z - low.z);

	NormalId answer = NORM_X;
	float closest = xHigh;

	if (yHigh < closest) {
		closest = yHigh;
		answer = NORM_Y;
	}
	if (zHigh < closest) {
		closest = zHigh;
		answer = NORM_Z;
	}
	if (xLow < closest) {
		closest = xLow;
		answer = NORM_X_NEG;
	}
	if (yLow < closest) {
		closest = yLow;
		answer = NORM_Y_NEG;
	}
	if (zLow < closest) {
		closest = zLow;
		answer = NORM_Z_NEG;
	}

	return answer;
}

// FUNCTION: WEBSERVICE 0x100a5a40
float Extents::areaXZ() const
{
	return (high.x - low.x) * (high.z - low.z);
}

// FUNCTION: WEBSERVICE 0x100a5a50
Vector3 Extents::getCorner(int index) const
{
	const Vector3* corners = &low;

	return Vector3(corners[index / 4].x, corners[index / 2 % 2].y, corners[index % 2].z);
}

// STUB: WEBSERVICE 0x100a5ab0
void Extents::getFaceCorners(NormalId normalId, Vector3& c0, Vector3& c1, Vector3& c2, Vector3& c3) const
{
	switch (normalId) {
	case NORM_X:
		c0 = getCorner(4);
		c1 = getCorner(6);
		c2 = getCorner(7);
		c3 = getCorner(5);
		break;
	case NORM_Y:
		c0 = getCorner(2);
		c1 = getCorner(3);
		c2 = getCorner(7);
		c3 = getCorner(6);
		break;
	case NORM_Z:
		c0 = getCorner(1);
		c1 = getCorner(5);
		c2 = getCorner(7);
		c3 = getCorner(3);
		break;
	case NORM_X_NEG:
		c0 = getCorner(0);
		c1 = getCorner(1);
		c2 = getCorner(3);
		c3 = getCorner(2);
		break;
	case NORM_Y_NEG:
		c0 = getCorner(0);
		c1 = getCorner(4);
		c2 = getCorner(5);
		c3 = getCorner(1);
		break;
	case NORM_Z_NEG:
		c0 = getCorner(0);
		c1 = getCorner(2);
		c2 = getCorner(6);
		c3 = getCorner(4);
		break;
	}
}

// FUNCTION: WEBSERVICE 0x100a5d20
bool Extents::overlapsOrTouches(const Extents& other) const
{
	if (!(low.x > other.high.x) && !(low.y > other.high.y) && !(low.z > other.high.z) && !(high.y < other.low.y) &&
		!(high.x < other.low.x) && !(high.z < other.low.z)) {
		return true;
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x100a5d80
bool Extents::separatedByMoreThan(const Extents& other, float distance) const
{
	Extents expanded(*this);
	expanded.expand(distance);

	return !expanded.overlapsOrTouches(other);
}

// STUB: WEBSERVICE 0x100a5de0
bool Extents::contains(const Vector3& point) const
{
	return !(point.x < low.x) && !(point.y < low.y) && !(point.z < low.z) && point.x <= high.x && point.y <= high.y &&
		   point.z <= high.z;
}

// FUNCTION: WEBSERVICE 0x100a5e40
bool Extents::fuzzyContains(const Vector3& point, float epsilon) const
{
	return low.x - epsilon <= point.x && low.y - epsilon <= point.y && low.z - epsilon <= point.z &&
		   point.x <= high.x + epsilon && point.y <= high.y + epsilon && point.z <= epsilon + high.z;
}

// STUB: WEBSERVICE 0x100a5eb0
const Extents& Extents::zero()
{
	static const Extents e(Vector3::zero(), Vector3::zero());

	return e;
}

// STUB: WEBSERVICE 0x100a5f70
const Extents& Extents::negativeInfiniteExtents()
{
	static const Extents e;

	return e;
}

// FUNCTION: WEBSERVICE 0x100a5fd0
Vector3 Extents::faceCenter(NormalId normalId) const
{
	Vector3 answer = (high + low) * 0.5f;

	const int axis = normalId % 3;
	answer[axis] = normalId < 3 ? high[axis] : low[axis];

	return answer;
}

// STUB: WEBSERVICE 0x100a6040
bool Extents::containedByFrustum(const G3D::GCamera::Frustum& frustum) const
{
	for (int face = 0; face < frustum.faceArray.size(); ++face) {
		for (int corner = 0; corner < 8; ++corner) {
			if (!frustum.faceArray[face].plane.halfSpaceContains(getCorner(corner))) {
				return false;
			}
		}
	}

	return true;
}

// FUNCTION: WEBSERVICE 0x100a6100
Extents Extents::vv(const Vector3& v0, const Vector3& v1)
{
	Extents answer;
	answer.low = v0.min(v1);
	answer.high = v0.max(v1);

	return answer;
}

// FUNCTION: WEBSERVICE 0x100a61d0
Extents Extents::express(const CoordinateFrame& from, const CoordinateFrame& to) const
{
	Vector3 expressedLow(Math::inf(), Math::inf(), Math::inf());
	Vector3 expressedHigh(-Math::inf(), -Math::inf(), -Math::inf());

	for (int i = 0; i < 8; ++i) {
		const Vector3 corner = to.pointToObjectSpace(from.pointToWorldSpace(getCorner(i)));

		expressedLow = expressedLow.min(corner);
		expressedHigh = expressedHigh.max(corner);
	}

	return vv(expressedLow, expressedHigh);
}

// FUNCTION: WEBSERVICE 0x100a6440
Extents Extents::toWorldSpace(const CoordinateFrame& coordinateFrame) const
{
	Vector3 worldLow(Math::inf(), Math::inf(), Math::inf());
	Vector3 worldHigh(-Math::inf(), -Math::inf(), -Math::inf());

	for (int i = 0; i < 8; ++i) {
		const Vector3 corner = coordinateFrame.pointToWorldSpace(getCorner(i));

		worldLow = worldLow.min(corner);
		worldHigh = worldHigh.max(corner);
	}

	return vv(worldLow, worldHigh);
}

// FUNCTION: WEBSERVICE 0x100a6660
Plane Extents::getPlane(NormalId normalId) const
{
	const Vector3 center = faceCenter(normalId);
	const Vector3 normal = normalIdToVector3(normalId);

	return Plane(normal, center);
}

} // namespace RBX
