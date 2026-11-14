#ifndef UTIL_ILOCATION_H
#define UTIL_ILOCATION_H

#include "placeholder.h"

#include <G3D/CoordinateFrame.h>

namespace RBX {

using G3D::CoordinateFrame;

class __declspec(novtable) ILocation
{
public:
	virtual const CoordinateFrame getLocation() const = 0;
};

} // namespace RBX

#endif // UTIL_ILOCATION_H
