#include "util/Extents.h"

namespace RBX {

// STUB: WEBSERVICE 0x100a59a0
NormalId Extents::closestFace(const Vector3& point) const
{
	NormalId answer = NORM_X;
	float closest = fabsf(point.x - high.x);

	float distance = fabsf(point.y - high.y);
	if (distance < closest) {
		closest = distance;
		answer = NORM_Y;
	}

	distance = fabsf(point.z - high.z);
	if (distance < closest) {
		closest = distance;
		answer = NORM_Z;
	}

	distance = fabsf(point.x - low.x);
	if (distance < closest) {
		closest = distance;
		answer = NORM_X_NEG;
	}

	distance = fabsf(point.y - low.y);
	if (distance < closest) {
		closest = distance;
		answer = NORM_Y_NEG;
	}

	distance = fabsf(point.z - low.z);
	if (distance < closest) {
		closest = distance;
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
	return low.x <= other.high.x && low.y <= other.high.y && low.z <= other.high.z && other.low.x <= high.x &&
		   other.low.y <= high.y && other.low.z <= high.z;
}

// STUB: WEBSERVICE 0x100a5de0
bool Extents::contains(const Vector3& point) const
{
	return low.x < point.x && low.y < point.y && low.z < point.z && point.x <= high.x && point.y <= high.y &&
		   point.z <= high.z;
}

// STUB: WEBSERVICE 0x100a5e40
bool Extents::fuzzyContains(const Vector3& point, float epsilon) const
{
	return low.x - epsilon < point.x && low.y - epsilon < point.y && low.z - epsilon < point.z &&
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
