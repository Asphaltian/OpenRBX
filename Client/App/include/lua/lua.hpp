#ifndef LUA_LUA_HPP
#define LUA_LUA_HPP

#include "decomp.h"

#include <lua.h>

namespace RBX {
namespace Lua {

float lua_tofloat(lua_State* L, int index);

// SIZE 0x8
class ScopedPopper
{
	int popCount;      // 0x00
	lua_State* thread; // 0x04

public:
	ScopedPopper(lua_State* thread, int popCount) : popCount(popCount), thread(thread) {}

	ScopedPopper& operator+=(int count)
	{
		popCount += count;
		return *this;
	}

	ScopedPopper& operator-=(int count)
	{
		popCount -= count;
		return *this;
	}

	// FUNCTION: WEBSERVICE 0x1005ea80
	~ScopedPopper() { lua_pop(thread, popCount); }
};

DECOMP_SIZE_ASSERT(ScopedPopper, 0x8)

} // namespace Lua
} // namespace RBX

#endif // LUA_LUA_HPP
