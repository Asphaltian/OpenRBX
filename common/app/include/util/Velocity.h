#ifndef UTIL_VELOCITY_H
#define UTIL_VELOCITY_H

#include "decomp.h"

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

class Velocity
{
public:
	Vector3 linear;     // 0x00
	Vector3 rotational; // 0x0c
};

DECOMP_SIZE_ASSERT(Velocity, 0x18)

} // namespace RBX

#endif // UTIL_VELOCITY_H
