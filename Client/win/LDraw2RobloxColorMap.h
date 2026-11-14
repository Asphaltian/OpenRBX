#ifndef LDRAW2ROBLOXCOLORMAP_H
#define LDRAW2ROBLOXCOLORMAP_H

#include "placeholder.h"

namespace RBX {
namespace LDraw2Lua {

class LDraw2RobloxColorMap
{
public:
	LDraw2RobloxColorMap(int p_unk0x04, int p_unk0x08);

	virtual ~LDraw2RobloxColorMap() {}

	int m_unk0x04;
	int m_unk0x08;
};

} // namespace LDraw2Lua
} // namespace RBX

#endif // LDRAW2ROBLOXCOLORMAP_H
