#ifndef UTIL_RECT_H
#define UTIL_RECT_H

#include <G3D/Rect2D.h>
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
	Rect(const Vector2& size) : low(Vector2::zero()), high(size) {}
	Rect(float lowX, float lowY, float highX, float highY) : low(lowX, lowY), high(highX, highY) {}

	static Rect fromLowSize(const Vector2& low, const Vector2& size) { return Rect(low, low + size); }

	G3D::Rect2D toRect2D() const { return G3D::Rect2D::xyxy(low, high); }

	Vector2 size() const { return high - low; }
	Vector2 center() const { return (high + low) * 0.5f; }

	// FUNCTION: WEBSERVICE 0x100d0f60
	bool pointInRect(G3D::Vector2int16 point) const
	{
		int y = point.y;
		int x = point.x;

		return x >= low.x && x <= high.x && y >= low.y && y <= high.y;
	}

	Rect inset(const G3D::Vector2int16& amount) const
	{
		return Rect(low.x + amount.x, low.y + amount.y, high.x - amount.x, high.y - amount.y);
	}

	Rect positionChild(const Rect& child, Location xLocation, Location yLocation) const;
};

} // namespace RBX

#endif // UTIL_RECT_H
