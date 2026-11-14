#ifndef LDRAW2ROBLOXMAPROOT_H
#define LDRAW2ROBLOXMAPROOT_H

#include "LDraw2RobloxColorMap.h"
#include "LDraw2RobloxPartMap.h"
#include "placeholder.h"

#include <map>
#include <string>

namespace RBX {
namespace LDraw2Lua {

class LDraw2RobloxMapRoot
{
public:
	LDraw2RobloxMapRoot(std::string p_unk0x04);

	virtual ~LDraw2RobloxMapRoot();

	int FUN_004711c0(std::string p_unk0x04);
	int FUN_004712c0(std::string p_unk0x04);
	int FUN_00471430(std::string p_unk0x04);
	LDraw2RobloxColorMap* FUN_004716b0(int p_unk0x04);
	int FUN_00471710(int p_unk0x04);
	LDraw2RobloxPartMap* FUN_00471730(const std::string& p_unk0x04);
	LDraw2RobloxPartMap* FUN_00471790(const std::string& p_unk0x04);
	void FUN_00471b90(LDraw2RobloxPartMap* p_unk0x04);

	std::string m_unk0x04;
	std::string m_unk0x20;
	std::map<int, LDraw2RobloxColorMap*> m_unk0x3c;
	std::map<std::string, LDraw2RobloxPartMap*> m_unk0x48;
};

} // namespace LDraw2Lua
} // namespace RBX

#endif // LDRAW2ROBLOXMAPROOT_H
