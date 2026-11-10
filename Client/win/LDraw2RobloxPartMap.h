#ifndef LDRAW2ROBLOXPARTMAP_H
#define LDRAW2ROBLOXPARTMAP_H

#include "decomp.h"

#include <string>
#include <vector>

namespace RBX {
namespace LDraw2Lua {

// SIZE 0x40
struct Unknown00473110
{
	Unknown00473110(int p_unk0x04, int p_unk0x08, std::string p_unk0x0c, std::string p_unk0x28);

	// SYNTHETIC: ROBLOX 0x00471160
	// RBX::LDraw2Lua::Unknown00473110::~Unknown00473110

	int m_unk0x00;         // 0x00
	int m_unk0x04;         // 0x04
	std::string m_unk0x08; // 0x08
	std::string m_unk0x24; // 0x24
};

DECOMP_SIZE_ASSERT(Unknown00473110, 0x40)

// VTABLE: ROBLOX 0x0080ffc8
// SIZE 0x6c
class LDraw2RobloxPartMap
{
public:
	LDraw2RobloxPartMap(std::string p_unk0x04, std::string p_unk0x20, std::string p_unk0x3c, int p_unk0x58);

	virtual ~LDraw2RobloxPartMap();

	// SYNTHETIC: ROBLOX 0x00473050
	// RBX::LDraw2Lua::LDraw2RobloxPartMap::`scalar deleting destructor'

	// FUNCTION: ROBLOX 0x00471130
	std::string FUN_00471130() const { return m_unk0x3c; }

	const Unknown00473110* FUN_00472ae0() const;
	void FUN_00473100(const Unknown00473110& p_unk0x04);

	std::string m_unk0x04;                  // 0x04
	std::string m_unk0x20;                  // 0x20
	std::string m_unk0x3c;                  // 0x3c
	int m_unk0x58;                          // 0x58
	std::vector<Unknown00473110> m_unk0x5c; // 0x5c
};

DECOMP_SIZE_ASSERT(LDraw2RobloxPartMap, 0x6c)

} // namespace LDraw2Lua
} // namespace RBX

#endif // LDRAW2ROBLOXPARTMAP_H
