#ifndef SCRIPT_LUAATOMICCLASSES_H
#define SCRIPT_LUAATOMICCLASSES_H

#include "decomp.h"
#include "lua/LuaBridge.h"

#include <G3D/Color3.h>
#include <G3D/Vector3.h>

namespace RBX {
namespace Lua {

// SIZE 0x1
class Color3Bridge : public Bridge<G3D::Color3, 1>
{
public:
	static void registerClassLibrary(lua_State* L);

	static void pushColor3(lua_State* L, G3D::Color3 value);

private:
	static int newColor3(lua_State* L);

	static const luaL_Reg classLibrary[];
};

// SIZE 0x1
class Vector3Bridge : public Bridge<G3D::Vector3, 1>
{
public:
	static void registerClassLibrary(lua_State* L);

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

} // namespace Lua
} // namespace RBX

#endif // SCRIPT_LUAATOMICCLASSES_H
