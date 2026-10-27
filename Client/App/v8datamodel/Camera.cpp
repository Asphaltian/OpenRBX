#include "v8datamodel/Camera.h"

#include "util/ICameraSubject.h"
#include "v8datamodel/ICameraOwner.h"

namespace RBX {

char sCamera[] = "Camera";

// FUNCTION: WEBSERVICE 0x100c2630
ICameraOwner* Camera::getCameraOwner()
{
	Instance* parent = getParent();
	while (parent) {
		if (ICameraOwner* owner = dynamic_cast<ICameraOwner*>(parent)) {
			return owner;
		}

		parent = parent->getParent();
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x100c2680
void Camera::lookAt(const G3D::Vector3& point)
{
	cameraFocus.translation = point;
	cameraGoal.lookAt(point);
}

// FUNCTION: WEBSERVICE 0x100c2810
float Camera::getNewZoomDistance(float currentDistance, float in)
{
	static const float ZOOM_FACTOR = 0.25f;

	float newDistance;

	if (in > 0.0f) {
		newDistance = std::max(currentDistance / (1.0f + in * ZOOM_FACTOR), distanceMin());
	}
	else if (in < 0.0f) {
		newDistance = std::min(currentDistance * (1.0f - in * ZOOM_FACTOR), distanceMax());
	}
	else {
		newDistance = currentDistance;
	}

	return newDistance;
}

// STUB: WEBSERVICE 0x100c28a0
bool Camera::setDistanceFromTarget(float newDistance)
{
	STUB(0x100c28a0);
	return false;
}

// FUNCTION: WEBSERVICE 0x100c29e0
Instance* Camera::getCameraSubjectInstance() const
{
	return cameraSubject.get();
}

// FUNCTION: WEBSERVICE 0x100c29f0
ICameraSubject* Camera::getCameraSubject() const
{
	return cameraSubject ? dynamic_cast<ICameraSubject*>(cameraSubject.get()) : NULL;
}

// FUNCTION: WEBSERVICE 0x100c2a20
void Camera::autoMode()
{
	if (animationType != AUTO) {
		animationType = AUTO;

		if (ICameraOwner* owner = getCameraOwner()) {
			owner->cameraMoved();
		}
	}
}

// STUB: WEBSERVICE 0x100c31e0
bool Camera::zoom(float in)
{
	STUB(0x100c31e0);
	return false;
}

// STUB: WEBSERVICE 0x100c4470
void Camera::setCameraType(CameraType value)
{
	STUB(0x100c4470);
}

// STUB: WEBSERVICE 0x100c44b0
void Camera::setCameraSubject(Instance* value)
{
	STUB(0x100c44b0);
}

// STUB: WEBSERVICE 0x100c4770
void Camera::zoomExtents(Extents extents, const G3D::Rect2D& viewPort, ZoomType zoomType)
{
	STUB(0x100c4770);
}

// STUB: WEBSERVICE 0x100c4aa0
bool Camera::zoomExtents(const G3D::Rect2D& viewPort)
{
	STUB(0x100c4aa0);
	return false;
}

// STUB: WEBSERVICE 0x100c4d80
void Camera::doFly(const NavKeys& navKeys, int steps)
{
	STUB(0x100c4d80);
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::Camera, RBX::Instance, RBX::sCamera>;
template class RBX::Reflection::
	Described<RBX::Camera, RBX::sCamera, RBX::FactoryProduct<RBX::Camera, RBX::Instance, RBX::sCamera> >;
template class RBX::DescribedCreatable<RBX::Camera, RBX::Instance, RBX::sCamera>;
