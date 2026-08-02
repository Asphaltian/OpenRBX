#ifndef V8DATAMODEL_FORCEFIELD_H
#define V8DATAMODEL_FORCEFIELD_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"

namespace RBX {

extern char sForceField[];
// SIZE 0x10c
class ForceField : public DescribedCreatable<ForceField, Instance, sForceField>
{
private:
	undefined m_unk0x0f8[0x10c - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(ForceField, 0x10c)

} // namespace RBX

#endif // V8DATAMODEL_FORCEFIELD_H
