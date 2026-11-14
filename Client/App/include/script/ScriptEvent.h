#ifndef SCRIPT_SCRIPTEVENT_H
#define SCRIPT_SCRIPTEVENT_H

#include "script/ThreadRef.h"
#include "placeholder.h"
#include "util/RunStateOwner.h"

#include <boost/shared_ptr.hpp>
#include <vector>

struct lua_State;

namespace RBX {

class ScriptContext;

namespace Lua {

class YieldingThreads
{
public:
	struct WaitingThread
	{
		WaitingThread(lua_State* L, double requestedDelay);

		boost::shared_ptr<ThreadRef> thread;
		double requestedDelay;
		double elapsedTime;
	};

	typedef std::vector<WaitingThread> ThreadRefs;

	YieldingThreads(ScriptContext* context);

	void queueWaiter(lua_State* L, double requestedDelay);
	void queueWaiter(lua_State* L);

	void resume(Heartbeat event);

private:
	void clearAllSinks();

	ScriptContext* context;
	ThreadRefs waitingThreads;
};

} // namespace Lua
} // namespace RBX

#endif // SCRIPT_SCRIPTEVENT_H
