#include "util/RunningAverage.h"

#include "util/Math.h"

namespace RBX {

// STUB: WEBSERVICE 0x10126c10
void RunningAverageState::update(const CoordinateFrame& cofm, float radius)
{
	position = position * (1.0f - weight()) + cofm.translation * weight();

	angles = angles * (1.0f - weight()) + Quaternion(cofm.rotation) * (weight() * radius);
}

// FUNCTION: WEBSERVICE 0x10126ce0
bool RunningAverageState::withinTolerance(const CoordinateFrame& cofm, float radius, float tolerance)
{
	Vector3 deltaPos = Math::vector3Abs(position - cofm.translation);

	if (Math::maxAxisLength(deltaPos) > tolerance) {
		return false;
	}

	Quaternion deltaAngles = angles - Quaternion(cofm.rotation) * radius;

	if (deltaAngles.maxComponent() > tolerance) {
		return false;
	}

	return true;
}

} // namespace RBX
