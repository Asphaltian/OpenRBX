#ifndef LUA_LUA_H
#define LUA_LUA_H

struct lua_State;

namespace RBX {
namespace Lua {

float lua_tofloat(lua_State* L, int index);

} // namespace Lua
} // namespace RBX

#endif // LUA_LUA_H
