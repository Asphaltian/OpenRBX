#ifndef UTIL_PV_H
#define UTIL_PV_H

#include "decomp.h"
#include "util/Velocity.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/Vector3.h>

namespace RBX {

using G3D::CoordinateFrame;
using G3D::Vector3;

class PV
{
public:
	CoordinateFrame position; // 0x00
	Velocity velocity;        // 0x30

	PV() {}
	PV(const CoordinateFrame& position, const Velocity& velocity) : position(position), velocity(velocity) {}

	Vector3 linearVelocityAtPoint(const Vector3& worldPos) const;

	Velocity velocityAtPoint(const Vector3& worldPos) const;

	PV pvAtLocalOffset(const Vector3& localOffset) const;

	PV pvAtLocalCoord(const CoordinateFrame& localCoord) const;
};

inline Vector3 PV::linearVelocityAtPoint(const Vector3& worldPos) const
{
	return velocity.linearVelocityAtOffset(worldPos - position.translation);
}

// FUNCTION: WEBSERVICE 0x1009a610
inline Velocity PV::velocityAtPoint(const Vector3& worldPos) const
{
	Vector3 offset = worldPos - position.translation;

	return Velocity(velocity.linear + velocity.rotational.cross(offset), velocity.rotational);
}

// FUNCTION: WEBSERVICE 0x1009a680
inline PV PV::pvAtLocalCoord(const CoordinateFrame& localCoord) const
{
	CoordinateFrame worldPos = position * localCoord;
	Velocity worldVel = velocityAtPoint(worldPos.translation);

	return PV(worldPos, worldVel);
}

// FUNCTION: WEBSERVICE 0x10120f60
inline PV PV::pvAtLocalOffset(const Vector3& localOffset) const
{
	return pvAtLocalCoord(CoordinateFrame(localOffset));
}

DECOMP_SIZE_ASSERT(PV, 0x48)

} // namespace RBX

#endif // UTIL_PV_H
