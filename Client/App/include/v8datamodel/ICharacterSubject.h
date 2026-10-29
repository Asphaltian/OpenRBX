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

	CoordinateFrame getFocusLookingAtGoal(const CoordinateFrame& cameraFocus, const CoordinateFrame& cameraGoal);
	bool testOcclusion(const CoordinateFrame& cameraFocus, const CoordinateFrame& cameraGoal, float& distance);
	CoordinateFrame goalFromDistance(
		const CoordinateFrame& cameraFocus, const CoordinateFrame& cameraGoal, const float distance
	);

	virtual bool zoom(const float in, CoordinateFrame& cameraGoal, CoordinateFrame& cameraFocus); // vtable+0x10
	virtual void stepGoalAndFocus(
		CoordinateFrame& cameraGoal, CoordinateFrame& cameraFocus, bool cameraExternallyAdjusted
	); // vtable+0x14

public:
	ICharacterSubject();

	virtual ContactManager* getContactManager() = 0;                    // vtable+0x18
	virtual void getIgnorePrims(std::vector<const Primitive*>& prims);  // vtable+0x1c
	virtual void tellCameraNear(float distance);                        // vtable+0x20
	virtual G3D::Vector3 getIntendedMovementVector() = 0;               // vtable+0x24
	virtual float getIntendedRotationAboutYAxis() = 0;                  // vtable+0x28
	virtual void cameraSetWalkOrientation(float angle, bool value) = 0; // vtable+0x2c

	void onHeartBeat(CoordinateFrame& cameraGoal, CoordinateFrame& cameraFocus);
	void setCursorCenterLocked(bool value);
};

DECOMP_SIZE_ASSERT(ICharacterSubject, 0x58)

} // namespace RBX

#endif // V8DATAMODEL_ICHARACTERSUBJECT_H
