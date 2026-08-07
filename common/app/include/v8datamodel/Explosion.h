#ifndef V8DATAMODEL_EXPLOSION_H
#define V8DATAMODEL_EXPLOSION_H

#include "decomp.h"
#include "util/Handle.h"
#include "util/IRenderable.h"
#include "util/RunStateOwner.h"
#include "v8tree/Instance.h"

namespace RBX {

extern char sExplosion[];
// SIZE 0x124
class Explosion : public DescribedCreatable<Explosion, Instance, sExplosion>,
				  public Listener<RunService, Stepped>,
				  public IRenderable
{
protected:
	virtual void onEvent(const RunService* source, Stepped event);

private:
	undefined m_unk0x10c[0x124 - 0x10c]; // 0x10c
};

DECOMP_SIZE_ASSERT(Explosion, 0x124)

} // namespace RBX

#endif // V8DATAMODEL_EXPLOSION_H
