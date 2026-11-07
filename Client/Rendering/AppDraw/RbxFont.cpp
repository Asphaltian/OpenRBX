#include "appdraw/RbxFont.h"

#include <G3D/fileutils.h>
#include <G3D/g3dmath.h>
#include <OgreDataStream.h>
#include <OgreTextureManager.h>
#include <stdio.h>

namespace Ogre {

// STUB: ROBLOX 0x0067de70
Vector2 RbxFont::FUN_0067de70(
	const std::string& s, double x, double y, double width, double height, RBX::Adorn::Spacing spacing, Vector2* quads
)
{
	unsigned int length = s.size();
	double scaleX = width / m_unk0x200;
	float scaleY = height / m_unk0x204;
	float advance = m_unk0x00['M'] * 0.85f * scaleX;
	for (unsigned int i = 0; i < length; i++) {
		char c = s[i] & 0x7f;
		if (c != ' ') {
			int row = c >> 4;
			int col = c & 0xf;
			float offset =
				spacing == RBX::Adorn::PROPORTIONAL_SPACING ? (m_unk0x200 - m_unk0x00[c]) * scaleX * 0.5 : 0.0f;
			float left = x - offset;
			quads->x = m_unk0x200 * col;
			quads->y = m_unk0x204 * row + 1;
			quads++;
			quads->x = left;
			quads->y = scaleY + y;
			quads++;
			quads->x = m_unk0x200 * col;
			quads->y = m_unk0x204 * (row + 1) - 2;
			quads++;
			quads->x = left;
			quads->y = y + height - scaleY;
			quads++;
			float right = left + width;
			quads->x = m_unk0x200 * (col + 1) - 1;
			quads->y = m_unk0x204 * (row + 1) - 2;
			quads++;
			quads->x = right;
			quads->y = y + height - scaleY;
			quads++;
			quads->x = m_unk0x200 * (col + 1) - 1;
			quads->y = m_unk0x204 * row + 1;
			quads++;
			quads->x = right;
			quads->y = scaleY + y;
			quads++;
		}
		x += spacing == RBX::Adorn::PROPORTIONAL_SPACING ? m_unk0x00[c] * scaleX : advance;
	}
	return Vector2(x - 0.0, height);
}

// FUNCTION: ROBLOX 0x0067e0f0
Vector2 RbxFont::FUN_0067e0f0(const std::string& s, double size, RBX::Adorn::Spacing spacing)
{
	double height = size * 1.5;
	double scale = m_unk0x200 * height / m_unk0x204 / m_unk0x200;
	double width = 0.0;
	int length = s.size();
	if (spacing == RBX::Adorn::PROPORTIONAL_SPACING) {
		for (int i = 0; i < length; i++) {
			width += m_unk0x00[s[i] & 0x7f] * scale;
		}
	}
	else {
		width = m_unk0x00['M'] * length * 0.85 * scale;
	}
	return Vector2(width, height);
}

// GLOBAL: ROBLOX 0x00961434
int g_unk0x00961434;

// STUB: ROBLOX 0x0067e610
RbxFont::RbxFont(const std::string& filename, G3D::BinaryInput& in)
{
	in.readUInt32();
	for (int i = 0; i < 128; i++) {
		m_unk0x00[i] = in.readUInt16();
	}
	m_unk0x208 = in.readUInt16();
	int cellSize = in.readUInt16();
	cellSize /= 16;
	m_unk0x200 = cellSize;
	m_unk0x204 = cellSize;
	int width = G3D::ceilPow2(cellSize * 16);
	int height = G3D::ceilPow2(cellSize * 8);
	const uint8* data = in.getCArray() + in.getPosition();
	char name[8];
	_snprintf(name, 8, "ft%d", g_unk0x00961434++);
	m_unk0x20c = TextureManager::getSingleton().create(name, "General");
	DataStreamPtr stream(new MemoryDataStream((void*) data, width * height));
	m_unk0x20c->loadRawData(stream, width, height, PF_A8);
}

// FUNCTION: ROBLOX 0x0067e9d0
SharedPtr<RbxFont> RbxFont::FUN_0067e9d0(const std::string& filename)
{
	if (!G3D::fileExists(filename)) {
		return SharedPtr<RbxFont>();
	}
	G3D::BinaryInput in(filename, G3D::G3D_LITTLE_ENDIAN, true);
	return SharedPtr<RbxFont>(new RbxFont(filename, in));
}

} // namespace Ogre
