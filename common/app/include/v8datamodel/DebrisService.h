#ifndef V8DATAMODEL_DEBRISSERVICE_H
#define V8DATAMODEL_DEBRISSERVICE_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"

namespace RBX {

extern char sDebrisService[];
// SIZE 0x114
class DebrisService : public DescribedCreatable<DebrisService, Instance, sDebrisService>
{
private:
	undefined m_unk0x0f8[0x114 - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(DebrisService, 0x114)

} // namespace RBX

#endif // V8DATAMODEL_DEBRISSERVICE_H
