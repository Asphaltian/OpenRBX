#ifndef UTIL_PV_H
#define UTIL_PV_H

#include "placeholder.h"
#include "util/Velocity.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/Vector3.h>

namespace RBX {

using G3D::CoordinateFrame;
using G3D::Vector3;

class PV
{
public:
	CoordinateFrame position;
	Velocity velocity;

	bool operator==(const PV&) const;
	bool operator!=(const PV&) const;

	PV() {}
	PV(const CoordinateFrame& position, const Velocity& velocity) : position(position), velocity(velocity) {}
	PV(const PV& other) : position(other.position), velocity(other.velocity) {}
	~PV() {}

	PV inverse() const;
	PV toObjectSpace(const PV&) const;

	Vector3 linearVelocityAtPoint(const Vector3& worldPos) const;

	Velocity velocityAtPoint(const Vector3& worldPos) const;

	Velocity velocityAtLocalOffset(const Vector3&) const;

	PV pvAtLocalOffset(const Vector3& localOffset) const;

	PV pvAtLocalCoord(const CoordinateFrame& localCoord) const;

	PV operator*(const PV&) const;
};

inline Vector3 PV::linearVelocityAtPoint(const Vector3& worldPos) const
{
	return velocity.linearVelocityAtOffset(worldPos - position.translation);
}

inline Velocity PV::velocityAtPoint(const Vector3& worldPos) const
{
	Vector3 offset = worldPos - position.translation;

	return Velocity(velocity.linear + velocity.rotational.cross(offset), velocity.rotational);
}

inline PV PV::pvAtLocalCoord(const CoordinateFrame& localCoord) const
{
	CoordinateFrame worldPos = position * localCoord;
	Velocity worldVel = velocityAtPoint(worldPos.translation);

	return PV(worldPos, worldVel);
}

inline PV PV::pvAtLocalOffset(const Vector3& localOffset) const
{
	return pvAtLocalCoord(CoordinateFrame(localOffset));
}

} // namespace RBX

#endif // UTIL_PV_H
