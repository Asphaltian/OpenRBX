#include "util/Units.h"

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

// FUNCTION: WEBSERVICE 0x10121f50
Vector3 Units::kmsAccelerationToRbx(const Vector3& a)
{
	return a * 20.0f;
}

// FUNCTION: WEBSERVICE 0x10121f80
float Units::kmsForceToRbx(float f)
{
	return f * 20.0f;
}

} // namespace RBX
