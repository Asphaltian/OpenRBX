#ifndef LDRAW2ROBLOXMAPROOT_H
#define LDRAW2ROBLOXMAPROOT_H

#include "LDraw2RobloxColorMap.h"
#include "LDraw2RobloxPartMap.h"
#include "decomp.h"

#include <map>
#include <string>

namespace RBX {
namespace LDraw2Lua {

// VTABLE: ROBLOX 0x0080fed4
// SIZE 0x54
class LDraw2RobloxMapRoot
{
public:
	LDraw2RobloxMapRoot(std::string p_unk0x04);

	virtual ~LDraw2RobloxMapRoot();

	// SYNTHETIC: ROBLOX 0x00471e50
	// RBX::LDraw2Lua::LDraw2RobloxMapRoot::`scalar deleting destructor'

	int FUN_004711c0(std::string p_unk0x04);
	int FUN_004712c0(std::string p_unk0x04);
	int FUN_00471430(std::string p_unk0x04);
	LDraw2RobloxColorMap* FUN_004716b0(int p_unk0x04);
	int FUN_00471710(int p_unk0x04);
	LDraw2RobloxPartMap* FUN_00471730(const std::string& p_unk0x04);
	LDraw2RobloxPartMap* FUN_00471790(const std::string& p_unk0x04);
	void FUN_00471b90(LDraw2RobloxPartMap* p_unk0x04);

	std::string m_unk0x04;                                 // 0x04
	std::string m_unk0x20;                                 // 0x20
	std::map<int, LDraw2RobloxColorMap*> m_unk0x3c;        // 0x3c
	std::map<std::string, LDraw2RobloxPartMap*> m_unk0x48; // 0x48
};

DECOMP_SIZE_ASSERT(LDraw2RobloxMapRoot, 0x54)

} // namespace LDraw2Lua
} // namespace RBX

// clang-format off
// SYNTHETIC: ROBLOX 0x007e5000
// RBX::LDraw2Lua::`dynamic initializer for 'g_unk0x00955318''
// SYNTHETIC: ROBLOX 0x007e5020
// RBX::LDraw2Lua::`dynamic initializer for 'g_unk0x0095531c''
// SYNTHETIC: ROBLOX 0x007e5040
// RBX::LDraw2Lua::`dynamic initializer for 'g_unk0x00955320''
// SYNTHETIC: ROBLOX 0x007e5060
// RBX::LDraw2Lua::`dynamic initializer for 'g_unk0x00955324''
// SYNTHETIC: ROBLOX 0x007e5080
// RBX::LDraw2Lua::`dynamic initializer for 'g_unk0x00955328''
// SYNTHETIC: ROBLOX 0x007e50a0
// RBX::LDraw2Lua::`dynamic initializer for 'g_unk0x0095532c''
// SYNTHETIC: ROBLOX 0x007e50c0
// RBX::LDraw2Lua::`dynamic initializer for 'g_unk0x00955330''
// SYNTHETIC: ROBLOX 0x007e50e0
// RBX::LDraw2Lua::`dynamic initializer for 'g_unk0x00955334''
// SYNTHETIC: ROBLOX 0x007e5100
// RBX::LDraw2Lua::`dynamic initializer for 'g_unk0x00955338''
// SYNTHETIC: ROBLOX 0x007e5120
// RBX::LDraw2Lua::`dynamic initializer for 'g_unk0x0095533c''
// SYNTHETIC: ROBLOX 0x007e5140
// RBX::LDraw2Lua::`dynamic initializer for 'g_unk0x00955340''
// SYNTHETIC: ROBLOX 0x007e5160
// RBX::LDraw2Lua::`dynamic initializer for 'g_unk0x00955344''
// SYNTHETIC: ROBLOX 0x007e5180
// RBX::LDraw2Lua::`dynamic initializer for 'g_unk0x00955348''
// SYNTHETIC: ROBLOX 0x007e51a0
// RBX::LDraw2Lua::`dynamic initializer for 'g_unk0x0095534c''
// SYNTHETIC: ROBLOX 0x007e51c0
// RBX::LDraw2Lua::`dynamic initializer for 'g_unk0x00955350''
// SYNTHETIC: ROBLOX 0x007e51e0
// RBX::LDraw2Lua::`dynamic initializer for 'g_unk0x00955354''
// SYNTHETIC: ROBLOX 0x007e5200
// RBX::LDraw2Lua::`dynamic initializer for 'g_unk0x00955358''
// clang-format on

#endif // LDRAW2ROBLOXMAPROOT_H
