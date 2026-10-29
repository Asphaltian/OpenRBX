#ifndef UTIL_COLOR_H
#define UTIL_COLOR_H

#include "decomp.h"

#include <G3D/Color3.h>

namespace RBX {

class Color
{
private:
	G3D::Color3 rgb; // 0x00

	Color(float r, float g, float b);

	const G3D::Color3& color3();

public:
	static const G3D::Color3& black();
	static const G3D::Color3& darkGray();
	static const G3D::Color3& red();
	static const G3D::Color3& blue();
	static const G3D::Color3& green();
	static const G3D::Color3& brown();
	static const G3D::Color3& purple();
	static const G3D::Color3& lightGray();

	// FUNCTION: WEBSERVICE 0x100a07f0
	// FUNCTION: ROBLOX 0x005e8d70
	static const G3D::Color3& lightGreen()
	{
		static G3D::Color3 c(129 / 255.0f, 197 / 255.0f, 22 / 255.0f);
		return c;
	}

	static const G3D::Color3& lightBlue();
	static const G3D::Color3& cyan();
	static const G3D::Color3& orange();
	static const G3D::Color3& yellow();
	static const G3D::Color3& tan();
	static const G3D::Color3& pink();
	static const G3D::Color3& white();
	static const G3D::Color3& colorFromIndex8(int);
};

DECOMP_SIZE_ASSERT(Color, 0xc)

} // namespace RBX

#endif // UTIL_COLOR_H
