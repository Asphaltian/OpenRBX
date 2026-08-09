#include "script/LuaSignalBridge.h"

#include <G3D/format.h>
#include <lua.h>
#include <stdexcept>
#include <string.h>

namespace RBX {
namespace Lua {

template <>
const char* Bridge<boost::signals::connection, 1>::className = "RBXScriptConnection";

template <>
const char* Bridge<boost::shared_ptr<RBX::Reflection::SignalInstance>, 0>::className = "RBXScriptSignal";

template boost::signals::connection* Bridge<boost::signals::connection, 1>::pushNewObject<boost::signals::connection>(
	lua_State* L,
	boost::signals::connection value
);

// FUNCTION: WEBSERVICE 0x100afc80
template <>
void Bridge<boost::shared_ptr<RBX::Reflection::SignalInstance>, 0>::on_newindex(
	boost::shared_ptr<RBX::Reflection::SignalInstance>& value,
	const char* name,
	lua_State* L
)
{
	if (value.get() == NULL) {
		throw std::runtime_error(G3D::format("The %s event has been deleted", name));
	}

	throw std::runtime_error(G3D::format("%s cannot be assigned to", name));
}

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

// STUB: WEBSERVICE 0x100b0f00
int SignalBridge::wait(lua_State* L)
{
	STUB(0x100b0f00);
	return 0;
}

// STUB: WEBSERVICE 0x100b10b0
int SignalBridge::connect(lua_State* L)
{
	STUB(0x100b10b0);
	return 0;
}

// FUNCTION: WEBSERVICE 0x100b11f0
template <>
int Bridge<boost::shared_ptr<RBX::Reflection::SignalInstance>, 0>::on_index(
	const boost::shared_ptr<RBX::Reflection::SignalInstance>& value,
	const char* name,
	lua_State* L
)
{
	if (value.get() == NULL) {
		throw std::runtime_error(G3D::format("The %s event has been deleted", name));
	}

	if (strcmp(name, "connect") == 0) {
		lua_pushcclosure(L, SignalBridge::connect, 0);
		return 1;
	}

	if (strcmp(name, "wait") == 0) {
		lua_pushcclosure(L, SignalBridge::wait, 0);
		return 1;
	}

	if (strcmp(name, "disconnect") == 0) {
		throw std::runtime_error(
			G3D::format("Event:disconnect() has been deprecated. Use connection object returned by connect()")
		);
	}

	throw std::runtime_error(G3D::format("%s is not a valid member", name));
}

} // namespace Lua
} // namespace RBX
