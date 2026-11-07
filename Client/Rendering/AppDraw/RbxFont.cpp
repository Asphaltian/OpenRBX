#include "appdraw/RbxFont.h"

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

} // namespace Ogre
