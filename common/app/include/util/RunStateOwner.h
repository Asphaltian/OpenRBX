#ifndef UTIL_RUNSTATEOWNER_H
#define UTIL_RUNSTATEOWNER_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sRunService[];
// SIZE 0x1c0
class RunService : public DescribedCreatable<RunService, Instance, sRunService>
{
private:
	undefined m_unk0x0f8[0x1c0 - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(RunService, 0x1c0)

} // namespace RBX

#endif // UTIL_RUNSTATEOWNER_H
