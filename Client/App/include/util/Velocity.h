#ifndef UTIL_VELOCITY_H
#define UTIL_VELOCITY_H

#include "decomp.h"

#include <G3D/Matrix3.h>
#include <G3D/Vector3.h>

namespace RBX {

using G3D::Matrix3;
using G3D::Vector3;

class Velocity
{
public:
	mutable Vector3 linear;     // 0x00
	mutable Vector3 rotational; // 0x0c

	// FUNCTION: WEBSERVICE 0x10104d20
	Velocity() : linear(Vector3::zero()), rotational(Vector3::zero()) {}
	Velocity(const Vector3& linear, const Vector3& rotational) : linear(linear), rotational(rotational) {}

	Vector3 linearVelocityAtOffset(const Vector3& offset) const { return linear + rotational.cross(offset); }

	Velocity velocityAtOffset(const Vector3& offset) const
	{
		return Velocity(linearVelocityAtOffset(offset), rotational);
	}

	void rotateBy(const Matrix3& rotation) const
	{
		linear = rotation * linear;
		rotational = rotation * rotational;
	}

	static const Velocity& zero()
	{
		static Velocity v;

		return v;
	}
};

DECOMP_SIZE_ASSERT(Velocity, 0x18)

} // namespace RBX

#endif // UTIL_VELOCITY_H
