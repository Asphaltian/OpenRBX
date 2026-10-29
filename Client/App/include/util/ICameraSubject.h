#ifndef UTIL_ICAMERASUBJECT_H
#define UTIL_ICAMERASUBJECT_H

#include "decomp.h"
#include "util/ILocation.h"

#include <G3D/CoordinateFrame.h>
#include <vector>

namespace RBX {

class Primitive;

// SIZE 0x0c
class __declspec(novtable) ICameraSubject : public virtual ILocation
{
public:
	ICameraSubject() {}

	virtual ~ICameraSubject() {} // vtable+0x00

	virtual void onCameraNear(float distance) {} // vtable+0x04

	virtual void getCameraIgnorePrimitives(std::vector<const Primitive*>& primitives) {} // vtable+0x08

	virtual float getLerp() const { return 0.9f; } // vtable+0x0c

	virtual bool zoom(const float zoom, CoordinateFrame& goal, CoordinateFrame& focus) // vtable+0x10
	{
		return false;
	}

	// FUNCTION: WEBSERVICE 0x1005aa10
	virtual void stepGoalAndFocus(
		CoordinateFrame& cameraGoal, CoordinateFrame& cameraFocus, bool cameraExternallyAdjusted
	) // vtable+0x14
	{
		cameraFocus = getLocation();
	}
};

DECOMP_SIZE_ASSERT(ICameraSubject, 0x0c)

// clang-format off
// FUNCTION: WEBSERVICE 0x1005aa00
// RBX::ICameraSubject::getLerp
// clang-format on

} // namespace RBX

#endif // UTIL_ICAMERASUBJECT_H
