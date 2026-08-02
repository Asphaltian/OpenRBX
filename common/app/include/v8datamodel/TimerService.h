#ifndef V8DATAMODEL_TIMERSERVICE_H
#define V8DATAMODEL_TIMERSERVICE_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"

namespace RBX {

extern char sTimerService[];
// SIZE 0x110
class TimerService : public DescribedCreatable<TimerService, Instance, sTimerService>
{
private:
	undefined m_unk0x0f8[0x110 - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(TimerService, 0x110)

} // namespace RBX

#endif // V8DATAMODEL_TIMERSERVICE_H
