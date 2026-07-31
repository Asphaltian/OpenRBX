#ifndef V8WORLD_SURFACEDATA_H
#define V8WORLD_SURFACEDATA_H

#include "decomp.h"
#include "v8world/Controller.h"

namespace RBX {

enum SurfaceType
{
	NO_SURFACE = 0,
	GLUE = 1,
	WELD = 2,
	STUDS = 3,
	INLET = 4,
	SPAWN_deprecated = 5,
	ROTATE = 6,
	ROTATE_V = 7,
	ROTATE_P = 8,
	NUM_SURF_TYPES = 9,
};

class SurfaceData
{
public:
	static const SurfaceData& empty();

	bool operator==(const SurfaceData& other) const;
	bool isEmpty() const;

	Controller::InputType inputType; // 0x00
	float paramA;                    // 0x04
	float paramB;                    // 0x08
};

DECOMP_SIZE_ASSERT(SurfaceData, 0x0c)

} // namespace RBX

#endif // V8WORLD_SURFACEDATA_H
