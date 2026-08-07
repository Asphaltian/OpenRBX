#ifndef V8DATAMODEL_ICHARACTERSUBJECT_H
#define V8DATAMODEL_ICHARACTERSUBJECT_H

#include "decomp.h"
#include "util/ExponentialRunningAverage.h"
#include "util/ICameraSubject.h"

#include <G3D/Vector3.h>

namespace RBX {

class ContactManager;

// SIZE 0x58
class __declspec(novtable) ICharacterSubject : public ICameraSubject
{
private:
	Vector3ERA focusERA;       // 0x08
	Vector3ERA focusOffsetERA; // 0x18
	G3D::Vector3 lastOffset;   // 0x28
	floatERA rotationERA;      // 0x34
	bool cursorLocked;         // 0x3c
	bool wasOccluded;          // 0x3d
	float unoccludedDistance;  // 0x40
	bool hasGoalInFocus;       // 0x44
	G3D::Vector3 goalInFocus;  // 0x48

	// STUB: WEBSERVICE 0x101074b0
	virtual bool zoom(const float zoom, CoordinateFrame& goal, CoordinateFrame& focus) // vtable+0x10
	{
		STUB(0x101074b0);
		return false;
	}

	// STUB: WEBSERVICE 0x10107860
	virtual void stepGoalAndFocus(CoordinateFrame& goal, CoordinateFrame& focus, bool zoom) // vtable+0x14
	{
		STUB(0x10107860);
	}

public:
	ICharacterSubject();

	virtual ContactManager* getContactManager() = 0;                    // vtable+0x18
	virtual void getIgnorePrims(std::vector<const Primitive*>& prims);  // vtable+0x1c
	virtual void tellCameraNear(float distance);                        // vtable+0x20
	virtual G3D::Vector3 getIntendedMovementVector() = 0;               // vtable+0x24
	virtual float getIntendedRotationAboutYAxis() = 0;                  // vtable+0x28
	virtual void cameraSetWalkOrientation(float angle, bool value) = 0; // vtable+0x2c
};

DECOMP_SIZE_ASSERT(ICharacterSubject, 0x58)

} // namespace RBX

#endif // V8DATAMODEL_ICHARACTERSUBJECT_H
