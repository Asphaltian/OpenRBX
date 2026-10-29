#include "v8datamodel/ICharacterSubject.h"

#include "util/Math.h"
#include "util/UserInputBase.h"
#include "v8datamodel/Camera.h"
#include "v8datamodel/UserController.h"
#include "v8datamodel/Workspace.h"
#include "v8world/SpatialHash.h"

#include <G3D/Ray.h>

namespace RBX {

// FUNCTION: WEBSERVICE 0x10107360
// FUNCTION: ROBLOX 0x0063d580
CoordinateFrame ICharacterSubject::getFocusLookingAtGoal(
	const CoordinateFrame& cameraFocus, const CoordinateFrame& cameraGoal
)
{
	CoordinateFrame answer = cameraFocus;
	answer.lookAt(cameraGoal.translation);
	return answer;
}

// FUNCTION: WEBSERVICE 0x101073a0
// FUNCTION: ROBLOX 0x0063d5c0
CoordinateFrame ICharacterSubject::goalFromDistance(
	const CoordinateFrame& cameraFocus, const CoordinateFrame& cameraGoal, const float distance
)
{
	CoordinateFrame manLookCamera = getFocusLookingAtGoal(cameraFocus, cameraGoal);

	G3D::Vector3 newCameraPt = manLookCamera.translation + manLookCamera.lookVector() * distance;

	return CoordinateFrame(newCameraPt);
}

// FUNCTION: WEBSERVICE 0x10107440
// FUNCTION: ROBLOX 0x0063d660
ICharacterSubject::ICharacterSubject()
	: focusERA(1.0f), focusOffsetERA(1.0f), lastOffset(G3D::Vector3::ZERO), rotationERA(1.0f), cursorLocked(false),
	  wasOccluded(false), hasGoalInFocus(false)
{
}

// FUNCTION: WEBSERVICE 0x101074b0
// FUNCTION: ROBLOX 0x0063d6d0
bool ICharacterSubject::zoom(const float in, CoordinateFrame& cameraGoal, CoordinateFrame& cameraFocus)
{
	G3D::Vector3 focusToGoal = cameraGoal.translation - cameraFocus.translation;

	float currentDistance = focusToGoal.magnitude();
	float newDistance = std::min(Camera::distanceMaxCharacter(), Camera::getNewZoomDistance(currentDistance, in));

	if (newDistance == currentDistance) {
		return false;
	}

	focusToGoal.y = 0.0f;
	focusToGoal.unitize();
	focusToGoal.y = 0.03f * newDistance;

	cameraGoal.translation = cameraFocus.translation + focusToGoal.direction() * newDistance;
	return true;
}

// FUNCTION: WEBSERVICE 0x101075c0
// FUNCTION: ROBLOX 0x0063d7e0
bool ICharacterSubject::testOcclusion(
	const CoordinateFrame& cameraFocus, const CoordinateFrame& cameraGoal, float& distance
)
{
	ContactManager* contactManager = getContactManager();
	if (contactManager) {
		std::vector<const Primitive*> ignorePrims;
		getIgnorePrims(ignorePrims);

		CoordinateFrame manLookCamera = getFocusLookingAtGoal(cameraFocus, cameraGoal);

		G3D::Vector3 manLeft = -manLookCamera.rotation.getColumn(0);

		int hits = 0;
		float originalDistance = (cameraGoal.translation - cameraFocus.translation).magnitude();
		distance = originalDistance;

		for (int x = -2; x <= 2; x += 2) {
			G3D::Vector3 pointOnMan = (float) x * manLeft + manLookCamera.translation;
			G3D::Ray manToCamera = G3D::Ray::fromOriginAndDirection(pointOnMan, cameraGoal.translation - pointOnMan);

			G3D::Vector3 hitPoint;
			if (contactManager->getHit(manToCamera, &ignorePrims, NULL, hitPoint)) {
				hits++;
				float thisDistance = (hitPoint - pointOnMan).magnitude();
				distance = std::min(distance, thisDistance);
			}
		}

		if (hits > 1) {
			distance -= 1.0f;
			distance = std::max(distance, Camera::distanceMin());
			distance = std::min(distance, Camera::distanceMax());
			return true;
		}

		distance = originalDistance;
	}

	return false;
}

// STUB: WEBSERVICE 0x10107860
// STUB: ROBLOX 0x0063da80
void ICharacterSubject::stepGoalAndFocus(
	CoordinateFrame& cameraGoal, CoordinateFrame& cameraFocus, bool cameraExternallyAdjusted
)
{
	G3D::Vector3 Offset;

	G3D::Vector3 delta = cameraFocus.translation - cameraGoal.translation;
	cameraFocus = getLocation();

	G3D::Vector3 intended = getIntendedMovementVector() * 16.0f;
	intended /= 30.0f;

	G3D::Vector3 focusMovement = cameraFocus.translation - focusERA.getAverage() - intended;
	focusMovement.y = 0.0f;

	Offset = focusOffsetERA.pushAndGetAverage(focusMovement);
	focusERA.pushAndGetAverage(cameraFocus.translation);

	getIntendedRotationAboutYAxis();

	CoordinateFrame headCF = getLocation();
	float heading = headCF.getHeading();
	rotationERA.getAverage();
	rotationERA.pushAndGetAverage(heading);

	CoordinateFrame focusSpace = Math::getFocusSpace(cameraFocus);

	cameraGoal.translation += Offset;

	float planarDistance = delta.xz().length();
	G3D::Vector2 planarOffset =
		(cameraFocus.translation.xz() - cameraGoal.translation.xz()).direction() * planarDistance;

	cameraGoal.translation = cameraFocus.translation - G3D::Vector3(planarOffset.x, delta.y, planarOffset.y);

	goalInFocus = focusSpace.pointToObjectSpace(cameraGoal.translation);
	hasGoalInFocus = true;

	CoordinateFrame unoccludedGoal =
		wasOccluded ? goalFromDistance(cameraFocus, cameraGoal, unoccludedDistance) : cameraGoal;

	float occludedDistance;
	bool nowOccluded = testOcclusion(cameraFocus, unoccludedGoal, occludedDistance);
	if (nowOccluded) {
		if (!wasOccluded) {
			unoccludedDistance = delta.length();
		}
		cameraGoal = goalFromDistance(cameraFocus, cameraGoal, occludedDistance);
	}
	else if (wasOccluded) {
		cameraGoal = unoccludedGoal;
	}

	wasOccluded = nowOccluded;
	tellCameraNear(occludedDistance);
}

inline void ICharacterSubject::setCursorCenterLocked(bool value)
{
	Instance* instance = dynamic_cast<Instance*>(this);
	if (ControllerService* controllerService = ServiceProvider::find<ControllerService>(instance)) {
		if (Workspace* workspace = ServiceProvider::find<Workspace>(instance)) {
			if (UserInputBase* hardwareDevice = controllerService->getHardwareDevice()) {
				if (value) {
					hardwareDevice->centerCursor();
					hardwareDevice->setWrapMode(UserInputBase::WRAP_CENTER);
					cursorLocked = true;
				}
				else if (cursorLocked) {
					if (!workspace->getInMouselookMode()) {
						hardwareDevice->setWrapMode(UserInputBase::WRAP_AUTO);
					}
					cursorLocked = false;
				}
			}
		}
	}
}

// FUNCTION: WEBSERVICE 0x10107c40
// FUNCTION: ROBLOX 0x0063de60
void ICharacterSubject::onHeartBeat(CoordinateFrame& cameraGoal, CoordinateFrame& cameraFocus)
{
	if (2.75f > (cameraGoal.translation - cameraFocus.translation).magnitude()) {
		setCursorCenterLocked(true);
	}
	else {
		setCursorCenterLocked(false);
	}

	float walkAngle = 0.0f;
	if (cursorLocked) {
		G3D::Vector3 look = cameraFocus.translation - cameraGoal.translation;
		look.y = 0.0f;
		const G3D::Vector3& lookDirection = look.direction();
		walkAngle = Math::angleToE0(G3D::Vector2(lookDirection.z, lookDirection.x));
	}

	cameraSetWalkOrientation(walkAngle, !cursorLocked);
}

} // namespace RBX
