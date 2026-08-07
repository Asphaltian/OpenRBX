#ifndef V8DATAMODEL_TIMERSERVICE_H
#define V8DATAMODEL_TIMERSERVICE_H

#include "decomp.h"
#include "util/Handle.h"
#include "util/RunStateOwner.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

namespace RBX {

extern char sTimerService[];
// SIZE 0x110
class TimerService : public DescribedCreatable<TimerService, Instance, sTimerService>,
					 public Listener<RunService, Heartbeat>,
					 public Service
{
protected:
	virtual void onEvent(const RunService* source, Heartbeat event);

private:
	undefined m_unk0x0fc[0x110 - 0x0fc]; // 0x0fc
};

DECOMP_SIZE_ASSERT(TimerService, 0x110)

} // namespace RBX

#endif // V8DATAMODEL_TIMERSERVICE_H
