#ifndef UTIL_VELOCITY_H
#define UTIL_VELOCITY_H

#include "placeholder.h"

#include <G3D/Matrix3.h>
#include <G3D/Vector3.h>

namespace RBX {

using G3D::Matrix3;
using G3D::Vector3;

class Velocity
{
public:
	mutable Vector3 linear;
	mutable Vector3 rotational;

	Velocity() : linear(Vector3::zero()), rotational(Vector3::zero()) {}
	Velocity(const Vector3& linear, const Vector3& rotational) : linear(linear), rotational(rotational) {}
	Velocity(const Velocity& other) : linear(other.linear), rotational(other.rotational) {}

	Vector3 linearVelocityAtOffset(const Vector3& offset) const { return linear + rotational.cross(offset); }

	Velocity velocityAtOffset(const Vector3& offset) const
	{
		return Velocity(linearVelocityAtOffset(offset), rotational);
	}

	Velocity rotateBy(const Matrix3&) const;

	static const Velocity& zero()
	{
		static Velocity v;

		return v;
	}
};

} // namespace RBX

#endif // UTIL_VELOCITY_H
