#ifndef V8DATAMODEL_CAMERA_H
#define V8DATAMODEL_CAMERA_H

#include "decomp.h"
#include "v8tree/Instance.h"

namespace RBX {

extern char sCamera[];
// SIZE 0x1b0
class Camera : public DescribedCreatable<Camera, Instance, sCamera>
{
private:
	undefined m_unk0x0f8[0x1b0 - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(Camera, 0x1b0)

} // namespace RBX

#endif // V8DATAMODEL_CAMERA_H
