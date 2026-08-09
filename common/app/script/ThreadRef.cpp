#include "script/ThreadRef.h"

#include <lauxlib.h>
#include <lua.h>

namespace RBX {
namespace Lua {

// FUNCTION: WEBSERVICE 0x100b1340
void ThreadRef::removeRef()
{
	if (L != NULL) {
		luaL_unref(L, LUA_REGISTRYINDEX, threadId);

		threadId = 0;
		L = NULL;
	}
}

// FUNCTION: WEBSERVICE 0x100b13d0
void lua_pushfunction(lua_State* L, const FunctionRef& ref)
{
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref.functionId);
}

} // namespace Lua
} // namespace RBX
