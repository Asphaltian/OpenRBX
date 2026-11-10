#ifndef LUAWRITER_H
#define LUAWRITER_H

#include "LuaObjectWriter.h"
#include "decomp.h"

#include <ostream>
#include <string>
#include <vector>

namespace RBX {
namespace LDraw2Lua {

class LDrawCommand;

// VTABLE: ROBLOX 0x00810198
// SIZE 0x30
class LuaWriter
{
public:
	LuaWriter(const char* p_unk0x04);

	virtual ~LuaWriter();

	// SYNTHETIC: ROBLOX 0x004741f0
	// RBX::LDraw2Lua::LuaWriter::`scalar deleting destructor'

	void FUN_00474020(std::ostream& stream);
	void FUN_00474210(LDrawCommand* p_unk0x04);

	std::string m_unk0x04;                   // 0x04
	std::vector<LuaObjectWriter*> m_unk0x20; // 0x20
};

DECOMP_SIZE_ASSERT(LuaWriter, 0x30)

} // namespace LDraw2Lua
} // namespace RBX

int FUN_004742e0(const std::string& p_unk0x04, const std::string& p_unk0x08);

#endif // LUAWRITER_H
