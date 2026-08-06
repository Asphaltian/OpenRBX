#ifndef UTIL_RECT_H
#define UTIL_RECT_H

#include <G3D/Vector2.h>
#include <G3D/Vector2int16.h>

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
	Rect(const Vector2& low, const Vector2& high) : low(low), high(high) {}
	Rect(float lowX, float lowY, float highX, float highY) : low(lowX, lowY), high(highX, highY) {}

	static Rect fromLowSize(const Vector2& low, const Vector2& size) { return Rect(low, low + size); }

	// FUNCTION: WEBSERVICE 0x100d0f60
	bool pointInRect(G3D::Vector2int16 point) const
	{
		int y = point.y;
		int x = point.x;

		return x >= low.x && x <= high.x && y >= low.y && y <= high.y;
	}

	Rect positionChild(const Rect& child, Location xLocation, Location yLocation) const;
};

} // namespace RBX

#endif // UTIL_RECT_H
