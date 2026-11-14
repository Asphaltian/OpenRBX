#ifndef LUA_LUA_HPP
#define LUA_LUA_HPP

#include "placeholder.h"

#include <lua.h>

namespace RBX {
namespace Lua {

float lua_tofloat(lua_State* L, int index);

class ScopedPopper
{
	int popCount;
	lua_State* thread;

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

	~ScopedPopper() { lua_pop(thread, popCount); }
};

} // namespace Lua
} // namespace RBX

#endif // LUA_LUA_HPP
