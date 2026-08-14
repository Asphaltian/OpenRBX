#ifndef SCRIPT_LUAINSTANCEBRIDGE_H
#define SCRIPT_LUAINSTANCEBRIDGE_H

#include "decomp.h"

#include <boost/shared_ptr.hpp>

struct lua_State;
struct luaL_Reg;

namespace RBX {

class Instance;

namespace Lua {

void newweaktable(lua_State* L, const char* mode);

// SIZE 0x1
class ObjectBridge
{
public:
	static boost::shared_ptr<Instance> getInstance(lua_State* L, unsigned int index);

	static int newInstance(lua_State* L);

	static int callMemberFunction(lua_State* L);

private:
	static const luaL_Reg* const classLibrary;
};

} // namespace Lua
} // namespace RBX

#endif // SCRIPT_LUAINSTANCEBRIDGE_H
