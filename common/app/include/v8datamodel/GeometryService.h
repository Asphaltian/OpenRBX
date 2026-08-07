#ifndef V8DATAMODEL_GEOMETRYSERVICE_H
#define V8DATAMODEL_GEOMETRYSERVICE_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

namespace RBX {

extern char sGeometryService[];
// SIZE 0xfc
class GeometryService : public DescribedCreatable<GeometryService, Instance, sGeometryService>, public Service
{
private:
	undefined m_unk0x0f8[0xfc - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(GeometryService, 0xfc)

} // namespace RBX

#endif // V8DATAMODEL_GEOMETRYSERVICE_H
