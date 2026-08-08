#ifndef V8DATAMODEL_CAMERA_H
#define V8DATAMODEL_CAMERA_H

#include "decomp.h"
#include "v8tree/Instance.h"

#include <G3D/GCamera.h>

namespace RBX {

extern char sCamera[];
// SIZE 0x1b0
class Camera : public DescribedCreatable<Camera, Instance, sCamera>
{
public:
	const G3D::GCamera& getGCamera() const { return gCamera; }

private:
	G3D::GCamera gCamera;                // 0x0f8
	undefined m_unk0x13c[0x1b0 - 0x13c]; // 0x13c
};

DECOMP_SIZE_ASSERT(Camera, 0x1b0)

} // namespace RBX

#endif // V8DATAMODEL_CAMERA_H
