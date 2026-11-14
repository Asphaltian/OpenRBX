#ifndef LDRAW2ROBLOXPARTMAP_H
#define LDRAW2ROBLOXPARTMAP_H

#include "placeholder.h"

#include <string>
#include <vector>

namespace RBX {
namespace LDraw2Lua {

struct Unknown00473110
{
	Unknown00473110(int p_unk0x04, int p_unk0x08, std::string p_unk0x0c, std::string p_unk0x28);

	int m_unk0x00;
	int m_unk0x04;
	std::string m_unk0x08;
	std::string m_unk0x24;
};

class LDraw2RobloxPartMap
{
public:
	LDraw2RobloxPartMap(std::string p_unk0x04, std::string p_unk0x20, std::string p_unk0x3c, int p_unk0x58);

	virtual ~LDraw2RobloxPartMap();

	std::string FUN_00471130() const { return m_unk0x3c; }

	const Unknown00473110* FUN_00472ae0() const;
	void FUN_00473100(const Unknown00473110& p_unk0x04);

	std::string m_unk0x04;
	std::string m_unk0x20;
	std::string m_unk0x3c;
	int m_unk0x58;
	std::vector<Unknown00473110> m_unk0x5c;
};

} // namespace LDraw2Lua
} // namespace RBX

#endif // LDRAW2ROBLOXPARTMAP_H
