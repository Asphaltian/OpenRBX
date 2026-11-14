#include "util/Units.h"

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

Vector3 Units::kmsAccelerationToRbx(const Vector3& kmsAccel)
{
	return kmsAccel * 20.0f;
}

float Units::kmsForceToRbx(float kmsForce)
{
	return kmsForce * 20.0f;
}

} // namespace RBX
