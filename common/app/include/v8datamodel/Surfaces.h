#ifndef V8DATAMODEL_SURFACES_H
#define V8DATAMODEL_SURFACES_H

#include "decomp.h"

namespace RBX {

// SIZE 0x30
class Surfaces
{
private:
	undefined m_unk0x00[0x30 - 0x00]; // 0x00
};

DECOMP_SIZE_ASSERT(Surfaces, 0x30)

} // namespace RBX

#endif // V8DATAMODEL_SURFACES_H
