#ifndef V8DATAMODEL_LIGHTING_H
#define V8DATAMODEL_LIGHTING_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <G3D/Color3.h>
#include <G3D/Color4.h>

namespace RBX {

extern char sLighting[];
// SIZE 0x230
class Lighting : public DescribedCreatable<Lighting, Instance, sLighting>, public Service
{
private:
	undefined m_unk0x0f8[0x1f0 - 0x0f8]; // 0x0f8
	G3D::Color3 ambientTop;              // 0x1f0
	G3D::Color3 ambientBottom;           // 0x1fc
	bool hasSky;                         // 0x208
	undefined m_unk0x209[0x20c - 0x209]; // 0x209
	G3D::Color4 clearColor;              // 0x20c
	undefined m_unk0x21c[0x230 - 0x21c]; // 0x21c
};

DECOMP_SIZE_ASSERT(Lighting, 0x230)

} // namespace RBX

#endif // V8DATAMODEL_LIGHTING_H
