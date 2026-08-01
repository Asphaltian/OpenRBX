#include "util/Extents.h"

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

// STUB: WEBSERVICE 0x100a5d20
bool Extents::overlapsOrTouches(const Extents& other) const
{
	return !(low.x > other.high.x) && !(low.y > other.high.y) && !(low.z > other.high.z) && !(high.y < other.low.y) &&
		   !(high.x < other.low.x) && !(high.z < other.low.z);
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
	static const Extents extents(Vector3::zero(), Vector3::zero());

	return extents;
}

// FUNCTION: WEBSERVICE 0x100a5fd0
Vector3 Extents::faceCenter(NormalId normalId) const
{
	Vector3 answer = (high + low) * 0.5f;

	const int axis = normalId % 3;
	answer[axis] = normalId < 3 ? high[axis] : low[axis];

	return answer;
}

} // namespace RBX
