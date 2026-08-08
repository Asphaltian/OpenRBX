#ifndef UTIL_COLOR_H
#define UTIL_COLOR_H

#include "decomp.h"

#include <G3D/Color3.h>

namespace RBX {

class Color
{
public:
	// FUNCTION: WEBSERVICE 0x100a07f0
	static const G3D::Color3& lightGreen()
	{
		static G3D::Color3 c(129 / 255.0f, 197 / 255.0f, 22 / 255.0f);
		return c;
	}

private:
	G3D::Color3 rgb; // 0x00
};

DECOMP_SIZE_ASSERT(Color, 0xc)

} // namespace RBX

#endif // UTIL_COLOR_H
