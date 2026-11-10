#include "LDraw2RobloxPartMap.h"

namespace RBX {
namespace LDraw2Lua {

// FUNCTION: ROBLOX 0x00472ae0
const Unknown00473110* LDraw2RobloxPartMap::FUN_00472ae0() const
{
	if (m_unk0x5c.size() > 0) {
		return &m_unk0x5c.at(0);
	}

	return NULL;
}

// FUNCTION: ROBLOX 0x00472e40
LDraw2RobloxPartMap::LDraw2RobloxPartMap(
	std::string p_unk0x04, std::string p_unk0x20, std::string p_unk0x3c, int p_unk0x58
)
	: m_unk0x04(p_unk0x04), m_unk0x20(p_unk0x20), m_unk0x3c(p_unk0x3c), m_unk0x58(p_unk0x58)
{
}

// FUNCTION: ROBLOX 0x00472f10
LDraw2RobloxPartMap::~LDraw2RobloxPartMap()
{
}

// FUNCTION: ROBLOX 0x00473100
void LDraw2RobloxPartMap::FUN_00473100(const Unknown00473110& p_unk0x04)
{
	m_unk0x5c.push_back(p_unk0x04);
}

// FUNCTION: ROBLOX 0x00473110
Unknown00473110::Unknown00473110(int p_unk0x04, int p_unk0x08, std::string p_unk0x0c, std::string p_unk0x28)
	: m_unk0x00(p_unk0x04), m_unk0x04(p_unk0x08), m_unk0x08(p_unk0x0c), m_unk0x24(p_unk0x28)
{
}

} // namespace LDraw2Lua
} // namespace RBX
