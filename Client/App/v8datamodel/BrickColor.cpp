#include "v8datamodel/BrickColor.h"

#include <G3D/g3dmath.h>
#include <cmath>
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

// FUNCTION: WEBSERVICE 0x100921f0
// FUNCTION: ROBLOX 0x005be1b0
void BrickColor::BrickMap::generatePaletteMap(Number number)
{
	float closestDistance = 10000.0f;
	int match = 0;

	G3D::Color4 c = map2[number];

	std::vector<BrickColor>::iterator iter = colorPalette.begin();
	std::vector<BrickColor>::iterator end = colorPalette.end();

	while (iter != end) {
		if (iter->number == number) {
			match = iter - colorPalette.begin();
			break;
		}

		G3D::Color3 p(map2[iter->number]);
		float distance = fabs(c.r - p.r);
		distance += fabs(c.g - p.g);
		distance += fabs(c.b - p.b);

		if (distance < closestDistance) {
			match = iter - colorPalette.begin();

			if (distance == 0.0f) {
				break;
			}

			closestDistance = distance;
		}

		++iter;
	}

	paletteMap[number] = match;
}

// FUNCTION: WEBSERVICE 0x10092360
// FUNCTION: ROBLOX 0x005be320
void BrickColor::BrickMap::generatePaletteMap()
{
	std::vector<BrickColor>::iterator iter = allColors.begin();
	std::vector<BrickColor>::iterator end = allColors.end();

	while (iter != end) {
		generatePaletteMap(iter->number);
		++iter;
	}
}

// FUNCTION: WEBSERVICE 0x100923f0
// FUNCTION: ROBLOX 0x005be3b0
void BrickColor::BrickMap::insert(Number number, unsigned char r, unsigned char g, unsigned char b, std::string name)
{
	G3D::Color4uint8 c;
	c.r = r;
	c.g = g;
	c.b = b;

	allColors.push_back(BrickColor(number));

	map1.insert(std::pair<Number, G3D::Color4uint8>(number, c));

	std::pair<Number, G3D::Color4> p2(number, c);
	map2.insert(p2);

	std::pair<Number, std::string> p3(number, name);
	map3.insert(p3);
}

// FUNCTION: WEBSERVICE 0x100926b0
// FUNCTION: ROBLOX 0x005be670
BrickColor::BrickMap::BrickMap()
{
	insert(lego_1, 242, 243, 242, "White");
	insert(lego_2, 161, 165, 162, "Grey");
	insert(lego_3, 249, 233, 153, "Light yellow");
	insert(lego_5, 215, 197, 153, "Brick yellow");
	insert(lego_6, 194, 218, 184, "Light green (Mint)");
	insert(lego_9, 232, 186, 199, "Light reddish violet");
	insert(lego_11, 128, 187, 219, "Pastel Blue");
	insert(lego_12, 203, 132, 66, "Light orange brown");
	insert(lego_18, 204, 142, 104, "Nougat");
	insert(lego_21, 196, 40, 27, "Bright red");
	insert(lego_22, 196, 112, 160, "Med. reddish violet");
	insert(lego_23, 13, 105, 171, "Bright blue");
	insert(lego_24, 245, 205, 47, "Bright yellow");
	insert(lego_25, 98, 71, 50, "Earth orange");
	insert(lego_26, 27, 42, 52, "Black");
	insert(lego_27, 109, 110, 108, "Dark grey");
	insert(lego_28, 40, 127, 70, "Dark green");
	insert(lego_29, 161, 196, 139, "Medium green");
	insert(lego_36, 243, 207, 155, "Lig. Yellowich orange");
	insert(lego_37, 75, 151, 74, "Bright green");
	insert(lego_38, 160, 95, 52, "Dark orange");
	insert(lego_39, 193, 202, 222, "Light bluish violet");
	insert(lego_40, 236, 236, 236, "Transparent");
	insert(lego_41, 205, 84, 75, "Tr. Red");
	insert(lego_42, 193, 223, 240, "Tr. Lg blue");
	insert(lego_43, 123, 182, 232, "Tr. Blue");
	insert(lego_44, 247, 241, 141, "Tr. Yellow");
	insert(lego_45, 180, 210, 227, "Light blue");
	insert(lego_47, 217, 133, 108, "Tr. Flu. Reddish orange");
	insert(lego_48, 132, 182, 141, "Tr. Green");
	insert(lego_49, 248, 241, 132, "Tr. Flu. Green");
	insert(lego_50, 236, 232, 222, "Phosph. White");
	insert(lego_100, 238, 196, 182, "Light red");
	insert(lego_101, 218, 134, 121, "Medium red");
	insert(lego_102, 110, 153, 201, "Medium blue");
	insert(lego_103, 199, 193, 183, "Light grey");
	insert(lego_104, 107, 50, 123, "Bright violet");
	insert(lego_105, 226, 155, 63, "Br. yellowish orange");
	insert(lego_106, 218, 133, 64, "Bright orange");
	insert(lego_107, 0, 143, 155, "Bright bluish green");
	insert(lego_108, 104, 92, 67, "Earth yellow");
	insert(lego_110, 67, 84, 147, "Bright bluish violet");
	insert(lego_111, 191, 183, 177, "Tr. Brown");
	insert(lego_112, 104, 116, 172, "Medium bluish violet");
	insert(lego_113, 228, 173, 200, "Tr. Medi. reddish violet");
	insert(lego_115, 199, 210, 60, "Med. yellowish green");
	insert(lego_116, 85, 165, 175, "Med. bluish green");
	insert(lego_118, 183, 215, 213, "Light bluish green");
	insert(lego_119, 164, 189, 70, "Br. yellowish green");
	insert(lego_120, 217, 228, 167, "Lig. yellowish green");
	insert(lego_121, 231, 172, 88, "Med. yellowish orange");
	insert(lego_123, 211, 111, 76, "Br. reddish orange");
	insert(lego_124, 146, 57, 120, "Bright reddish violet");
	insert(lego_125, 234, 184, 145, "Light orange");
	insert(lego_126, 165, 165, 203, "Tr. Bright bluish violet");
	insert(lego_127, 220, 188, 129, "Gold");
	insert(lego_128, 174, 122, 89, "Dark nougat");
	insert(lego_131, 156, 163, 168, "Silver");
	insert(lego_133, 213, 115, 61, "Neon orange");
	insert(lego_134, 216, 221, 86, "Neon green");
	insert(lego_135, 116, 134, 156, "Sand blue");
	insert(lego_136, 135, 124, 144, "Sand violet");
	insert(lego_137, 224, 152, 100, "Medium orange");
	insert(lego_138, 149, 138, 115, "Sand yellow");
	insert(lego_140, 32, 58, 86, "Earth blue");
	insert(lego_141, 39, 70, 44, "Earth green");
	insert(lego_143, 207, 226, 247, "Tr. Flu. Blue");
	insert(lego_145, 121, 136, 161, "Sand blue metallic");
	insert(lego_146, 149, 142, 163, "Sand violet metallic");
	insert(lego_147, 147, 135, 103, "Sand yellow metallic");
	insert(lego_148, 87, 88, 87, "Dark grey metallic");
	insert(lego_149, 22, 29, 50, "Black metallic");
	insert(lego_150, 171, 173, 172, "Light grey metallic");
	insert(lego_151, 120, 144, 129, "Sand green");
	insert(lego_153, 149, 121, 118, "Sand red");
	insert(lego_154, 123, 46, 47, "Dark red");
	insert(lego_157, 255, 246, 123, "Tr. Flu. Yellow");
	insert(lego_158, 225, 164, 194, "Tr. Flu. Red");
	insert(lego_168, 117, 108, 98, "Gun metallic");
	insert(lego_176, 151, 105, 91, "Red flip/flop");
	insert(lego_178, 180, 132, 85, "Yellow flip/flop");
	insert(lego_179, 137, 135, 136, "Silver flip/flop");
	insert(lego_180, 215, 169, 75, "Curry");
	insert(lego_190, 249, 214, 46, "Fire Yellow");
	insert(lego_191, 232, 171, 45, "Flame yellowish orange");
	insert(lego_192, 105, 64, 39, "Reddish brown");
	insert(lego_193, 207, 96, 36, "Flame reddish orange");
	insert(lego_194, 163, 162, 164, "Medium stone grey");
	insert(lego_195, 70, 103, 164, "Royal blue");
	insert(lego_196, 35, 71, 139, "Dark Royal blue");
	insert(lego_198, 142, 66, 133, "Bright reddish lilac");
	insert(lego_199, 99, 95, 97, "Dark stone grey");
	insert(lego_200, 130, 138, 93, "Lemon metalic");
	insert(lego_208, 229, 228, 222, "Light stone grey");
	insert(lego_209, 176, 142, 68, "Dark Curry");
	insert(lego_210, 112, 149, 120, "Faded green");
	insert(lego_211, 121, 181, 181, "Turquoise");
	insert(lego_212, 159, 195, 233, "Light Royal blue");
	insert(lego_213, 108, 129, 183, "Medium Royal blue");
	insert(lego_216, 143, 76, 42, "Rust");
	insert(lego_217, 124, 92, 69, "Brown");
	insert(lego_218, 150, 112, 159, "Reddish lilac");
	insert(lego_219, 107, 98, 155, "Lilac");
	insert(lego_220, 167, 169, 206, "Light lilac");
	insert(lego_221, 205, 98, 152, "Bright purple");
	insert(lego_222, 228, 173, 200, "Light purple");
	insert(lego_223, 220, 144, 149, "Light pink");
	insert(lego_224, 240, 213, 160, "Light brick yellow");
	insert(lego_225, 235, 184, 127, "Warm yellowish orange");
	insert(lego_226, 253, 234, 140, "Cool yellow");
	insert(lego_232, 125, 187, 221, "Dove blue");
	insert(lego_268, 52, 43, 117, "Medium lilac");

	colorPalette.push_back(BrickColor(lego_1));
	colorPalette.push_back(BrickColor(lego_208));
	colorPalette.push_back(BrickColor(lego_194));
	colorPalette.push_back(BrickColor(lego_199));
	colorPalette.push_back(BrickColor(lego_26));
	colorPalette.push_back(BrickColor(lego_21));
	colorPalette.push_back(BrickColor(lego_24));
	colorPalette.push_back(BrickColor(lego_226));
	colorPalette.push_back(BrickColor(lego_23));
	colorPalette.push_back(BrickColor(lego_107));
	colorPalette.push_back(BrickColor(lego_102));
	colorPalette.push_back(BrickColor(lego_11));
	colorPalette.push_back(BrickColor(lego_45));
	colorPalette.push_back(BrickColor(lego_135));
	colorPalette.push_back(BrickColor(lego_106));
	colorPalette.push_back(BrickColor(lego_105));
	colorPalette.push_back(BrickColor(lego_141));
	colorPalette.push_back(BrickColor(lego_28));
	colorPalette.push_back(BrickColor(lego_37));
	colorPalette.push_back(BrickColor(lego_119));
	colorPalette.push_back(BrickColor(lego_29));
	colorPalette.push_back(BrickColor(lego_151));
	colorPalette.push_back(BrickColor(lego_38));
	colorPalette.push_back(BrickColor(lego_192));
	colorPalette.push_back(BrickColor(lego_104));
	colorPalette.push_back(BrickColor(lego_9));
	colorPalette.push_back(BrickColor(lego_101));
	colorPalette.push_back(BrickColor(lego_5));
	colorPalette.push_back(BrickColor(lego_153));
	colorPalette.push_back(BrickColor(lego_217));
	colorPalette.push_back(BrickColor(lego_18));
	colorPalette.push_back(BrickColor(lego_125));

	generatePaletteMap();
}

// FUNCTION: WEBSERVICE 0x10093cb0
// FUNCTION: ROBLOX 0x005bfc70
BrickColor::BrickMap& BrickColor::BrickMap::singleton()
{
	static BrickMap s;
	return s;
}

// FUNCTION: WEBSERVICE 0x10093d10
// FUNCTION: ROBLOX 0x005bfcd0
const BrickColor::Colors& BrickColor::colorPalette()
{
	return BrickMap::singleton().colorPalette;
}

// FUNCTION: WEBSERVICE 0x10093d20
// FUNCTION: ROBLOX 0x005bfce0
unsigned int BrickColor::getClosestPaletteIndex() const
{
	return BrickMap::singleton().paletteMap[number];
}

// FUNCTION: WEBSERVICE 0x10093d40
// FUNCTION: ROBLOX 0x005bfd00
BrickColor BrickColor::parse(const char* name)
{
	std::map<Number, std::string>::const_iterator iter = BrickMap::singleton().map3.begin();
	std::map<Number, std::string>::const_iterator end = BrickMap::singleton().map3.end();

	for (; iter != end; ++iter) {
		if (iter->second == name) {
			return BrickColor(iter->first);
		}
	}

	return BrickColor();
}

// FUNCTION: ROBLOX 0x005bfdb0
BrickColor BrickColor::closest(G3D::Color4uint8 color)
{
	int closestDistance = 10000;
	Number result = lego_194;

	std::map<Number, G3D::Color4uint8>::const_iterator iter = BrickMap::singleton().map1.begin();
	std::map<Number, G3D::Color4uint8>::const_iterator end = BrickMap::singleton().map1.end();

	while (iter != end) {
		int distance = abs(iter->second.r - color.r);
		distance += abs(iter->second.g - color.g);
		distance += abs(iter->second.b - color.b);

		if (distance < closestDistance) {
			result = iter->first;

			if (distance == 0) {
				break;
			}

			closestDistance = distance;
		}

		++iter;
	}

	return BrickColor(result);
}

// FUNCTION: WEBSERVICE 0x10093df0
// FUNCTION: ROBLOX 0x005bfed0
BrickColor BrickColor::random()
{
	int i = G3D::iRandom(0, BrickMap::singleton().colorPalette.size() - 1);
	return BrickMap::singleton().colorPalette[i];
}

// FUNCTION: WEBSERVICE 0x10093e50
// FUNCTION: ROBLOX 0x005bff30
BrickColor BrickColor::closest(G3D::Color4 color)
{
	float closestDistance = 10000.0f;
	Number result = lego_194;

	std::map<Number, G3D::Color4>::const_iterator iter = BrickMap::singleton().map2.begin();
	std::map<Number, G3D::Color4>::const_iterator end = BrickMap::singleton().map2.end();

	while (iter != end) {
		float distance = fabs(iter->second.r - color.r);
		distance += fabs(iter->second.g - color.g);
		distance += fabs(iter->second.b - color.b);

		if (distance < closestDistance) {
			result = iter->first;

			if (distance == 0.0f) {
				break;
			}

			closestDistance = distance;
		}

		++iter;
	}

	return BrickColor(result);
}

// FUNCTION: WEBSERVICE 0x10093f70
// FUNCTION: ROBLOX 0x005c0050
BrickColor::BrickColor(int number)
{
	std::map<Number, G3D::Color4uint8>::iterator result = BrickMap::singleton().map1.find((Number) number);
	if (result != BrickMap::singleton().map1.end()) {
		this->number = result->first;
	}
	else {
		this->number = lego_194;
	}
}

// FUNCTION: WEBSERVICE 0x10094000
// FUNCTION: ROBLOX 0x005c0190
const std::string& BrickColor::name() const
{
	return BrickMap::singleton().map3.find(number)->second;
}

// FUNCTION: WEBSERVICE 0x10094050
// FUNCTION: ROBLOX 0x005c01e0
G3D::Color4 BrickColor::color4() const
{
	return BrickMap::singleton().map2.find(number)->second;
}

// FUNCTION: WEBSERVICE 0x100940b0
// FUNCTION: ROBLOX 0x005c0240
G3D::Color3 BrickColor::color3() const
{
	return G3D::Color3(color4());
}

// FUNCTION: ROBLOX 0x005c0270
BrickColor BrickColor::closest(G3D::Color3uint8 color)
{
	return closest(G3D::Color4uint8(color, 0));
}

// FUNCTION: WEBSERVICE 0x100940e0
// FUNCTION: ROBLOX 0x005c02b0
BrickColor BrickColor::closest(G3D::Color3 color)
{
	G3D::Color4 c4;
	c4.r = color.r;
	c4.g = color.g;
	c4.b = color.b;
	return closest(c4);
}

} // namespace RBX
