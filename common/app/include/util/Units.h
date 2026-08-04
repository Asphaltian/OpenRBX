#ifndef UTIL_UNITS_H
#define UTIL_UNITS_H

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

namespace Units {

Vector3 kmsAccelerationToRbx(const Vector3& a);
float kmsForceToRbx(float f);

} // namespace Units
} // namespace RBX

#endif // UTIL_UNITS_H
