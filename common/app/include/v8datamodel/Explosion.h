#ifndef V8DATAMODEL_EXPLOSION_H
#define V8DATAMODEL_EXPLOSION_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"

namespace RBX {

extern char sExplosion[];
// SIZE 0x124
class Explosion : public DescribedCreatable<Explosion, Instance, sExplosion>
{
private:
	undefined m_unk0x0f8[0x124 - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(Explosion, 0x124)

} // namespace RBX

#endif // V8DATAMODEL_EXPLOSION_H
