#ifndef V8DATAMODEL_VISIT_H
#define V8DATAMODEL_VISIT_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

namespace RBX {

extern char sVisit[];
// SIZE 0x118
class Visit : public DescribedCreatable<Visit, Instance, sVisit>, public Service
{
private:
	undefined m_unk0x0f8[0x118 - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(Visit, 0x118)

} // namespace RBX

#endif // V8DATAMODEL_VISIT_H
