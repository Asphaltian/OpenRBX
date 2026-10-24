#include "script/ScriptContext.h"

#include "reflection/signal.h"
#include "script/LuaAtomicClasses.h"
#include "script/LuaMemory.h"
#include "script/Script.h"
#include "script/ScriptEvent.h"
#include "security/SecurityContext.h"
#include "util/ContentProvider.h"
#include "util/Name.h"
#include "util/StandardOut.h"
#include "v8datamodel/DebugSettings.h"
#include "v8datamodel/Stats.h"

#include <algorithm>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <lualib.h>

static int contextCount;

namespace RBX {

char sScriptContext[] = "ScriptContext";

namespace Lua {

template <>
const char* Bridge<boost::shared_ptr<Reflection::DescribedBase>, 0>::className = "Object";

} // namespace Lua

// FUNCTION: WEBSERVICE 0x1005eb00
ScriptContext& ScriptContext::getContext(lua_State* L)
{
	lua_pushlightuserdata(L, (void*) 0x43);
	lua_gettable(L, LUA_GLOBALSINDEX);

	ScriptContext* context = static_cast<ScriptContext*>(lua_touserdata(L, -1));

	lua_settop(L, -2);

	return *context;
}

// STUB: WEBSERVICE 0x1005eb60
void ScriptContext::onEvent(const RunService* source, RunTransition event)
{
	STUB(0x1005eb60);
}

// FUNCTION: WEBSERVICE 0x1005f880
void Lua::CoordinateFrameBridge::pushCoordinateFrame(lua_State* L, G3D::CoordinateFrame value)
{
	pushNewObject(L, value);
}

// FUNCTION: WEBSERVICE 0x1005f8c0
void Lua::Vector3Bridge::pushVector3(lua_State* L, G3D::Vector3 value)
{
	pushNewObject(L, value);
}

// FUNCTION: WEBSERVICE 0x1005f910
void Lua::Color3Bridge::pushColor3(lua_State* L, G3D::Color3 value)
{
	pushNewObject(L, value);
}

// FUNCTION: WEBSERVICE 0x1005f9a0
unsigned int ScriptContext::getThreadCount() const
{
	return globalState ? RobloxExtraSpace::get(globalState)->getThreadCount() : 0;
}

// FUNCTION: WEBSERVICE 0x100644e0
void ScriptContext::disassociateState(Script* script)
{
	if (script->association().contains<boost::shared_ptr<Lua::ThreadRef::Node> >()) {
		script->association().get<boost::shared_ptr<Lua::ThreadRef::Node> >()->eraseAllRefs();
		script->association().remove<boost::shared_ptr<Lua::ThreadRef::Node> >();
	}
}

// FUNCTION: WEBSERVICE 0x10064510
void ScriptContext::removeScript(Script* script)
{
	if (scripts.erase(script)) {
		disassociateState(script);

		std::vector<boost::shared_ptr<Script> >::iterator iter =
			std::find(pendingScripts.begin(), pendingScripts.end(), shared_from(script));
		if (iter != pendingScripts.end()) {
			pendingScripts.erase(iter);
		}
	}
}

// FUNCTION: WEBSERVICE 0x10065f80
void ScriptContext::closeState()
{
	if (!globalState) {
		return;
	}

	commandLineSandbox.reset();

	try {
		Lua::ThreadRef::Node::get(globalState)->eraseAllRefs();

		std::set<Script*> copy(scripts);
		std::for_each(copy.begin(), copy.end(), boost::bind(&ScriptContext::disassociateState, this, _1));

		yieldEvent.reset();
	}
	catch (std::exception& e) {
		StandardOut::singleton()->print(MESSAGE_ERROR, "Exception thrown while cleaning up Lua: %s", e.what());
	}

	lua_close(globalState);
	contextCount--;

	{
		unsigned int heapSize;
		unsigned int heapCount;
		unsigned int maxHeapSize;
		unsigned int maxHeapCount;
		allocator->getHeapStats(heapSize, heapCount, maxHeapSize, maxHeapCount);
		StandardOut::singleton()->print(
			MESSAGE_INFO,
			"Script Heap Stats: Max Size = %d bytes, Max Count = %d blocks",
			maxHeapSize,
			maxHeapCount
		);
		allocator->clearHeapMax();

		if (contextCount == 0 && (heapSize > 0 || heapCount > 0)) {
			StandardOut::singleton()
				->print(MESSAGE_ERROR, "Script memory leaks: %d bytes, %d blocks", heapSize, heapCount);
		}
	}

	globalState = NULL;
}

// FUNCTION: WEBSERVICE 0x10066200
ScriptContext::ScriptContext()
	: globalState(NULL), nextPendingScripts(boost::posix_time::second_clock::local_time()), scriptsDisabled(false)
{
	setName("Script Context");
}

// FUNCTION: WEBSERVICE 0x10066460
ScriptContext::~ScriptContext()
{
}

// FUNCTION: WEBSERVICE 0x10066ce0
int ScriptContext::settings(lua_State* thread)
{
	Security::Context::current().requirePermission(Security::Administrator, NULL);

	Lua::SharedPtrBridge<Reflection::DescribedBase>::push(thread, GlobalSettings::singleton());

	return 1;
}

// FUNCTION: WEBSERVICE 0x10066de0
int ScriptContext::trustedThread(lua_State* thread)
{
	bool trusted = Security::Context::current().hasPermission(Security::Administrator);
	lua_pushboolean(thread, trusted ? 1 : 0);

	return 1;
}

static int loadAux(lua_State* L, int status)
{
	if (status == 0) {
		return 1;
	}
	else {
		lua_pushnil(L);
		lua_insert(L, -2);
		return 2;
	}
}

// FUNCTION: WEBSERVICE 0x10066e10
int ScriptContext::loadfile(lua_State* L)
{
	Security::Context::current().requirePermission(Security::Administrator, "loadfile");

	ContentId contentId(lua_tostring(L, -1));
	std::string file = ContentProvider::singleton().getFile(contentId);

	return loadAux(L, luaL_loadfile(L, file.c_str()));
}

// FUNCTION: WEBSERVICE 0x10066f20
int ScriptContext::stats(lua_State* L)
{
	Security::Context::current().requirePermission(Security::Administrator, "Stats");

	Lua::SharedPtrBridge<Reflection::DescribedBase>::push(
		L,
		shared_from(ServiceProvider::create<Stats::StatsService>(&getContext(L)))
	);

	return 1;
}

// FUNCTION: WEBSERVICE 0x10067140
void ScriptContext::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	stopListening<RunService, RunTransition>(runService, this);
	stopListening<RunService, Heartbeat>(runService, this);

	if (oldProvider && !newProvider) {
		closeState();
	}

	if (statsItem) {
		statsItem->setParent(NULL);
		statsItem.reset();
	}

	Instance::onServiceProvider(oldProvider, newProvider);

	Stats::StatsService* stats = newProvider ? newProvider->create<Stats::StatsService>() : NULL;
	if (stats) {
		statsItem = Creatable<Instance>::create<Stats::Item>("Lua");
		statsItem->setParent(stats);
		statsItem->createBoundChildItem("disabled", scriptsDisabled);
		statsItem->createChildItem<int>("threads", boost::bind(&ScriptContext::getThreadCount, this));
	}

	runService = shared_from(newProvider ? newProvider->create<RunService>() : NULL);

	startListening<RunService, RunTransition>(runService, this);
	startListening<RunService, Heartbeat>(runService, this);
}

// STUB: WEBSERVICE 0x100673d0
ScriptContext::Result ScriptContext::resume(lua_State* L, int nargs)
{
	STUB(0x100673d0);
	return Success;
}

// FUNCTION: WEBSERVICE 0x10067c20
void ScriptContext::setThreadIdentity(lua_State* thread, Security::Identities identity)
{
	lua_pushlightuserdata(thread, (void*) 0x54);
	lua_pushinteger(thread, identity);
	lua_settable(thread, LUA_GLOBALSINDEX);

	if (DebugSettings::singleton().getioEnabled()) {
		lua_pushcfunction(thread, luaopen_io);
		lua_pushstring(thread, "io");
		lua_call(thread, 1, 0);
	}

	lua_pushstring(thread, "settings");
	lua_pushcfunction(thread, settings);
	lua_settable(thread, LUA_GLOBALSINDEX);
}

// STUB: WEBSERVICE 0x10067e00
void ScriptContext::startScript(boost::shared_ptr<Script> script)
{
	STUB(0x10067e00);
}

// FUNCTION: WEBSERVICE 0x10068640
void ScriptContext::startPendingScripts()
{
	if (!scriptsDisabled) {
		std::vector<boost::shared_ptr<Script> > copy(pendingScripts);
		pendingScripts.clear();

		std::for_each(copy.begin(), copy.end(), boost::bind(&ScriptContext::startScript, this, _1));
	}
}

// FUNCTION: WEBSERVICE 0x10068760
void ScriptContext::onChangedScriptEnabled(const Reflection::PropertyDescriptor&)
{
	if (scriptsDisabled) {
		startPendingScripts();
	}
}

// FUNCTION: WEBSERVICE 0x10068780
void ScriptContext::addScript(Script* script)
{
	if (scripts.insert(script).second == true) {
		if (!script->isDisabled()) {
			startScript(shared_from(script));
		}
	}
}

// STUB: WEBSERVICE 0x10068880
void ScriptContext::onEvent(const RunService* source, Heartbeat event)
{
	STUB(0x10068880);
}

} // namespace RBX

template void RBX::Lua::SharedPtrBridge<RBX::Reflection::DescribedBase>::push(
	lua_State* L,
	boost::shared_ptr<RBX::Reflection::DescribedBase> instance
);

template void RBX::Lua::SharedPtrBridge<RBX::Reflection::SignalInstance>::push(
	lua_State* L,
	boost::shared_ptr<RBX::Reflection::SignalInstance> instance
);

template const RBX::Name& RBX::Name::doDeclare<RBX::Stats::sStatsItem>();
template const RBX::Name& RBX::Name::doDeclare<RBX::Stats::sStats>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sDebugSettings>();
template void RBX::Name::callDoDeclare<RBX::Stats::sStatsItem>();
template void RBX::Name::callDoDeclare<RBX::Stats::sStats>();
template void RBX::Name::callDoDeclare<RBX::sDebugSettings>();

template class RBX::FactoryProduct<RBX::ScriptContext, RBX::Instance, RBX::sScriptContext>;
template class RBX::Reflection::Described<
	RBX::ScriptContext,
	RBX::sScriptContext,
	RBX::FactoryProduct<RBX::ScriptContext, RBX::Instance, RBX::sScriptContext> >;
template class RBX::DescribedCreatable<RBX::ScriptContext, RBX::Instance, RBX::sScriptContext>;
