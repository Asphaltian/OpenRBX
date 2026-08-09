#ifndef LUA_LUABRIDGE_H
#define LUA_LUABRIDGE_H

#include <G3D/format.h>
#include <lauxlib.h>
#include <lua.h>
#include <stdexcept>

struct lua_State;

namespace RBX {
namespace Lua {

// SIZE 0x1
template <class T, int Tag>
class Bridge
{
public:
	static T& pushNewObject(lua_State* L);

	static T& getObject(lua_State* L, unsigned int index)
	{
		return *static_cast<T*>(luaL_checkudata(L, index, className));
	}

	template <class U>
	static bool getValue(lua_State* L, unsigned int index, U& value);

	static void registerClass(lua_State* L);

protected:
	static int on_index(lua_State* L);
	static int on_index(const T& value, const char* name, lua_State* L);

	static int on_newindex(lua_State* L);
	static void on_newindex(T& value, const char* name, lua_State* L);

	static int on_tostring(lua_State* L);

	static int on_gc(lua_State* L);
	static int on_eq(lua_State* L);

	static const char* className;
};

// TEMPLATE: WEBSERVICE 0x100adbf0
// ??$getValue@VVector3@G3D@@@?$Bridge@VVector3@G3D@@$00@Lua@RBX@@KA_NPAUlua_State@@IAAVVector3@G3D@@@Z
template <class T, int Tag>
template <class U>
bool Bridge<T, Tag>::getValue(lua_State* L, unsigned int index, U& value)
{
	const void* userdata = lua_touserdata(L, index);

	if (userdata != NULL) {
		if (lua_getmetatable(L, index)) {
			lua_getfield(L, LUA_REGISTRYINDEX, className);

			if (lua_rawequal(L, -1, -2)) {
				lua_settop(L, -3);

				value = *static_cast<const U*>(userdata);
				return true;
			}
		}
		else {
			lua_settop(L, -2);
		}
	}

	return false;
}

// TEMPLATE: WEBSERVICE 0x100adb90
// ?on_newindex@?$Bridge@VColor3@G3D@@$00@Lua@RBX@@KAXAAVColor3@G3D@@PBDPAUlua_State@@@Z
template <class T, int Tag>
void Bridge<T, Tag>::on_newindex(T& value, const char* name, lua_State* L)
{
	throw std::runtime_error(G3D::format("%s cannot be assigned to", name));
}

} // namespace Lua
} // namespace RBX

#endif // LUA_LUABRIDGE_H
