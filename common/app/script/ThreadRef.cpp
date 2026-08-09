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

void ThreadRef::removeFromNode()
{
	if (node != NULL) {
		if (next != NULL) {
			next->previous = previous;
		}

		if (previous != NULL) {
			previous->next = next;
		}

		if (node->first == this) {
			node->first = next;
		}

		next = NULL;
		previous = NULL;
		node = NULL;
	}
}

// FUNCTION: WEBSERVICE 0x100b13d0
void lua_pushfunction(lua_State* L, const FunctionRef& ref)
{
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref.functionId);
}

// FUNCTION: WEBSERVICE 0x100b13f0
void FunctionRef::removeRef()
{
	if (functionId != 0 && thread() != NULL) {
		luaL_unref(thread(), LUA_REGISTRYINDEX, functionId);

		functionId = 0;
	}

	ThreadRef::removeRef();
}

// FUNCTION: WEBSERVICE 0x100b1810
void ThreadRef::reset()
{
	boost::mutex::scoped_lock lock(*sync);

	removeFromNode();

	removeRef();
}

// FUNCTION: WEBSERVICE 0x100b1ac0
ThreadRef::~ThreadRef()
{
	reset();
}

// FUNCTION: WEBSERVICE 0x100b1bb0
FunctionRef::~FunctionRef()
{
	if (functionId != 0 && thread() != NULL) {
		luaL_unref(thread(), LUA_REGISTRYINDEX, functionId);
	}
}

} // namespace Lua
} // namespace RBX
