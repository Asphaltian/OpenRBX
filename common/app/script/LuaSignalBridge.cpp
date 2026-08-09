#include "script/LuaSignalBridge.h"

#include <G3D/format.h>
#include <lua.h>
#include <stdexcept>
#include <string.h>

namespace RBX {
namespace Lua {

template <>
const char* Bridge<boost::signals::connection, 1>::className = "RBXScriptConnection";

// FUNCTION: WEBSERVICE 0x100afe30
int SignalConnectionBridge::disconnect(lua_State* L)
{
	getObject(L, 1).disconnect();
	return 0;
}

// FUNCTION: WEBSERVICE 0x100afee0
template <>
int Bridge<boost::signals::connection, 1>::on_index(
	const boost::signals::connection& value,
	const char* name,
	lua_State* L
)
{
	if (strcmp(name, "disconnect") == 0) {
		lua_pushcclosure(L, SignalConnectionBridge::disconnect, 0);
		return 1;
	}

	if (strcmp(name, "connected") == 0) {
		lua_pushboolean(L, value.connected());
		return 1;
	}

	throw std::runtime_error(G3D::format("%s is not a valid member", name));
}

} // namespace Lua
} // namespace RBX
