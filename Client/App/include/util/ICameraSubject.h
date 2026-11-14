#ifndef UTIL_ICAMERASUBJECT_H
#define UTIL_ICAMERASUBJECT_H

#include "placeholder.h"
#include "util/ILocation.h"

#include <G3D/CoordinateFrame.h>
#include <vector>

namespace RBX {

class Primitive;

class __declspec(novtable) ICameraSubject : public virtual ILocation
{
public:
	ICameraSubject() {}

	virtual ~ICameraSubject() {}

	virtual void onCameraNear(float distance) {}

	virtual void getCameraIgnorePrimitives(std::vector<const Primitive*>& primitives) {}

	virtual float getLerp() const { return 0.9f; }

	virtual bool zoom(const float zoom, CoordinateFrame& goal, CoordinateFrame& focus) { return false; }

	virtual void stepGoalAndFocus(
		CoordinateFrame& cameraGoal, CoordinateFrame& cameraFocus, bool cameraExternallyAdjusted
	)
	{
		cameraFocus = getLocation();
	}
};

} // namespace RBX

#endif // UTIL_ICAMERASUBJECT_H
