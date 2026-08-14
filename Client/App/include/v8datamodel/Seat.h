#ifndef V8DATAMODEL_SEAT_H
#define V8DATAMODEL_SEAT_H

#include "decomp.h"
#include "v8datamodel/PartInstance.h"
#include "v8tree/Instance.h"

namespace RBX {

extern char sSeat[];
// SIZE 0x2e4
class Seat : public DescribedCreatable<Seat, PartInstance, sSeat>
{
private:
	undefined m_unk0x2ac[0x2e4 - 0x2ac]; // 0x2ac
};

DECOMP_SIZE_ASSERT(Seat, 0x2e4)

} // namespace RBX

#endif // V8DATAMODEL_SEAT_H
