#ifndef APPDRAW_RBXFONT_H
#define APPDRAW_RBXFONT_H

#include "decomp.h"
#include "rbxgraphics/Adorn.h"

#include <OgreTexture.h>
#include <OgreVector2.h>
#include <string>

namespace Ogre {

// SIZE 0x218
class RbxFont
{
public:
	Vector2 FUN_0067de70(
		const std::string& s,
		double x,
		double y,
		double width,
		double height,
		RBX::Adorn::Spacing spacing,
		Vector2* quads
	);
	Vector2 FUN_0067e0f0(const std::string& s, double size, RBX::Adorn::Spacing spacing);

private:
	int m_unk0x00[128];    // 0x00
	int m_unk0x200;        // 0x200
	int m_unk0x204;        // 0x204
	int m_unk0x208;        // 0x208
	TexturePtr m_unk0x20c; // 0x20c
};

DECOMP_SIZE_ASSERT(RbxFont, 0x218)

} // namespace Ogre

#endif // APPDRAW_RBXFONT_H
