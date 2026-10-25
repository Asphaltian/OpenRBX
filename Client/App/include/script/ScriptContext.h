#ifndef SCRIPT_SCRIPTCONTEXT_H
#define SCRIPT_SCRIPTCONTEXT_H

#include "decomp.h"
#include "script/ThreadRef.h"
#include "security/SecurityContext.h"
#include "util/Handle.h"
#include "util/RunStateOwner.h"
#include "v8datamodel/Stats.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <lua.h>
#include <set>
#include <vector>

class LuaAllocator;
struct lua_State;

namespace RBX {

class Script;

namespace Lua {
class YieldingThreads;
}

extern char sScriptContext[];
// SIZE 0x168
class ScriptContext : public DescribedCreatable<ScriptContext, Instance, sScriptContext>,
					  public Listener<RunService, RunTransition>,
					  public Listener<RunService, Heartbeat>,
					  public Service
{
public:
	ScriptContext();
	virtual ~ScriptContext();

	enum Result
	{
		Success,
		Yield,
		Error
	};

	static ScriptContext& getContext(lua_State* L);

	Result resume(lua_State* thread, int narg);

	unsigned int getThreadCount() const;

	void closeState();
	void addScript(Script* script);
	void removeScript(Script* script);

protected:
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
	virtual void onEvent(const RunService* source, RunTransition event);
	virtual void onEvent(const RunService* source, Heartbeat event);

private:
	static void setThreadIdentity(lua_State* thread, Security::Identities identity);

	static Security::Identities getThreadIdentity(lua_State* thread)
	{
		lua_pushlightuserdata(thread, (void*) 0x54);
		lua_gettable(thread, LUA_GLOBALSINDEX);

		if (!lua_isnumber(thread, -1)) {
			return Security::Anonymous;
		}

		Security::Identities identity = static_cast<Security::Identities>(lua_tointeger(thread, -1));
		lua_settop(thread, -2);

		return identity;
	}

	static int trustedThread(lua_State* thread);
	static int loadfile(lua_State* L);
	static int settings(lua_State* thread);
	static int stats(lua_State* L);
	static int dofile(lua_State* L);

	static void reportError(lua_State* thread);

	void onChangedScriptEnabled(const Reflection::PropertyDescriptor&);

	void disassociateState(Script* script);
	void startScript(boost::shared_ptr<Script> script);
	void startPendingScripts();

	lua_State* globalState;                                 // 0x100
	Lua::ThreadRef commandLineSandbox;                      // 0x104
	std::set<Script*> scripts;                              // 0x124
	boost::posix_time::ptime nextPendingScripts;            // 0x130
	std::vector<boost::shared_ptr<Script> > pendingScripts; // 0x138
	boost::shared_ptr<RunService> runService;               // 0x148
	boost::scoped_ptr<Lua::YieldingThreads> yieldEvent;     // 0x150
	bool scriptsDisabled;                                   // 0x154
	boost::shared_ptr<Stats::Item> statsItem;               // 0x158

	// SIZE 0x4
	class ScriptImpersonator : public Security::Impersonator
	{
	public:
		ScriptImpersonator(lua_State* thread);
		~ScriptImpersonator() {}
	};

public:
	boost::scoped_ptr<LuaAllocator> allocator; // 0x160
};

DECOMP_SIZE_ASSERT(ScriptContext, 0x168)

} // namespace RBX

#endif // SCRIPT_SCRIPTCONTEXT_H
