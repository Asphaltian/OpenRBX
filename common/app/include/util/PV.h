#ifndef UTIL_PV_H
#define UTIL_PV_H

#include "decomp.h"
#include "util/Velocity.h"

#include <G3D/CoordinateFrame.h>

namespace RBX {

using G3D::CoordinateFrame;

class PV
{
public:
	CoordinateFrame position; // 0x00
	Velocity velocity;        // 0x30

	PV() {}
	PV(const CoordinateFrame& position, const Velocity& velocity) : position(position), velocity(velocity) {}
};

DECOMP_SIZE_ASSERT(PV, 0x48)

} // namespace RBX

#endif // UTIL_PV_H
