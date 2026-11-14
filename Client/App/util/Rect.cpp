#include "util/Rect.h"

#include <algorithm>

namespace RBX {

Vector2 Rect::positionPoint(Location xLocation, Location yLocation) const
{
	Vector2 point = center();

	switch (xLocation) {
	case LEFT:
		point.x = low.x;
		break;
	case RIGHT:
		point.x = high.x;
		break;
	}

	switch (yLocation) {
	case TOP:
		point.y = low.y;
		break;
	case BOTTOM:
		point.y = high.y;
		break;
	case CENTER:
		return point;
	}

	return point;
}

Vector2 Rect::positionPoint(const Vector2& p_unk0x04, Location p_unk0x08, Location p_unk0x0c) const
{
	Vector2 point(0, 0);

	switch (p_unk0x08) {
	case LEFT:
		point.x = low.x + p_unk0x04.x;
		break;
	case RIGHT:
		point.x = high.x - p_unk0x04.x;
		break;
	}

	switch (p_unk0x0c) {
	case TOP:
		point.y = low.y + p_unk0x04.y;
		break;
	case BOTTOM:
		point.y = high.y - p_unk0x04.y;
		break;
	}

	return point;
}

Rect Rect::positionChild(const Rect& child, Location xLocation, Location yLocation) const
{
	float x = 0.0f;
	float y = 0.0f;

	float width = child.high.x - child.low.x;
	float height = child.high.y - child.low.y;

	switch (xLocation) {
	case LEFT:
		x = low.x;
		break;
	case RIGHT:
		x = high.x - width;
		break;
	case CENTER:
		x = (high.x - width) * 0.5f;
		break;
	}

	switch (yLocation) {
	case TOP:
		y = low.y;
		break;
	case BOTTOM:
		y = high.y - height;
		break;
	case CENTER:
		y = (high.y - height) * 0.5f;
		break;
	}

	return fromLowSize(Vector2(x, y), Vector2(width, height));
}

void Rect::unionWith(const Rect& other)
{
	low.x = std::min(low.x, other.low.x);
	low.y = std::min(low.y, other.low.y);
	high.x = std::max(high.x, other.high.x);
	high.y = std::max(high.y, other.high.y);
}

} // namespace RBX
