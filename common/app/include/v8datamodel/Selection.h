#ifndef V8DATAMODEL_SELECTION_H
#define V8DATAMODEL_SELECTION_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sSelection[];
// SIZE 0x12c
class Selection : public DescribedCreatable<Selection, Instance, sSelection>
{
private:
	undefined m_unk0x0f8[0x12c - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(Selection, 0x12c)

} // namespace RBX

#endif // V8DATAMODEL_SELECTION_H
