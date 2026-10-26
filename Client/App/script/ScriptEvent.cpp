#include "script/ScriptEvent.h"

#include "script/ScriptContext.h"

#include <lua.h>

namespace RBX {
namespace Lua {

// FUNCTION: WEBSERVICE 0x100b5790
YieldingThreads::WaitingThread::WaitingThread(lua_State* L, double requestedDelay)
	: thread(new ThreadRef(L)), requestedDelay(requestedDelay), elapsedTime(0.0)
{
}

// FUNCTION: WEBSERVICE 0x100b5e00
YieldingThreads::YieldingThreads(ScriptContext* context) : context(context)
{
}

// FUNCTION: WEBSERVICE 0x100b5f80
void YieldingThreads::queueWaiter(lua_State* L, double requestedDelay)
{
	RobloxExtraSpace::get(L)->yieldCaptured = true;

	waitingThreads.push_back(WaitingThread(L, requestedDelay));
}

// FUNCTION: WEBSERVICE 0x100b6030
void YieldingThreads::resume(Heartbeat event)
{
	if (waitingThreads.size() == 0) {
		return;
	}

	ThreadRefs copy(waitingThreads);

	waitingThreads.clear();

	for (ThreadRefs::iterator iter = copy.begin(); iter != copy.end(); ++iter) {
		if (!iter->thread->thread()) {
			continue;
		}

		iter->elapsedTime += event.step;
		if (iter->elapsedTime < iter->requestedDelay) {
			waitingThreads.push_back(*iter);
		}
		else {
			lua_State* thread = iter->thread->thread();

			const int oldTop = lua_gettop(thread);

			lua_pushnumber(thread, iter->elapsedTime);

			context->resume(thread, 1);

			lua_settop(thread, oldTop);
		}
	}
}

// FUNCTION: WEBSERVICE 0x100b61d0
void YieldingThreads::queueWaiter(lua_State* L)
{
	queueWaiter(L, 0.0);
}

} // namespace Lua
} // namespace RBX
