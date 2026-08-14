#ifndef GUI_LAYOUT_H
#define GUI_LAYOUT_H

#include "decomp.h"
#include "util/Rect.h"

#include <G3D/Color4.h>
#include <G3D/Vector2int16.h>

namespace RBX {

// SIZE 0x20
class Layout
{
public:
	enum Style
	{
		HORIZONTAL = 0,
		VERTICAL = 1
	};

	Rect::Location xLocation;  // 0x00
	Rect::Location yLocation;  // 0x04
	G3D::Vector2int16 offset;  // 0x08
	Style layoutStyle;         // 0x0c
	G3D::Color4 backdropColor; // 0x10
};

DECOMP_SIZE_ASSERT(Layout, 0x20)

} // namespace RBX

#endif // GUI_LAYOUT_H
