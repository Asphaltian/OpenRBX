#ifndef SCRIPT_LUASIGNALBRIDGE_H
#define SCRIPT_LUASIGNALBRIDGE_H

#include "decomp.h"
#include "lua/LuaBridge.h"
#include "reflection/signal.h"

#include <boost/signals/connection.hpp>

namespace RBX {
namespace Lua {

template <>
int Bridge<boost::signals::connection, 1>::on_index(
	const boost::signals::connection& value,
	const char* name,
	lua_State* L
);

template <>
void Bridge<boost::signals::connection, 1>::on_newindex(
	boost::signals::connection& value,
	const char* name,
	lua_State* L
);

template <>
inline int Bridge<boost::signals::connection, 1>::on_tostring(const boost::signals::connection& value, lua_State* L)
{
	lua_pushstring(L, "Connection");
	return 1;
}

template <>
int Bridge<boost::shared_ptr<RBX::Reflection::SignalInstance>, 0>::on_index(
	const boost::shared_ptr<RBX::Reflection::SignalInstance>& value,
	const char* name,
	lua_State* L
);

template <>
void Bridge<boost::shared_ptr<RBX::Reflection::SignalInstance>, 0>::on_newindex(
	boost::shared_ptr<RBX::Reflection::SignalInstance>& value,
	const char* name,
	lua_State* L
);

// FUNCTION: WEBSERVICE 0x1005f7f0
template <>
inline int Bridge<boost::shared_ptr<RBX::Reflection::SignalInstance>, 0>::on_tostring(
	const boost::shared_ptr<RBX::Reflection::SignalInstance>& value,
	lua_State* L
)
{
	std::string result("Signal ");
	result += value->descriptor.name.toString();
	lua_pushstring(L, result.c_str());
	return 1;
}

// SIZE 0x1
class SignalConnectionBridge : public Bridge<boost::signals::connection, 1>
{
	friend class Bridge<boost::signals::connection, 1>;

private:
	static int disconnect(lua_State* L);
};

// SIZE 0x1
class SignalBridge : public Bridge<boost::shared_ptr<RBX::Reflection::SignalInstance>, 0>
{
public:
	static int connect(lua_State* L);
	static int wait(lua_State* L);
};

} // namespace Lua
} // namespace RBX

#endif // SCRIPT_LUASIGNALBRIDGE_H
