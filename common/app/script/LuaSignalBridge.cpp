#include "script/LuaSignalBridge.h"

#include "reflection/signal.h"
#include "script/Script.h"
#include "script/ScriptContext.h"
#include "script/ThreadRef.h"

#include <G3D/format.h>
#include <boost/any.hpp>
#include <lua.h>
#include <stdexcept>
#include <string.h>
#include <vector>

// SIZE 0x28
class WaitScriptSlot : public RBX::Script::Slot
{
public:
	WaitScriptSlot(lua_State* L);

	void operator()(const std::vector<boost::any>& arguments);

private:
	RBX::Lua::ThreadRef waitThread; // 0x08
};

DECOMP_SIZE_ASSERT(WaitScriptSlot, 0x28)

// SIZE 0x54
class FunctionScriptSlot : public RBX::Script::Slot
{
public:
	FunctionScriptSlot(lua_State* L, int index);

	void operator()(const std::vector<boost::any>& arguments);

private:
	RBX::ScriptContext& context;          // 0x08
	RBX::Lua::FunctionRef function;       // 0x0c
	RBX::Lua::ThreadRef cachedSlotThread; // 0x30
	int executionDepth;                   // 0x50
};

DECOMP_SIZE_ASSERT(FunctionScriptSlot, 0x54)

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

} // namespace Lua
} // namespace RBX

void WaitScriptSlot::operator()(const std::vector<boost::any>& arguments)
{
	cnction->disconnect();
}

// STUB: WEBSERVICE 0x100b0920
void FunctionScriptSlot::operator()(const std::vector<boost::any>& arguments)
{
	STUB(0x100b0920);
}

// STUB: WEBSERVICE 0x100b0cb0
FunctionScriptSlot::FunctionScriptSlot(lua_State* L, int index)
	: context(RBX::ScriptContext::getContext(L)), function(L, index), executionDepth(0)
{
}

// STUB: WEBSERVICE 0x100b0d80
WaitScriptSlot::WaitScriptSlot(lua_State* L) : waitThread(L)
{
}

namespace RBX {
namespace Lua {

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

template RBX::Reflection::GenericSlotWrapper* RBX::Reflection::GenericSlotWrapper::create<WaitScriptSlot>(
	WaitScriptSlot slot
);

template RBX::Reflection::GenericSlotWrapper* RBX::Reflection::GenericSlotWrapper::create<FunctionScriptSlot>(
	FunctionScriptSlot slot
);
