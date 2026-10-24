#include "v8datamodel/Camera.h"

namespace RBX {

char sCamera[] = "Camera";

// FUNCTION: WEBSERVICE 0x100c2680
void Camera::lookAt(const G3D::Vector3& point)
{
	cameraFocus.translation = point;
	cameraGoal.lookAt(point);
}

// STUB: WEBSERVICE 0x100c4770
void Camera::zoomExtents(Extents extents, const G3D::Rect2D& viewPort, ZoomType zoomType)
{
	STUB(0x100c4770);
}

// STUB: WEBSERVICE 0x100c4aa0
void Camera::zoomExtents(const G3D::Rect2D& viewPort)
{
	STUB(0x100c4aa0);
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::Camera, RBX::Instance, RBX::sCamera>;
template class RBX::Reflection::
	Described<RBX::Camera, RBX::sCamera, RBX::FactoryProduct<RBX::Camera, RBX::Instance, RBX::sCamera> >;
template class RBX::DescribedCreatable<RBX::Camera, RBX::Instance, RBX::sCamera>;
