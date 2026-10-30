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
	static const G3D::Color3& red()
	{
		static G3D::Color3 c(173 / 255.0f, 35 / 255.0f, 35 / 255.0f);
		return c;
	}
	static const G3D::Color3& blue()
	{
		static G3D::Color3 c(42 / 255.0f, 75 / 255.0f, 215 / 255.0f);
		return c;
	}
	static const G3D::Color3& green()
	{
		static G3D::Color3 c(29 / 255.0f, 105 / 255.0f, 20 / 255.0f);
		return c;
	}
	static const G3D::Color3& brown();
	static const G3D::Color3& purple()
	{
		static G3D::Color3 c(129 / 255.0f, 38 / 255.0f, 192 / 255.0f);
		return c;
	}
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
	static const G3D::Color3& orange()
	{
		static G3D::Color3 c(255 / 255.0f, 146 / 255.0f, 51 / 255.0f);
		return c;
	}
	static const G3D::Color3& yellow()
	{
		static G3D::Color3 c(255 / 255.0f, 238 / 255.0f, 51 / 255.0f);
		return c;
	}
	static const G3D::Color3& tan()
	{
		static G3D::Color3 c(233 / 255.0f, 222 / 255.0f, 187 / 255.0f);
		return c;
	}
	static const G3D::Color3& pink()
	{
		static G3D::Color3 c(255 / 255.0f, 205 / 255.0f, 243 / 255.0f);
		return c;
	}
	static const G3D::Color3& white();
	static const G3D::Color3& colorFromIndex8(int p_unk0x04)
	{
		switch (p_unk0x04) {
		case 0:
			return red();
		case 1:
			return blue();
		case 2:
			return green();
		case 3:
			return purple();
		case 4:
			return orange();
		case 5:
			return yellow();
		case 6:
			return pink();
		default:
			return tan();
		}
	}
};

DECOMP_SIZE_ASSERT(Color, 0xc)

// FUNCTION: ROBLOX 0x00611c20
// RBX::Color::red
// FUNCTION: ROBLOX 0x00658320
// RBX::Color::blue
// FUNCTION: ROBLOX 0x00658360
// RBX::Color::green
// FUNCTION: ROBLOX 0x006583a0
// RBX::Color::purple
// FUNCTION: ROBLOX 0x006583e0
// RBX::Color::orange
// FUNCTION: ROBLOX 0x00658420
// RBX::Color::yellow
// FUNCTION: ROBLOX 0x00658460
// RBX::Color::pink
// FUNCTION: ROBLOX 0x006584a0
// RBX::Color::colorFromIndex8

} // namespace RBX

#endif // UTIL_COLOR_H
