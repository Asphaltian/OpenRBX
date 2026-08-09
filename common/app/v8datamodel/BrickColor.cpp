#include "v8datamodel/BrickColor.h"

namespace RBX {

// STUB: WEBSERVICE 0x10094000
const std::string& BrickColor::name() const
{
	STUB(0x10094000);
	static const std::string empty;
	return empty;
}

// STUB: WEBSERVICE 0x100940b0
G3D::Color3 BrickColor::color3() const
{
	STUB(0x100940b0);
	return G3D::Color3();
}

// STUB: WEBSERVICE 0x100940e0
BrickColor BrickColor::closest(G3D::Color3 color)
{
	STUB(0x100940e0);
	return BrickColor();
}

} // namespace RBX
