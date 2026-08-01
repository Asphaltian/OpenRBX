#ifndef UTIL_FACE_H
#define UTIL_FACE_H

#include "decomp.h"
#include "util/Extents.h"
#include "util/NormalId.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/Vector3.h>

namespace RBX {

using G3D::CoordinateFrame;
using G3D::Vector3;

// SIZE 0x30
class Face
{
public:
	static Face fromExtentsSide(const Extents& extents, NormalId normalId);

	Face toWorldSpace(const CoordinateFrame& coord) const;

	Vector3& operator[](int index);

private:
	Vector3 c0; // 0x00
	Vector3 c1; // 0x0c
	Vector3 c2; // 0x18
	Vector3 c3; // 0x24
};

DECOMP_SIZE_ASSERT(Face, 0x30)

} // namespace RBX

#endif // UTIL_FACE_H
