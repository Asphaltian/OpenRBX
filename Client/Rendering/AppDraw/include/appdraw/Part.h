#ifndef APPDRAW_PART_H
#define APPDRAW_PART_H

#include "decomp.h"
#include "util/SurfaceType.h"
#include "util/Vector6.h"

#include <G3D/Color4.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/Vector3.h>

namespace RBX {

// SIZE 0x68
class Part
{
public:
	enum PartType
	{
		BALL_PART = 0,
		BLOCK_PART = 1,
		CYLINDER_PART = 2,
	};

	PartType type;                        // 0x00
	G3D::Vector3 gridSize;                // 0x04
	G3D::Color4 color;                    // 0x10
	Vector6<SurfaceType> surfaceType;     // 0x20
	G3D::CoordinateFrame coordinateFrame; // 0x38
};

DECOMP_SIZE_ASSERT(Part, 0x68)

} // namespace RBX

#endif // APPDRAW_PART_H
