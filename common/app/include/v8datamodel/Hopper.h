#ifndef V8DATAMODEL_HOPPER_H
#define V8DATAMODEL_HOPPER_H

#include "decomp.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sLegacyHopperService[];

extern const char sStarterPackService[];

extern const char sHopperBin[];

extern const char sBackpackItem[];

// SIZE 0x168
class BackpackItem : public Instance
{
private:
	undefined m_unk0x0f8[0x168 - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(BackpackItem, 0x168)

} // namespace RBX

#endif // V8DATAMODEL_HOPPER_H
