#ifndef V8WORLD_TOLERANCE_H
#define V8WORLD_TOLERANCE_H

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

class Tolerance
{
public:
	static float mainGrid();
	static float jointMaxUnaligned();
	static float jointOverlapMin();

	// FUNCTION: WEBSERVICE 0x1011ed70
	static bool pointsUnaligned(const Vector3& p0, const Vector3& p1)
	{
		return 0.05f * 0.05f < (p1 - p0).squaredMagnitude();
	}

	static float jointAngleMax();
	static float jointPlanarMax();
	static float rotateAngleMax();
	static float rotatePlanarMax();
	static float glueAngleMax();
	static float gluePlanarMax();
	static float maxOverlapOrGap();
};

} // namespace RBX

#endif // V8WORLD_TOLERANCE_H
