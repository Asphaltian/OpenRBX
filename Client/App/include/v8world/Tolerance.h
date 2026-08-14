#ifndef V8WORLD_TOLERANCE_H
#define V8WORLD_TOLERANCE_H

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

class Tolerance
{
public:
	// FUNCTION: WEBSERVICE 0x1011ed70
	static bool pointsUnaligned(const Vector3& p0, const Vector3& p1)
	{
		return 0.05f * 0.05f < (p1 - p0).squaredMagnitude();
	}
};

} // namespace RBX

#endif // V8WORLD_TOLERANCE_H
