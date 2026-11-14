#ifndef LUA_LUABRIDGE_H
#define LUA_LUABRIDGE_H

#include "util/Utilities.h"

#include <G3D/format.h>
#include <boost/shared_ptr.hpp>
#include <lauxlib.h>
#include <lua.h>
#include <new>
#include <stdexcept>

struct lua_State;

namespace RBX {
namespace Lua {

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

template <class T, int Tag>
void Bridge<T, Tag>::registerClass(lua_State* L)
{
	luaL_newmetatable(L, className);

	lua_pushstring(L, "__index");
	lua_pushcfunction(L, on_index);
	lua_settable(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, on_newindex);
	lua_settable(L, -3);

	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, on_gc);
	lua_settable(L, -3);

	if (Tag == 1) {
		lua_pushstring(L, "__eq");
		lua_pushcfunction(L, on_eq);
		lua_settable(L, -3);
	}

	lua_pushstring(L, "__tostring");
	lua_pushcfunction(L, on_tostring);
	lua_settable(L, -3);

	lua_pop(L, 1);
}

template <class T, int Tag>
template <class U>
U* Bridge<T, Tag>::pushNewObject(lua_State* L, U param1)
{
	U* data = static_cast<U*>(lua_newuserdata(L, sizeof(U)));

	new (data) U(param1);

	lua_getfield(L, LUA_REGISTRYINDEX, className);
	lua_setmetatable(L, -2);

	return data;
}

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

				value = *static_cast<const T*>(userdata);
				return true;
			}
		}
		else {
			lua_settop(L, -2);
		}
	}

	return false;
}

template <class T, int Tag>
int Bridge<T, Tag>::on_gc(lua_State* L)
{
	getObject(L, 1).~T();

	return 0;
}

template <class T, int Tag>
int Bridge<T, Tag>::on_tostring(const T& value, lua_State* L)
{
	std::string text = StringConverter<T>::convertToString(value);

	lua_pushstring(L, text.c_str());

	return 1;
}

template <class T, int Tag>
int Bridge<T, Tag>::on_tostring(lua_State* L)
{
	return on_tostring(getObject(L, 1), L);
}

template <class T, int Tag>
int Bridge<T, Tag>::on_newindex(lua_State* L)
{
	const char* name = luaL_checkstring(L, 2);
	T& value = getObject(L, 1);

	on_newindex(value, name, L);

	return 0;
}

template <class T, int Tag>
int Bridge<T, Tag>::on_eq(lua_State* L)
{
	lua_pushboolean(L, getObject(L, 1) == getObject(L, 2));

	return 1;
}

template <class T, int Tag>
int Bridge<T, Tag>::on_index(lua_State* L)
{
	const char* name = luaL_checkstring(L, 2);

	return on_index(getObject(L, 1), name, L);
}

template <class T>
class SharedPtrBridge : protected Bridge<boost::shared_ptr<T>, 0>
{
public:
	static void registerClass(lua_State* L) { Bridge<boost::shared_ptr<T>, 0>::registerClass(L); }
	static void registerClassLibrary(lua_State* L);

	static void push(lua_State* L, boost::shared_ptr<T> instance);

	static boost::shared_ptr<T> getPtr(lua_State* L, unsigned int index);

	template <class U>
	static bool getPtr(lua_State* L, unsigned int index, U& value);
};

template <class T>
template <class U>
bool SharedPtrBridge<T>::getPtr(lua_State* L, unsigned int index, U& value)
{
	if (lua_type(L, index) == LUA_TNIL) {
		value = boost::shared_ptr<T>();
		return true;
	}

	return Bridge<boost::shared_ptr<T>, 0>::getValue(L, index, value);
}

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
