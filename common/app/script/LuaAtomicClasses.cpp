#include "script/LuaAtomicClasses.h"

#include "decomp.h"
#include "lua/LuaBridge.h"
#include "lua/lua.h"
#include "v8datamodel/BrickColor.h"

#include <G3D/Color3.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/Vector3.h>
#include <G3D/g3dmath.h>
#include <float.h>
#include <limits>
#include <lua.h>
#include <string.h>

namespace RBX {
namespace Lua {

// FUNCTION: WEBSERVICE 0x100ada30
float lua_tofloat(lua_State* L, int index)
{
	double value = lua_tonumber(L, index);

	if (value == std::numeric_limits<double>::infinity()) {
		return std::numeric_limits<float>::infinity();
	}

	if (value == -std::numeric_limits<double>::infinity()) {
		return -std::numeric_limits<float>::infinity();
	}

	if (!(value < 0.0 || value >= 0.0)) {
		return static_cast<float>(value);
	}

	if (value > FLT_MAX) {
		return FLT_MAX;
	}

	if (value < -FLT_MAX) {
		return -FLT_MAX;
	}

	return static_cast<float>(value);
}

template <>
const char* Bridge<G3D::Color3, 1>::className = "Color3";

// FUNCTION: WEBSERVICE 0x100adac0
template <>
int Bridge<G3D::Color3, 1>::on_index(const G3D::Color3& value, const char* name, lua_State* L)
{
	if (strcmp(name, "r") == 0) {
		lua_pushnumber(L, value.r);
		return 1;
	}

	if (strcmp(name, "g") == 0) {
		lua_pushnumber(L, value.g);
		return 1;
	}

	if (strcmp(name, "b") == 0) {
		lua_pushnumber(L, value.b);
		return 1;
	}

	throw std::runtime_error(G3D::format("%s is not a valid member", name));
}

// FUNCTION: WEBSERVICE 0x100add30
int Vector3Bridge::on_add(lua_State* L)
{
	pushNewObject(L, getObject(L, 2) + getObject(L, 1));
	return 1;
}

// FUNCTION: WEBSERVICE 0x100addc0
int Vector3Bridge::on_sub(lua_State* L)
{
	const G3D::Vector3& left = getObject(L, 1);
	const G3D::Vector3& right = getObject(L, 2);

	pushNewObject(L, left - right);
	return 1;
}

// FUNCTION: WEBSERVICE 0x100ae030
int Vector3Bridge::on_unm(lua_State* L)
{
	pushNewObject(L, -getObject(L, 1));
	return 1;
}

// STUB: WEBSERVICE 0x100ae160
static int lerp(lua_State* L)
{
	STUB(0x100ae160);
	return 0;
}

// FUNCTION: WEBSERVICE 0x100ae210
template <>
int Bridge<G3D::Vector3, 1>::on_index(const G3D::Vector3& value, const char* name, lua_State* L)
{
	if (strcmp(name, "x") == 0) {
		lua_pushnumber(L, value.x);
		return 1;
	}

	if (strcmp(name, "y") == 0) {
		lua_pushnumber(L, value.y);
		return 1;
	}

	if (strcmp(name, "z") == 0) {
		lua_pushnumber(L, value.z);
		return 1;
	}

	if (strcmp(name, "unit") == 0) {
		pushNewObject(L, value.unit());
		return 1;
	}

	if (strcmp(name, "magnitude") == 0) {
		lua_pushnumber(L, value.magnitude());
		return 1;
	}

	if (strcmp(name, "lerp") == 0) {
		lua_pushcclosure(L, lerp, 0);
		return 1;
	}

	throw std::runtime_error(G3D::format("%s is not a valid member", name));
}

template <>
const char* Bridge<G3D::Vector3, 1>::className = "Vector3";

template bool Bridge<G3D::Vector3, 1>::getValue<G3D::Vector3>(lua_State* L, unsigned int index, G3D::Vector3& value);

template G3D::Vector3* Bridge<G3D::Vector3, 1>::pushNewObject<G3D::Vector3>(lua_State* L, G3D::Vector3 value);

template G3D::Color3* Bridge<G3D::Color3, 1>::pushNewObject<G3D::Color3>(lua_State* L, G3D::Color3 value);

// FUNCTION: WEBSERVICE 0x100ae790
template <>
int Bridge<RBX::BrickColor, 1>::on_index(const RBX::BrickColor& value, const char* name, lua_State* L)
{
	if (strcmp(name, "number") == 0) {
		lua_pushinteger(L, value.number);
		return 1;
	}

	if (strcmp(name, "Number") == 0) {
		lua_pushinteger(L, value.number);
		return 1;
	}

	if (strcmp(name, "Color") == 0) {
		Bridge<G3D::Color3, 1>::pushNewObject(L, value.color3());
		return 1;
	}

	if (strcmp(name, "r") == 0) {
		lua_pushnumber(L, value.color3().r);
		return 1;
	}

	if (strcmp(name, "g") == 0) {
		lua_pushnumber(L, value.color3().g);
		return 1;
	}

	if (strcmp(name, "b") == 0) {
		lua_pushnumber(L, value.color3().b);
		return 1;
	}

	if (strcmp(name, "name") == 0) {
		lua_pushstring(L, value.name().c_str());
		return 1;
	}

	if (strcmp(name, "Name") == 0) {
		lua_pushstring(L, value.name().c_str());
		return 1;
	}

	throw std::runtime_error(G3D::format("%s is not a valid member", name));
}

template <>
const char* Bridge<RBX::BrickColor, 1>::className = "BrickColor";

template <>
const char* Bridge<G3D::CoordinateFrame, 1>::className = "CFrame";

template bool Bridge<G3D::CoordinateFrame, 1>::getValue<G3D::CoordinateFrame>(
	lua_State* L,
	unsigned int index,
	G3D::CoordinateFrame& value
);

template void Bridge<G3D::Color3, 1>::on_newindex(G3D::Color3& value, const char* name, lua_State* L);

} // namespace Lua
} // namespace RBX
