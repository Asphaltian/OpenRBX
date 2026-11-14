#ifndef GUI_LAYOUT_H
#define GUI_LAYOUT_H

#include "placeholder.h"
#include "util/Rect.h"

#include <G3D/Color4.h>
#include <G3D/Vector2int16.h>

namespace RBX {

class Layout
{
public:
	Layout()
		: xLocation(Rect::LEFT), yLocation(Rect::TOP), offset(0, 0), layoutStyle(HORIZONTAL),
		  backdropColor(G3D::Color4::clear())
	{
	}

	enum Style
	{
		HORIZONTAL = 0,
		VERTICAL = 1
	};

	Rect::Location xLocation;
	Rect::Location yLocation;
	G3D::Vector2int16 offset;
	Style layoutStyle;
	G3D::Color4 backdropColor;
};

} // namespace RBX

#endif // GUI_LAYOUT_H
