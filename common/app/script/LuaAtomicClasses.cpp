#include "lua/LuaBridge.h"
#include "lua/lua.h"

#include <G3D/Color3.h>
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

template void Bridge<G3D::Color3, 1>::on_newindex(G3D::Color3& value, const char* name, lua_State* L);

} // namespace Lua
} // namespace RBX
