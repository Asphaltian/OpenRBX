#ifndef UTIL_UNITS_H
#define UTIL_UNITS_H

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

class Units
{
public:
	static float mPerRbx();
	static float rbxPerM();
	static float rbxPerMM();
	static Vector3 kmsVelocityToRbx(const Vector3&);
	static Vector3 kmsAccelerationToRbx(const Vector3& a);
	static float kmsForceToRbx(float f);
	static Vector3 kmsForceToRbx(const Vector3&);
	static Vector3 kmsTorqueToRbx(const Vector3&);
	static float kmsDensityToRbx(float);
	static Vector3 kmsKRotToRbx(const Vector3&);
	static Vector3 kmsKRotDampToRbx(const Vector3&);
};

} // namespace RBX

#endif // UTIL_UNITS_H
