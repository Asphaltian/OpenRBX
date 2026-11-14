#ifndef LUAWRITER_H
#define LUAWRITER_H

#include "LuaObjectWriter.h"
#include "placeholder.h"

#include <ostream>
#include <string>
#include <vector>

namespace RBX {
namespace LDraw2Lua {

class LDrawCommand;

class LuaWriter
{
public:
	LuaWriter(const char* p_unk0x04);

	virtual ~LuaWriter();

	void FUN_00474020(std::ostream& stream);
	void FUN_00474210(LDrawCommand* p_unk0x04);

	std::string m_unk0x04;
	std::vector<LuaObjectWriter*> m_unk0x20;
};

} // namespace LDraw2Lua
} // namespace RBX

int FUN_004742e0(const std::string& p_unk0x04, const std::string& p_unk0x08);

#endif // LUAWRITER_H
