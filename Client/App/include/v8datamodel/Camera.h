#ifndef V8DATAMODEL_CAMERA_H
#define V8DATAMODEL_CAMERA_H

#include "decomp.h"
#include "util/Extents.h"
#include "v8tree/Instance.h"

#include <G3D/GCamera.h>
#include <G3D/Rect2D.h>
#include <algorithm>

namespace RBX {

class ICameraOwner;
class NavKeys;
class ICameraSubject;

extern char sCamera[];
// SIZE 0x1b0
class Camera : public DescribedCreatable<Camera, Instance, sCamera>
{
public:
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

	enum ZoomType
	{
		ZOOM_IN_OR_OUT = 0,
		ZOOM_OUT_ONLY = 1,
		ZOOM_CHAR_PART_DRAG = 2
	};

	static float distanceMin() { return 0.5f; }
	static float distanceMax() { return 1000.0f; }
	static float distanceMaxCharacter() { return 400.0f; }

	void zoomExtents(Extents extents, const G3D::Rect2D& viewPort, ZoomType zoomType);
	bool zoomExtents(const G3D::Rect2D& viewPort);
	void alwaysMode();
	void autoMode();
	void setImageServerViewNoLerp(const G3D::CoordinateFrame& modelCoord, const G3D::Rect2D& viewPortRect);
	void doFly(const NavKeys& navKeys, int steps);
	void lookAt(const G3D::Vector3& point);

	ICameraSubject* getCameraSubject() const;
	Instance* getCameraSubjectInstance() const;
	void setCameraSubject(Instance* value);

	// FUNCTION: WEBSERVICE 0x1007c9c0
	G3D::CoordinateFrame getCameraCoordinateFrame() const { return gCamera.getCoordinateFrame(); }

	CameraType getCameraType() const { return cameraType; }
	void setCameraType(CameraType value);

	bool setDistanceFromTarget(float newDistance);
	bool zoom(float in);

	// FUNCTION: WEBSERVICE 0x100c4370
	virtual ~Camera() {}

	// SYNTHETIC: WEBSERVICE 0x100c4450
	// RBX::Camera::`scalar deleting destructor'

	const G3D::GCamera& getGCamera() const { return gCamera; }

	static float getNewZoomDistance(float currentDistance, float in);

private:
	ICameraOwner* getCameraOwner();

	G3D::GCamera gCamera;                      // 0x0f8
	G3D::CoordinateFrame cameraGoal;           // 0x13c
	G3D::CoordinateFrame cameraFocus;          // 0x16c
	CameraType cameraType;                     // 0x19c
	AnimationType animationType;               // 0x1a0
	boost::shared_ptr<Instance> cameraSubject; // 0x1a4
	bool cameraExternallyAdjusted;             // 0x1ac
};

DECOMP_SIZE_ASSERT(Camera, 0x1b0)

} // namespace RBX

#endif // V8DATAMODEL_CAMERA_H
