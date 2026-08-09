#ifndef LUA_LUABRIDGE_H
#define LUA_LUABRIDGE_H

#include <G3D/format.h>
#include <boost/shared_ptr.hpp>
#include <lauxlib.h>
#include <lua.h>
#include <new>
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

	template <class U>
	static U* pushNewObject(lua_State* L, U value);

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
	static int on_tostring(const T& value, lua_State* L);

	static int on_gc(lua_State* L);
	static int on_eq(lua_State* L);

	static const char* className;
};

template <class T, int Tag>
T& Bridge<T, Tag>::pushNewObject(lua_State* L)
{
	T* object = static_cast<T*>(lua_newuserdata(L, sizeof(T)));

	new (object) T();

	lua_getfield(L, LUA_REGISTRYINDEX, className);
	lua_setmetatable(L, -2);

	return *object;
}

// clang-format off
// TEMPLATE: WEBSERVICE 0x1005f1b0
// ??$pushNewObject@VVector3@G3D@@@?$Bridge@VVector3@G3D@@$00@Lua@RBX@@SAPAVVector3@G3D@@PAUlua_State@@V23@@Z
// TEMPLATE: WEBSERVICE 0x1005f250
// ??$pushNewObject@VBrickColor@RBX@@@?$Bridge@VBrickColor@RBX@@$00@Lua@RBX@@SAPAVBrickColor@2@PAUlua_State@@V32@@Z
// TEMPLATE: WEBSERVICE 0x10060550
// ??$pushNewObject@V?$shared_ptr@VDescribedBase@Reflection@RBX@@@boost@@@?$Bridge@V?$shared_ptr@VDescribedBase@Reflection@RBX@@@boost@@$0A@@Lua@RBX@@SAPAV?$shared_ptr@VDescribedBase@Reflection@RBX@@@boost@@PAUlua_State@@V34@@Z
// TEMPLATE: WEBSERVICE 0x100afbe0
// ??$pushNewObject@Vconnection@signals@boost@@@?$Bridge@Vconnection@signals@boost@@$00@Lua@RBX@@SAPAVconnection@signals@boost@@PAUlua_State@@V345@@Z
// clang-format on
template <class T, int Tag>
template <class U>
U* Bridge<T, Tag>::pushNewObject(lua_State* L, U value)
{
	U* object = static_cast<U*>(lua_newuserdata(L, sizeof(U)));

	new (object) U(value);

	lua_getfield(L, LUA_REGISTRYINDEX, className);
	lua_setmetatable(L, -2);

	return object;
}

// clang-format off
// TEMPLATE: WEBSERVICE 0x100ab220
// ??$getValue@V?$shared_ptr@VDescribedBase@Reflection@RBX@@@boost@@@?$Bridge@V?$shared_ptr@VDescribedBase@Reflection@RBX@@@boost@@$0A@@Lua@RBX@@SA_NPAUlua_State@@IAAV?$shared_ptr@VDescribedBase@Reflection@RBX@@@boost@@@Z
// TEMPLATE: WEBSERVICE 0x100adbf0
// ??$getValue@VVector3@G3D@@@?$Bridge@VVector3@G3D@@$00@Lua@RBX@@KA_NPAUlua_State@@IAAVVector3@G3D@@@Z
// clang-format on
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

template <class T, int Tag>
int Bridge<T, Tag>::on_gc(lua_State* L)
{
	getObject(L, 1).~T();

	return 0;
}

template <class T, int Tag>
int Bridge<T, Tag>::on_tostring(lua_State* L)
{
	return on_tostring(getObject(L, 1), L);
}

// clang-format off
// STUB: WEBSERVICE 0x1005ffb0
// ?on_newindex@?$Bridge@Vconnection@signals@boost@@$00@Lua@RBX@@KAHPAUlua_State@@@Z
// STUB: WEBSERVICE 0x10060360
// ?on_newindex@?$Bridge@V?$shared_ptr@VSignalInstance@Reflection@RBX@@@boost@@$0A@@Lua@RBX@@KAHPAUlua_State@@@Z
// clang-format on
template <class T, int Tag>
int Bridge<T, Tag>::on_newindex(lua_State* L)
{
	const char* name = luaL_checkstring(L, 2);
	T& value = getObject(L, 1);

	on_newindex(value, name, L);

	return 0;
}

// clang-format off
// TEMPLATE: WEBSERVICE 0x1005ffe0
// ?on_index@?$Bridge@Vconnection@signals@boost@@$00@Lua@RBX@@KAHPAUlua_State@@@Z
// TEMPLATE: WEBSERVICE 0x10060390
// ?on_index@?$Bridge@V?$shared_ptr@VSignalInstance@Reflection@RBX@@@boost@@$0A@@Lua@RBX@@KAHPAUlua_State@@@Z
// clang-format on
template <class T, int Tag>
int Bridge<T, Tag>::on_index(lua_State* L)
{
	const char* name = luaL_checkstring(L, 2);

	return on_index(getObject(L, 1), name, L);
}

// SIZE 0x1
template <class T>
class SharedPtrBridge : protected Bridge<boost::shared_ptr<T>, 0>
{
public:
	static void registerClass(lua_State* L);
	static void registerClassLibrary(lua_State* L);

	static void push(lua_State* L, boost::shared_ptr<T> instance);

	static boost::shared_ptr<T> getPtr(lua_State* L, unsigned int index);

	template <class U>
	static bool getPtr(lua_State* L, unsigned int index, U& value);
};

// clang-format off
// TEMPLATE: WEBSERVICE 0x100612f0
// ?push@?$SharedPtrBridge@VDescribedBase@Reflection@RBX@@@Lua@RBX@@SAXPAUlua_State@@V?$shared_ptr@VDescribedBase@Reflection@RBX@@@boost@@@Z
// TEMPLATE: WEBSERVICE 0x10061640
// ?push@?$SharedPtrBridge@VSignalInstance@Reflection@RBX@@@Lua@RBX@@SAXPAUlua_State@@V?$shared_ptr@VSignalInstance@Reflection@RBX@@@boost@@@Z
// clang-format on
template <class T>
void SharedPtrBridge<T>::push(lua_State* L, boost::shared_ptr<T> instance)
{
	if (instance.get() == NULL) {
		lua_pushnil(L);
	}
	else {
		lua_gettop(L);

		lua_pushlightuserdata(L, (void*) push);
		lua_rawget(L, LUA_REGISTRYINDEX);

		lua_pushlightuserdata(L, instance.get());
		lua_rawget(L, -2);

		if (lua_type(L, -1) == LUA_TNIL) {
			lua_settop(L, -2);

			SharedPtrBridge<T>::pushNewObject(L, instance);

			lua_pushlightuserdata(L, instance.get());
			lua_pushvalue(L, -2);
			lua_rawset(L, -4);
		}

		lua_remove(L, -2);
	}
}

// clang-format off
// TEMPLATE: WEBSERVICE 0x100ab1b0
// ?getPtr@?$SharedPtrBridge@VDescribedBase@Reflection@RBX@@@Lua@RBX@@SA?AV?$shared_ptr@VDescribedBase@Reflection@RBX@@@boost@@PAUlua_State@@I@Z
// clang-format on
template <class T>
boost::shared_ptr<T> SharedPtrBridge<T>::getPtr(lua_State* L, unsigned int index)
{
	if (lua_type(L, index) == LUA_TNIL) {
		return boost::shared_ptr<T>();
	}

	return getObject(L, index);
}

} // namespace Lua
} // namespace RBX

#endif // LUA_LUABRIDGE_H
