#ifndef UTIL_SURFACETYPE_H
#define UTIL_SURFACETYPE_H

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

} // namespace RBX

#endif // UTIL_SURFACETYPE_H
