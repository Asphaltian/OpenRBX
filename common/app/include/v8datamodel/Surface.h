#ifndef V8DATAMODEL_SURFACE_H
#define V8DATAMODEL_SURFACE_H

#include "decomp.h"
#include "util/NormalId.h"
#include "v8world/SurfaceData.h"

namespace RBX {

class PartInstance;

// SIZE 0x8
class Surface
{
private:
	PartInstance* partInstance; // 0x00
	NormalId surfId;            // 0x04

public:
	SurfaceType getSurfaceType() const;
};

DECOMP_SIZE_ASSERT(Surface, 0x8)

} // namespace RBX

#endif // V8DATAMODEL_SURFACE_H
