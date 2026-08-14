#include "util/Rect.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x1011ced0
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
