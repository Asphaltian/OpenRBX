#ifndef SCRIPT_SCRIPTEVENT_H
#define SCRIPT_SCRIPTEVENT_H

#include "decomp.h"
#include "script/ThreadRef.h"

#include <boost/shared_ptr.hpp>
#include <vector>

struct lua_State;

namespace RBX {

class ScriptContext;

namespace Lua {

// SIZE 0x14
class YieldingThreads
{
public:
	// SIZE 0x18
	struct WaitingThread
	{
		WaitingThread(lua_State* L, double requestedDelay);

		boost::shared_ptr<ThreadRef> thread; // 0x00
		double requestedDelay;               // 0x08
		double elapsedTime;                  // 0x10
	};

	typedef std::vector<WaitingThread> ThreadRefs;

	YieldingThreads(ScriptContext* context);

	void queueWaiter(lua_State* L, double requestedDelay);
	void queueWaiter(lua_State* L);

	void resume(double elapsedTime);

private:
	void clearAllSinks();

	ScriptContext* context;    // 0x00
	ThreadRefs waitingThreads; // 0x04
};

DECOMP_SIZE_ASSERT(YieldingThreads, 0x14)
DECOMP_SIZE_ASSERT(YieldingThreads::WaitingThread, 0x18)

} // namespace Lua
} // namespace RBX

#endif // SCRIPT_SCRIPTEVENT_H
