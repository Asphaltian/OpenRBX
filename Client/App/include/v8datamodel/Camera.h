#ifndef V8DATAMODEL_CAMERA_H
#define V8DATAMODEL_CAMERA_H

#include "decomp.h"
#include "util/Extents.h"
#include "util/Math.h"
#include "v8datamodel/ICameraOwner.h"
#include "v8tree/Instance.h"

#include <G3D/GCamera.h>
#include <G3D/Rect2D.h>
#include <algorithm>

namespace RBX {

class ICameraOwner;
class NavKeys;
class ICameraSubject;
class ContactManager;

extern char sCamera[];
// SIZE 0x1b0
class Camera : public DescribedCreatable<Camera, Instance, sCamera>
{
public:
	static float distanceMin() { return 0.5f; }
	static float distanceMax() { return 1000.0f; }
	static float distanceMaxCharacter() { return 400.0f; }
	static float distanceMinOcclude();

	enum AnimationType
	{
		ALWAYS = 0,
		AUTO = 1
	};

	enum CameraType
	{
		FIXED_CAMERA = 0,
		ATTACH_CAMERA = 1,
		WATCH_CAMERA = 2,
		TRACK_CAMERA = 3,
		FOLLOW_CAMERA = 4,
		CUSTOM_CAMERA = 5,
		NUM_CAMERA_TYPE = 6
	};
	static float distanceDefault();

private:
	G3D::GCamera gCamera;                      // 0x0f8
	G3D::CoordinateFrame cameraGoal;           // 0x13c
	G3D::CoordinateFrame cameraFocus;          // 0x16c
	CameraType cameraType;                     // 0x19c
	AnimationType animationType;               // 0x1a0
	boost::shared_ptr<Instance> cameraSubject; // 0x1a4
	bool cameraExternallyAdjusted;             // 0x1ac

	ICameraOwner* getCameraOwner();
	void updateFocus();
	void updateGoal();
	bool characterZoom(float in);
	bool nonCharacterZoom(float in);
	void tryZoomExtents(float low, float current, float high, const Extents& extents, const G3D::Rect2D& viewPort);
	ContactManager& getContactManager();

	float goalToFocusDistance() const { return (cameraGoal.translation - cameraFocus.translation).magnitude(); }

	void setGCameraCoordinateFrame(const G3D::CoordinateFrame& value)
	{
		if (Math::legalCameraCoord(value)) {
			gCamera.setCoordinateFrame(value);
		}
	}
	G3D::CoordinateFrame computeLineOfSiteGoal();

	void getHeadingElevationDistance(float& heading, float& elevation, float& distance);
	void setHeadingElevationDistance(float heading, float elevation, float distance);

	void tellCameraMoved()
	{
		if (ICameraOwner* owner = getCameraOwner()) {
			owner->cameraMoved();
		}
	}
	void getIgnorePrims(const G3D::Array<Primitive*>&);

	virtual bool askSetParent(const Instance* instance) const;

public:
	Camera();

	// FUNCTION: WEBSERVICE 0x100c4370
	// FUNCTION: ROBLOX 0x005d7ed0
	virtual ~Camera() {}

	const G3D::GCamera& getGCamera() const { return gCamera; }

	void onHeartbeat();
	void autoMode();
	void alwaysMode();

	bool isCharacterCamera() const
	{
		return cameraSubject &&
			   (cameraType == FOLLOW_CAMERA || cameraType == ATTACH_CAMERA || cameraType == TRACK_CAMERA);
	}
	bool isFirstPersonCamera() const;

	ICameraSubject* getCameraSubject() const;
	Instance* getCameraSubjectInstance() const;
	void setCameraSubject(Instance* newSubject);

	// FUNCTION: WEBSERVICE 0x100c25f0
	const G3D::CoordinateFrame& getCameraFocus() const { return cameraFocus; }

	void setCameraFocus(const G3D::CoordinateFrame& value);

	// FUNCTION: WEBSERVICE 0x1007c9c0
	// FUNCTION: ROBLOX 0x0045f4f0
	G3D::CoordinateFrame getCameraCoordinateFrame() const { return gCamera.getCoordinateFrame(); }

	void setCameraCoordinateFrameNoLerp(const G3D::CoordinateFrame& value);
	void goalToCamera();

	CameraType getCameraType() const { return cameraType; }
	void setCameraType(CameraType value);
	bool canZoom(int) const;
	bool canTilt(int) const;
	void onWrapMouse(const G3D::Vector2&);

	bool zoom(float in);
	bool setDistanceFromTarget(float newDistance);

	enum ZoomType
	{
		ZOOM_IN_OR_OUT = 0,
		ZOOM_OUT_ONLY = 1,
		ZOOM_CHAR_PART_DRAG = 2
	};

	void zoomExtents(Extents extents, const G3D::Rect2D& viewPort, ZoomType zoomType);
	bool zoomExtents(const G3D::Rect2D& viewPort);
	void panRadians(float angle);
	void panUnits(int);
	bool tiltRadians(float);
	bool tiltUnits(int);
	void lookAt(const G3D::Vector3& point);
	void setImageServerViewNoLerp(const G3D::CoordinateFrame& modelCoord, const G3D::Rect2D& viewPort);
	void doFly(const NavKeys& nav, int steps);

	static float getNewZoomDistance(float currentDistance, float in);

	// SYNTHETIC: WEBSERVICE 0x100c4450
	// SYNTHETIC: ROBLOX 0x005d7fb0
	// RBX::Camera::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(Camera, 0x1b0)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021be30
// RBX::`dynamic initializer for 'desc_cameraType''
// SYNTHETIC: WEBSERVICE 0x1021be70
// RBX::`dynamic initializer for 'desc_CoordFrame''
// SYNTHETIC: WEBSERVICE 0x1021beb0
// RBX::`dynamic initializer for 'desc_Focus''
// SYNTHETIC: WEBSERVICE 0x1021bef0
// RBX::`dynamic initializer for 'cameraSubjectProp''
// SYNTHETIC: WEBSERVICE 0x10223ab0
// RBX::`dynamic atexit destructor for 'cameraSubjectProp''
// SYNTHETIC: WEBSERVICE 0x10223ad0
// RBX::`dynamic atexit destructor for 'desc_cameraType''
// SYNTHETIC: WEBSERVICE 0x10223af0
// RBX::`dynamic atexit destructor for 'desc_Focus''
// SYNTHETIC: WEBSERVICE 0x10223b10
// RBX::`dynamic atexit destructor for 'desc_CoordFrame''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_CAMERA_H
