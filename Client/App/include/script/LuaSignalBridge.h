#ifndef SCRIPT_LUASIGNALBRIDGE_H
#define SCRIPT_LUASIGNALBRIDGE_H

#include "decomp.h"
#include "lua/LuaBridge.h"
#include "reflection/signal.h"

#include <boost/signals/connection.hpp>

namespace RBX {
namespace Lua {

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
