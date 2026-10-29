#ifndef SCRIPT_LUAATOMICCLASSES_H
#define SCRIPT_LUAATOMICCLASSES_H

#include "decomp.h"
#include "lua/LuaBridge.h"
#include "v8datamodel/BrickColor.h"

#include <G3D/Color3.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/Vector3.h>

namespace RBX {
namespace Lua {

#ifdef __clang__
template <>
const char* Bridge<G3D::Color3, 1>::className;
template <>
const char* Bridge<G3D::Vector3, 1>::className;
template <>
const char* Bridge<RBX::BrickColor, 1>::className;
template <>
const char* Bridge<G3D::CoordinateFrame, 1>::className;
#endif

// SIZE 0x1
class Color3Bridge : public Bridge<G3D::Color3, 1>
{
public:
	static void registerClassLibrary(lua_State* L)
	{
		luaL_register(L, className, classLibrary);
		lua_pop(L, 1);
	}

	static void pushColor3(lua_State* L, G3D::Color3 value);

private:
	static int newColor3(lua_State* L);

	static const luaL_Reg classLibrary[];
};

// SIZE 0x1
class Vector3Bridge : public Bridge<G3D::Vector3, 1>
{
	friend class Bridge<G3D::Vector3, 1>;

public:
	static void registerClassLibrary(lua_State* L)
	{
		luaL_register(L, className, classLibrary);
		lua_pop(L, 1);
	}

	static void pushVector3(lua_State* L, G3D::Vector3 value);

private:
	static int newVector3(lua_State* L);

	static int on_add(lua_State* L);
	static int on_sub(lua_State* L);
	static int on_mul(lua_State* L);
	static int on_div(lua_State* L);
	static int on_unm(lua_State* L);

	static const luaL_Reg classLibrary[];
};

// SIZE 0x1
class BrickColorBridge : public Bridge<RBX::BrickColor, 1>
{
public:
	static void registerClassLibrary(lua_State* L)
	{
		luaL_register(L, className, classLibrary);
		lua_pop(L, 1);
	}

private:
	static int newBrickColor(lua_State* L);
	static int randomBrickColor(lua_State* L);

	static const luaL_Reg classLibrary[];
};

// SIZE 0x1
class CoordinateFrameBridge : public Bridge<G3D::CoordinateFrame, 1>
{
	friend class Bridge<G3D::CoordinateFrame, 1>;

public:
	static void registerClassLibrary(lua_State* L)
	{
		luaL_register(L, className, classLibrary);
		lua_pop(L, 1);
	}

	static void pushCoordinateFrame(lua_State* L, G3D::CoordinateFrame value);

private:
	static int newCoordinateFrame(lua_State* L);
	static int fromEulerAnglesXYZ(lua_State* L);
	static int fromAxisAngle(lua_State* L);

	static int on_add(lua_State* L);
	static int on_sub(lua_State* L);
	static int on_mul(lua_State* L);
	static int on_inverse(lua_State* L);
	static int on_toWorldSpace(lua_State* L);
	static int on_toObjectSpace(lua_State* L);
	static int on_pointToWorldSpace(lua_State* L);
	static int on_pointToObjectSpace(lua_State* L);
	static int on_vectorToWorldSpace(lua_State* L);
	static int on_vectorToObjectSpace(lua_State* L);
	static int on_toEulerAnglesXYZ(lua_State* L);
	static int on_components(lua_State* L);

	static const luaL_Reg classLibrary[];
};

// FUNCTION: WEBSERVICE 0x10060e20
// FUNCTION: ROBLOX 0x0056df80
template <>
inline void Bridge<G3D::Vector3, 1>::registerClass(lua_State* L)
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

	lua_pushstring(L, "__eq");
	lua_pushcfunction(L, on_eq);
	lua_settable(L, -3);

	lua_pushstring(L, "__tostring");
	lua_pushcfunction(L, on_tostring);
	lua_settable(L, -3);

	lua_pushstring(L, "__add");
	lua_pushcfunction(L, Vector3Bridge::on_add);
	lua_settable(L, -3);

	lua_pushstring(L, "__sub");
	lua_pushcfunction(L, Vector3Bridge::on_sub);
	lua_settable(L, -3);

	lua_pushstring(L, "__mul");
	lua_pushcfunction(L, Vector3Bridge::on_mul);
	lua_settable(L, -3);

	lua_pushstring(L, "__div");
	lua_pushcfunction(L, Vector3Bridge::on_div);
	lua_settable(L, -3);

	lua_pushstring(L, "__unm");
	lua_pushcfunction(L, Vector3Bridge::on_unm);
	lua_settable(L, -3);

	lua_pop(L, 1);
}

// FUNCTION: WEBSERVICE 0x10060f90
// FUNCTION: ROBLOX 0x0056e0f0
template <>
inline void Bridge<G3D::CoordinateFrame, 1>::registerClass(lua_State* L)
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

	lua_pushstring(L, "__eq");
	lua_pushcfunction(L, on_eq);
	lua_settable(L, -3);

	lua_pushstring(L, "__tostring");
	lua_pushcfunction(L, on_tostring);
	lua_settable(L, -3);

	lua_pushstring(L, "__add");
	lua_pushcfunction(L, CoordinateFrameBridge::on_add);
	lua_settable(L, -3);

	lua_pushstring(L, "__sub");
	lua_pushcfunction(L, CoordinateFrameBridge::on_sub);
	lua_settable(L, -3);

	lua_pushstring(L, "__mul");
	lua_pushcfunction(L, CoordinateFrameBridge::on_mul);
	lua_settable(L, -3);

	lua_pushstring(L, "inverse");
	lua_pushcfunction(L, CoordinateFrameBridge::on_inverse);
	lua_settable(L, -3);

	lua_pop(L, 1);
}

} // namespace Lua
} // namespace RBX

#endif // SCRIPT_LUAATOMICCLASSES_H
