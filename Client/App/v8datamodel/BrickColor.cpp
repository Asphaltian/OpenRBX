#include "v8datamodel/BrickColor.h"

#include <map>

namespace RBX {

// SIZE 0x50
class BrickColor::BrickMap
{
public:
	Colors allColors;                                    // 0x00
	Colors colorPalette;                                 // 0x10
	std::map<BrickColor::Number, G3D::Color4uint8> map1; // 0x20
	std::map<BrickColor::Number, G3D::Color4> map2;      // 0x2c
	std::map<BrickColor::Number, std::string> map3;      // 0x38
	std::map<BrickColor::Number, int> paletteMap;        // 0x44

	static BrickMap& singleton();

private:
	void insert(Number number, unsigned char r, unsigned char g, unsigned char b, std::string name);
	void insertPaletteItem(Number number);
	void generatePaletteMap();
	void generatePaletteMap(Number number);

	BrickMap();
};

DECOMP_SIZE_ASSERT(BrickColor::BrickMap, 0x50)

// STUB: WEBSERVICE 0x100926b0
BrickColor::BrickMap::BrickMap()
{
	STUB(0x100926b0);
}

// FUNCTION: WEBSERVICE 0x10093cb0
BrickColor::BrickMap& BrickColor::BrickMap::singleton()
{
	static BrickMap s;
	return s;
}

// FUNCTION: WEBSERVICE 0x10093d10
const BrickColor::Colors& BrickColor::colorPalette()
{
	return BrickMap::singleton().colorPalette;
}

// STUB: WEBSERVICE 0x10093d20
unsigned int BrickColor::getClosestPaletteIndex() const
{
	STUB(0x10093d20);
	return 0;
}

// STUB: WEBSERVICE 0x10093d40
BrickColor BrickColor::parse(const char* name)
{
	STUB(0x10093d40);
	return BrickColor();
}

// STUB: WEBSERVICE 0x10093df0
BrickColor BrickColor::random()
{
	STUB(0x10093df0);
	return BrickColor();
}

// STUB: WEBSERVICE 0x10093e50
BrickColor BrickColor::closest(G3D::Color4 color)
{
	STUB(0x10093e50);
	return BrickColor();
}

// STUB: WEBSERVICE 0x10093f70
BrickColor::BrickColor(int number)
{
	STUB(0x10093f70);
}

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
