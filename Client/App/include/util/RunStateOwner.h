#ifndef UTIL_RUNSTATEOWNER_H
#define UTIL_RUNSTATEOWNER_H

#include <time.h>
#undef TIME_UTC

#include "placeholder.h"
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

class Stepped
{
public:
	const float time;
	const float step;

	Stepped(float time, float step) : time(time), step(step) {}
};

class Heartbeat
{
public:
	const float time;
	const float step;

	Heartbeat(float time, float step) : time(time), step(step) {}
};

class RunTransition
{
public:
	RunState oldState;
	RunState newState;

	RunTransition(RunState oldState, RunState newState) : oldState(oldState), newState(newState) {}

	bool startEvent();
	bool resetEvent();
};

class __declspec(novtable) IRunView
{
public:
	virtual void InvalidateRunView() = 0;
	virtual void AdjustThrottle(double frameRate) = 0;
};

extern const char sRunService[];

class RunService : public DescribedCreatable<RunService, Instance, sRunService>,
				   public Notifier<RunService, Heartbeat>,
				   public Notifier<RunService, Stepped>,
				   public Notifier<RunService, RunTransition>,
				   public Notifier<RunService, RunState>,
				   public Service
{
private:
	typedef std::map<IRunView*, bool> Views;

	RunState runState;
	boost::mutex runMutex;
	bool stopRequested;
	boost::condition runViewsDoneCondition;
	boost::condition stateChangedCondition;
	float framePeriod;
	boost::scoped_ptr<boost::thread> runThread;
	boost::mutex viewMutex;
	Views views;
	int invalidRunViewCount;
	int stepCount;
	double stepTime;

	void start();

	friend class DataModel;

public:
	bool runDisabled;

	RunService();

	virtual ~RunService();

	void setRunState(RunState state);

	void run()
	{
		if (!runDisabled) {
			setRunState(RS_RUNNING);
		}
	}

	void pause() { setRunState(RS_PAUSED); }

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

	int getStepCount() const { return stepCount; }
	double getStepTime() const { return stepTime; }

	void addRunView(IRunView* runView);
	void removeRunView(IRunView* runView);
	void invalidateRunViews();
	void runViewValid(IRunView* runView);

	static Reflection::SignalDesc<RunService, void(float, float)> event_Stepped;

	virtual void onAncestorChanged(const AncestorChanged& event);

private:
	void runProc(boost::shared_ptr<DataModel> dataModel);
};

} // namespace RBX

#endif // UTIL_RUNSTATEOWNER_H
