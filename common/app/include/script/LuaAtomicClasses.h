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

// SIZE 0x1
class BrickColorBridge : public Bridge<RBX::BrickColor, 1>
{
public:
	static void registerClassLibrary(lua_State* L);

private:
	static int newBrickColor(lua_State* L);
	static int randomBrickColor(lua_State* L);

	static const luaL_Reg classLibrary[];
};

// SIZE 0x1
class CoordinateFrameBridge : public Bridge<G3D::CoordinateFrame, 1>
{
public:
	static void registerClassLibrary(lua_State* L);

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

} // namespace Lua
} // namespace RBX

#endif // SCRIPT_LUAATOMICCLASSES_H
