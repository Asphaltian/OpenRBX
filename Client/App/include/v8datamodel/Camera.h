#ifndef V8DATAMODEL_CAMERA_H
#define V8DATAMODEL_CAMERA_H

#include "decomp.h"
#include "v8tree/Instance.h"

#include <G3D/GCamera.h>

namespace RBX {

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

	// FUNCTION: WEBSERVICE 0x100c4370
	virtual ~Camera() {}

	// SYNTHETIC: WEBSERVICE 0x100c4450
	// RBX::Camera::`scalar deleting destructor'

	const G3D::GCamera& getGCamera() const { return gCamera; }

private:
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
