#ifndef UTIL_UNITS_H
#define UTIL_UNITS_H

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

class Units
{
public:
	static Vector3 kmsAccelerationToRbx(const Vector3& a);
	static float kmsForceToRbx(float f);
};

} // namespace RBX

#endif // UTIL_UNITS_H
