#ifndef UTIL_RECT_H
#define UTIL_RECT_H

#include <G3D/Vector2.h>

namespace RBX {

using G3D::Vector2;

class Rect
{
public:
	enum Location
	{
		TOP = 0,
		BOTTOM = 1,
		LEFT = 2,
		RIGHT = 3,
		CENTER = 4,
		NONE = 5,
	};

	Vector2 low;  // 0x00
	Vector2 high; // 0x08

	Rect() {}
	Rect(float lowX, float lowY, float highX, float highY) : low(lowX, lowY), high(highX, highY) {}

	Rect positionChild(const Rect& child, Location xLocation, Location yLocation) const;
};

} // namespace RBX

#endif // UTIL_RECT_H
