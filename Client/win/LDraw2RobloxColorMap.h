#ifndef LDRAW2ROBLOXCOLORMAP_H
#define LDRAW2ROBLOXCOLORMAP_H

#include "decomp.h"

namespace RBX {
namespace LDraw2Lua {

// VTABLE: ROBLOX 0x0080fe90
// SIZE 0x0c
class LDraw2RobloxColorMap
{
public:
	LDraw2RobloxColorMap(int p_unk0x04, int p_unk0x08);

	virtual ~LDraw2RobloxColorMap() {}

	// SYNTHETIC: ROBLOX 0x00471110
	// RBX::LDraw2Lua::LDraw2RobloxColorMap::`scalar deleting destructor'

	int m_unk0x04; // 0x04
	int m_unk0x08; // 0x08
};

DECOMP_SIZE_ASSERT(LDraw2RobloxColorMap, 0x0c)

} // namespace LDraw2Lua
} // namespace RBX

#endif // LDRAW2ROBLOXCOLORMAP_H
