#ifndef UTIL_RUNSTATEOWNER_H
#define UTIL_RUNSTATEOWNER_H

#include <time.h>
#undef TIME_UTC

#include "decomp.h"
#include "util/Events.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <map>

namespace boost {
class thread;
}

namespace RBX {

class DataModel;

enum RunState
{
	RS_NORMAL = 0,
	RS_RUNNING = 1,
	RS_PAUSED = 2
};

// SIZE 0x8
class Stepped
{
public:
	const float time; // 0x00
	const float step; // 0x04

	Stepped(float time, float step) : time(time), step(step) {}
};

DECOMP_SIZE_ASSERT(Stepped, 0x8)

// SIZE 0x8
class Heartbeat
{
public:
	const float time; // 0x00
	const float step; // 0x04

	Heartbeat(float time, float step) : time(time), step(step) {}
};

DECOMP_SIZE_ASSERT(Heartbeat, 0x8)

// SIZE 0x8
class RunTransition
{
public:
	RunState oldState; // 0x00
	RunState newState; // 0x04

	RunTransition(RunState oldState, RunState newState) : oldState(oldState), newState(newState) {}

	bool startEvent();
	bool resetEvent();
};

DECOMP_SIZE_ASSERT(RunTransition, 0x8)

// SIZE 0x4
class __declspec(novtable) IRunView
{
public:
	virtual void InvalidateRunView() = 0;              // vtable+0x00
	virtual void AdjustThrottle(double frameRate) = 0; // vtable+0x04
};

DECOMP_SIZE_ASSERT(IRunView, 0x4)

extern const char sRunService[];

// SIZE 0x1c0
class RunService : public DescribedCreatable<RunService, Instance, sRunService>,
				   public Notifier<RunService, Heartbeat>,
				   public Notifier<RunService, Stepped>,
				   public Notifier<RunService, RunTransition>,
				   public Notifier<RunService, RunState>,
				   public Service
{
private:
	typedef std::map<IRunView*, bool> Views;

	RunState runState;                          // 0x15c
	boost::mutex runMutex;                      // 0x160
	bool stopRequested;                         // 0x168
	boost::condition runViewsDoneCondition;     // 0x16c
	boost::condition stateChangedCondition;     // 0x184
	float framePeriod;                          // 0x19c
	boost::scoped_ptr<boost::thread> runThread; // 0x1a0
	boost::mutex viewMutex;                     // 0x1a4
	Views views;                                // 0x1ac
	int invalidRunViewCount;                    // 0x1b8

	void start();

public:
	bool runDisabled; // 0x1bc

	RunService();

	virtual ~RunService();

	void setRunState(RunState state)
	{
		if (runState == state) {
			return;
		}

		RunState oldState = runState;

		runState = state;
		Notifier<RunService, RunState>::raise(state);
		Notifier<RunService, RunTransition>::raise(RunTransition(oldState, state));
	}

	void run()
	{
		if (!runDisabled && (isEditState() || isPauseState())) {
			setRunState(RS_RUNNING);
		}
	}

	void pause()
	{
		if (isRunState()) {
			setRunState(RS_PAUSED);
		}
	}

	void reset() { setRunState(RS_NORMAL); }

	void endRunThread(bool join);

	void raiseHeartbeat(float time, float step);
	void raiseStepped(float time, float step);

	RunState getRunState() const { return runState; }

	bool isEditState() const { return runState == RS_NORMAL; }
	bool isRunState() const { return runState == RS_RUNNING; }
	bool isPauseState() const { return runState == RS_PAUSED; }

	void setPeriod(float period) { framePeriod = period; }
	float getPeriod() { return framePeriod; }

	void addRunView(IRunView* runView);
	void removeRunView(IRunView* runView);
	void invalidateRunViews();
	void runViewValid(IRunView* runView);

	static Reflection::SignalDesc<RunService, void(float, float)> event_Stepped;

	virtual void onAncestorChanged(const AncestorChanged& event); // vtable+0x10

private:
	void runProc(boost::shared_ptr<DataModel> dataModel);
};

DECOMP_SIZE_ASSERT(RunService, 0x1c0)

// clang-format off
// FUNCTION: WEBSERVICE 0x100543f0
// RBX::Notifier<RBX::RunService,RBX::Heartbeat>::raise
// FUNCTION: WEBSERVICE 0x100544b0
// RBX::Notifier<RBX::RunService,RBX::Stepped>::raise
// FUNCTION: WEBSERVICE 0x10054890
// RBX::Notifier<RBX::RunService,RBX::Heartbeat>::raise
// FUNCTION: WEBSERVICE 0x10054930
// RBX::Notifier<RBX::RunService,RBX::Stepped>::raise
// clang-format on

} // namespace RBX

#endif // UTIL_RUNSTATEOWNER_H
