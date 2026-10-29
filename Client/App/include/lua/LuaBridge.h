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
// TEMPLATE: WEBSERVICE 0x10061220
// RBX::Lua::Bridge<boost::signals::connection,1>::registerClass
// TEMPLATE: WEBSERVICE 0x10061410
// RBX::Lua::Bridge<G3D::Color3,1>::registerClass
// TEMPLATE: WEBSERVICE 0x100614e0
// RBX::Lua::Bridge<RBX::BrickColor,1>::registerClass
// TEMPLATE: WEBSERVICE 0x10062680
// RBX::Lua::Bridge<boost::shared_ptr<RBX::Lua::ThreadRef::Node>,1>::registerClass
// TEMPLATE: WEBSERVICE 0x10062750
// RBX::Lua::Bridge<boost::shared_ptr<RBX::Reflection::SignalInstance>,0>::registerClass
// TEMPLATE: WEBSERVICE 0x10064ad0
// RBX::Lua::Bridge<boost::shared_ptr<RBX::Reflection::DescribedBase>,0>::registerClass
// clang-format on
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
U* Bridge<T, Tag>::pushNewObject(lua_State* L, U param1)
{
	U* data = static_cast<U*>(lua_newuserdata(L, sizeof(U)));

	new (data) U(param1);

	lua_getfield(L, LUA_REGISTRYINDEX, className);
	lua_setmetatable(L, -2);

	return data;
}

// clang-format off
// TEMPLATE: WEBSERVICE 0x100630c0
// ??$getValue@VValue@Reflection@RBX@@@?$Bridge@VCoordinateFrame@G3D@@$00@Lua@RBX@@SA_NPAUlua_State@@IAAVValue@Reflection@2@@Z
// TEMPLATE: WEBSERVICE 0x10063140
// ??$getValue@VValue@Reflection@RBX@@@?$Bridge@VVector3@G3D@@$00@Lua@RBX@@SA_NPAUlua_State@@IAAVValue@Reflection@2@@Z
// TEMPLATE: WEBSERVICE 0x100631c0
// ??$getValue@VValue@Reflection@RBX@@@?$Bridge@VColor3@G3D@@$00@Lua@RBX@@SA_NPAUlua_State@@IAAVValue@Reflection@2@@Z
// TEMPLATE: WEBSERVICE 0x10063240
// ??$getValue@VValue@Reflection@RBX@@@?$Bridge@VBrickColor@RBX@@$00@Lua@RBX@@SA_NPAUlua_State@@IAAVValue@Reflection@2@@Z
// TEMPLATE: WEBSERVICE 0x10063550
// ??$getValue@VValue@Reflection@RBX@@@?$Bridge@V?$shared_ptr@VDescribedBase@Reflection@RBX@@@boost@@$0A@@Lua@RBX@@SA_NPAUlua_State@@IAAVValue@Reflection@2@@Z
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

// clang-format off
// TEMPLATE: WEBSERVICE 0x1005fd30
// ?on_gc@?$Bridge@VCoordinateFrame@G3D@@$00@Lua@RBX@@KAHPAUlua_State@@@Z
// TEMPLATE: WEBSERVICE 0x1005fe50
// ?on_gc@?$Bridge@VVector3@G3D@@$00@Lua@RBX@@KAHPAUlua_State@@@Z
// TEMPLATE: WEBSERVICE 0x1005ff60
// ?on_gc@?$Bridge@Vconnection@signals@boost@@$00@Lua@RBX@@KAHPAUlua_State@@@Z
// TEMPLATE: WEBSERVICE 0x100600f0
// ?on_gc@?$Bridge@VColor3@G3D@@$00@Lua@RBX@@KAHPAUlua_State@@@Z
// TEMPLATE: WEBSERVICE 0x10060200
// ?on_gc@?$Bridge@VBrickColor@RBX@@$00@Lua@RBX@@KAHPAUlua_State@@@Z
// clang-format on
template <class T, int Tag>
int Bridge<T, Tag>::on_gc(lua_State* L)
{
	getObject(L, 1).~T();

	return 0;
}

// clang-format off
// TEMPLATE: WEBSERVICE 0x1005edb0
// ?on_tostring@?$Bridge@VVector3@G3D@@$00@Lua@RBX@@KAHABVVector3@G3D@@PAUlua_State@@@Z
// clang-format on
template <class T, int Tag>
int Bridge<T, Tag>::on_tostring(const T& value, lua_State* L)
{
	std::string text = StringConverter<T>::convertToString(value);

	lua_pushstring(L, text.c_str());

	return 1;
}

// clang-format off
// TEMPLATE: WEBSERVICE 0x1005fe70
// ?on_tostring@?$Bridge@VVector3@G3D@@$00@Lua@RBX@@KAHPAUlua_State@@@Z
// clang-format on
template <class T, int Tag>
int Bridge<T, Tag>::on_tostring(lua_State* L)
{
	return on_tostring(getObject(L, 1), L);
}

// clang-format off
// FUNCTION: WEBSERVICE 0x1005ffb0
// ?on_newindex@?$Bridge@Vconnection@signals@boost@@$00@Lua@RBX@@KAHPAUlua_State@@@Z
// FUNCTION: WEBSERVICE 0x10060360
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
// TEMPLATE: WEBSERVICE 0x1005fdd0
// ?on_eq@?$Bridge@VCoordinateFrame@G3D@@$00@Lua@RBX@@KAHPAUlua_State@@@Z
// TEMPLATE: WEBSERVICE 0x1005fef0
// ?on_eq@?$Bridge@VVector3@G3D@@$00@Lua@RBX@@KAHPAUlua_State@@@Z
// TEMPLATE: WEBSERVICE 0x10060010
// ?on_eq@?$Bridge@Vconnection@signals@boost@@$00@Lua@RBX@@KAHPAUlua_State@@@Z
// TEMPLATE: WEBSERVICE 0x10060190
// ?on_eq@?$Bridge@VColor3@G3D@@$00@Lua@RBX@@KAHPAUlua_State@@@Z
// TEMPLATE: WEBSERVICE 0x100602a0
// ?on_eq@?$Bridge@VBrickColor@RBX@@$00@Lua@RBX@@KAHPAUlua_State@@@Z
// clang-format on
template <class T, int Tag>
int Bridge<T, Tag>::on_eq(lua_State* L)
{
	lua_pushboolean(L, getObject(L, 1) == getObject(L, 2));

	return 1;
}

// clang-format off
// TEMPLATE: WEBSERVICE 0x1005fda0
// ?on_index@?$Bridge@VCoordinateFrame@G3D@@$00@Lua@RBX@@KAHPAUlua_State@@@Z
// TEMPLATE: WEBSERVICE 0x1005fec0
// ?on_index@?$Bridge@VVector3@G3D@@$00@Lua@RBX@@KAHPAUlua_State@@@Z
// TEMPLATE: WEBSERVICE 0x1005ffe0
// ?on_index@?$Bridge@Vconnection@signals@boost@@$00@Lua@RBX@@KAHPAUlua_State@@@Z
// TEMPLATE: WEBSERVICE 0x10060160
// ?on_index@?$Bridge@VColor3@G3D@@$00@Lua@RBX@@KAHPAUlua_State@@@Z
// TEMPLATE: WEBSERVICE 0x10060270
// ?on_index@?$Bridge@VBrickColor@RBX@@$00@Lua@RBX@@KAHPAUlua_State@@@Z
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
	static void registerClass(lua_State* L) { Bridge<boost::shared_ptr<T>, 0>::registerClass(L); }
	static void registerClassLibrary(lua_State* L);

	static void push(lua_State* L, boost::shared_ptr<T> instance);

	static boost::shared_ptr<T> getPtr(lua_State* L, unsigned int index);

	template <class U>
	static bool getPtr(lua_State* L, unsigned int index, U& value);
};

// clang-format off
// TEMPLATE: WEBSERVICE 0x10063970
// ??$getPtr@VValue@Reflection@RBX@@@?$SharedPtrBridge@VDescribedBase@Reflection@RBX@@@Lua@RBX@@SA_NPAUlua_State@@IAAVValue@Reflection@2@@Z
// TEMPLATE: WEBSERVICE 0x100ab580
// ??$getPtr@V?$shared_ptr@VDescribedBase@Reflection@RBX@@@boost@@@?$SharedPtrBridge@VDescribedBase@Reflection@RBX@@@Lua@RBX@@SA_NPAUlua_State@@IAAV?$shared_ptr@VDescribedBase@Reflection@RBX@@@boost@@@Z
// clang-format on
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
// TEMPLATE: WEBSERVICE 0x1005f010
// RBX::Lua::Bridge<RBX::BrickColor,1>::getObject
// TEMPLATE: WEBSERVICE 0x1005ecf0
// RBX::Lua::Bridge<G3D::CoordinateFrame,1>::getObject
// TEMPLATE: WEBSERVICE 0x1005ed90
// RBX::Lua::Bridge<G3D::Vector3,1>::getObject
// TEMPLATE: WEBSERVICE 0x1005ef70
// RBX::Lua::Bridge<G3D::Color3,1>::getObject
// TEMPLATE: WEBSERVICE 0x1005f110
// RBX::Lua::Bridge<G3D::CoordinateFrame,1>::pushNewObject<G3D::CoordinateFrame>
// TEMPLATE: WEBSERVICE 0x1005f200
// RBX::Lua::Bridge<G3D::Color3,1>::pushNewObject<G3D::Color3>
// TEMPLATE: WEBSERVICE 0x10060610
// RBX::Lua::Bridge<boost::shared_ptr<RBX::Reflection::SignalInstance>,0>::pushNewObject<boost::shared_ptr<RBX::Reflection::SignalInstance> >
// TEMPLATE: WEBSERVICE 0x100afa60
// RBX::Lua::Bridge<G3D::CoordinateFrame,1>::getValue<G3D::CoordinateFrame>
// clang-format on
template <class T>
boost::shared_ptr<T> SharedPtrBridge<T>::getPtr(lua_State* L, unsigned int index)
{
	if (lua_type(L, index) == LUA_TNIL) {
		return boost::shared_ptr<T>();
	}

	return SharedPtrBridge<T>::getObject(L, index);
}

} // namespace Lua
} // namespace RBX

#endif // LUA_LUABRIDGE_H
