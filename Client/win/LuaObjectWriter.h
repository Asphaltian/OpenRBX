#ifndef LUAOBJECTWRITER_H
#define LUAOBJECTWRITER_H

#include "placeholder.h"

#include <ostream>
#include <string>

namespace RBX {
namespace LDraw2Lua {

class LuaObjectWriter
{
public:
	virtual ~LuaObjectWriter();
	virtual bool VTable0x04(std::ostream& stream, std::string p_unk0x08) = 0;
};

} // namespace LDraw2Lua
} // namespace RBX

#endif // LUAOBJECTWRITER_H
