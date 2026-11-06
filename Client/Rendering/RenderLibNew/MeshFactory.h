#ifndef RENDERLIBNEW_MESHFACTORY_H
#define RENDERLIBNEW_MESHFACTORY_H

#include "decomp.h"
#include "util/NormalId.h"

namespace RBX {
namespace ViewNew {

enum RenderSurfaceType
{
	FLAT = 0,
	STUDS = 1,
	INLET = 2,
	WELD = 3,
	GLUE = 4
};

// SIZE 0x4
class RenderSurfaceTypes
{
public:
	unsigned int data; // 0x00

	RenderSurfaceTypes() : data(0) {}

	void setSurfaceType(NormalId face, RenderSurfaceType surfaceType)
	{
		data = (data & (unsigned short) ~(7 << (face * 3))) | (surfaceType << (face * 3));
	}

	RenderSurfaceType getSurfaceType(NormalId face) const { return (RenderSurfaceType) ((data >> (face * 3)) & 7); }

	unsigned int hashCode() const { return data; }

	bool operator==(const RenderSurfaceTypes& other) const { return data == other.data; }
	bool operator<(const RenderSurfaceTypes& other) const { return data < other.data; }
	bool operator>(const RenderSurfaceTypes& other) const { return data > other.data; }
};

DECOMP_SIZE_ASSERT(RenderSurfaceTypes, 0x4)

} // namespace ViewNew
} // namespace RBX

#endif // RENDERLIBNEW_MESHFACTORY_H
