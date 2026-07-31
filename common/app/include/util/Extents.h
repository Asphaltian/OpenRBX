#ifndef UTIL_EXTENTS_H
#define UTIL_EXTENTS_H

#include "decomp.h"

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

class Extents
{
public:
	const Vector3& min() const { return low; }
	const Vector3& max() const { return high; }

private:
	Vector3 low;  // 0x00
	Vector3 high; // 0x0c
};

DECOMP_SIZE_ASSERT(Extents, 0x18)

} // namespace RBX

#endif // UTIL_EXTENTS_H
