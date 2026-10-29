#include "util/Units.h"

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

// FUNCTION: WEBSERVICE 0x10121f50
// FUNCTION: ROBLOX 0x006620b0
Vector3 Units::kmsAccelerationToRbx(const Vector3& kmsAccel)
{
	return kmsAccel * 20.0f;
}

// FUNCTION: WEBSERVICE 0x10121f80
// FUNCTION: ROBLOX 0x006620e0
float Units::kmsForceToRbx(float kmsForce)
{
	return kmsForce * 20.0f;
}

} // namespace RBX
