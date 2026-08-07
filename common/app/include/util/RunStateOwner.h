#ifndef UTIL_RUNSTATEOWNER_H
#define UTIL_RUNSTATEOWNER_H

#include "decomp.h"
#include "util/Events.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

namespace RBX {

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
	undefined m_unk0x15c[0x1c0 - 0x15c]; // 0x15c
};

DECOMP_SIZE_ASSERT(RunService, 0x1c0)

} // namespace RBX

#endif // UTIL_RUNSTATEOWNER_H
