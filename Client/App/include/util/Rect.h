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

	static bool legalX(Location);
	static bool legalY(Location);

	Vector2 low;  // 0x00
	Vector2 high; // 0x08

	Rect() {}
	Rect(G3D::Rect2D rect) : low(rect.x0y0()), high(rect.x1y1()) {}
	Rect(float lowX, float lowY, float highX, float highY) : low(lowX, lowY), high(highX, highY) {}
	Rect(const Vector2& size) : low(Vector2::zero()), high(size) {}
	Rect(const Vector2& low, const Vector2& high) : low(low), high(high) {}

	G3D::Rect2D toRect2D() const { return G3D::Rect2D::xyxy(low, high); }

	static Rect fromLowSize(const Vector2& low, const Vector2& size) { return Rect(low, low + size); }
	static Rect fromCenterSize(const Vector2&, float);

	void unionWith(const Rect&);
	void unionWith(const Vector2&);

	bool operator==(const Rect&) const;

	bool pointInRect(int, int);

	// FUNCTION: WEBSERVICE 0x100d0f60
	// FUNCTION: ROBLOX 0x0058ca90
	bool pointInRect(G3D::Vector2int16 point)
	{
		int y = point.y;
		int x = point.x;

		return x >= low.x && x <= high.x && y >= low.y && y <= high.y;
	}

	Vector2 size() const { return high - low; }
	Vector2 center() const { return (high + low) * 0.5f; }

	Location pointInBorder(const Vector2&, float);
	Vector2 positionPoint(Location, Location) const;
	Vector2 positionPoint(const Vector2&, Location, Location) const;
	Rect positionChild(const Rect& child, Location xLocation, Location yLocation) const;

	Rect inset(int amount) { return Rect(low.x + amount, low.y + amount, high.x - amount, high.y - amount); }

	Rect inset(const G3D::Vector2int16& amount)
	{
		return Rect(low.x + amount.x, low.y + amount.y, high.x - amount.x, high.y - amount.y);
	}

	Vector2 clamp(const Vector2&);
};

} // namespace RBX

#endif // UTIL_RECT_H
