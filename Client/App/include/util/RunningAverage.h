#ifndef UTIL_RUNNINGAVERAGE_H
#define UTIL_RUNNINGAVERAGE_H

#include "placeholder.h"
#include "util/Quaternion.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/Vector3.h>

namespace RBX {

using G3D::CoordinateFrame;
using G3D::Vector3;

class RunningAverageState
{
public:
	RunningAverageState() : position(0, 0, 0), angles(0, 0, 0, 1) {}

	static int stepsToSleep() { return 6; }

	void update(const CoordinateFrame& cofm, float radius);
	bool withinTolerance(const CoordinateFrame& cofm, float radius, float tolerance);

private:
	static float weight() { return 0.25f; }

	Vector3 position;
	Quaternion angles;
};

} // namespace RBX

#endif // UTIL_RUNNINGAVERAGE_H
