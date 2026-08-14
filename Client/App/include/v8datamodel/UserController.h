#ifndef V8DATAMODEL_USERCONTROLLER_H
#define V8DATAMODEL_USERCONTROLLER_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

namespace RBX {

extern char sControllerService[];
// SIZE 0x10c
class ControllerService : public DescribedCreatable<ControllerService, Instance, sControllerService>, public Service
{
private:
	undefined m_unk0x0f8[0x10c - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(ControllerService, 0x10c)

} // namespace RBX

#endif // V8DATAMODEL_USERCONTROLLER_H
