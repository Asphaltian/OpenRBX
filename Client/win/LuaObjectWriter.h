#ifndef LUAOBJECTWRITER_H
#define LUAOBJECTWRITER_H

#include "decomp.h"

#include <ostream>
#include <string>

namespace RBX {
namespace LDraw2Lua {

// VTABLE: ROBLOX 0x0080ffd0
// SIZE 0x04
class LuaObjectWriter
{
public:
	virtual ~LuaObjectWriter();                                               // vtable+0x00
	virtual bool VTable0x04(std::ostream& stream, std::string p_unk0x08) = 0; // vtable+0x04

	// SYNTHETIC: ROBLOX 0x004731c0
	// RBX::LDraw2Lua::LuaObjectWriter::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(LuaObjectWriter, 0x04)

} // namespace LDraw2Lua
} // namespace RBX

#endif // LUAOBJECTWRITER_H
