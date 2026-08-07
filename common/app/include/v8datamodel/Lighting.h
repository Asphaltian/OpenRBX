#ifndef V8DATAMODEL_LIGHTING_H
#define V8DATAMODEL_LIGHTING_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

namespace RBX {

extern char sLighting[];
// SIZE 0x230
class Lighting : public DescribedCreatable<Lighting, Instance, sLighting>, public Service
{
private:
	undefined m_unk0x0f8[0x230 - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(Lighting, 0x230)

} // namespace RBX

#endif // V8DATAMODEL_LIGHTING_H
