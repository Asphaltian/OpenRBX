#include "util/Rect.h"

namespace RBX {

// FUNCTION: ROBLOX 0x005d9120
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

// FUNCTION: WEBSERVICE 0x1011ced0
// FUNCTION: ROBLOX 0x005d9180
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

} // namespace RBX
