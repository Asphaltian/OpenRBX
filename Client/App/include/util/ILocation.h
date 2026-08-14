#ifndef UTIL_ILOCATION_H
#define UTIL_ILOCATION_H

#include "decomp.h"

#include <G3D/CoordinateFrame.h>

namespace RBX {

using G3D::CoordinateFrame;

// SIZE 0x4
class __declspec(novtable) ILocation
{
public:
	virtual const CoordinateFrame getLocation() const = 0; // vtable+0x00
};

DECOMP_SIZE_ASSERT(ILocation, 0x4)

} // namespace RBX

#endif // UTIL_ILOCATION_H
