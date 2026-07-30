#ifndef UTIL_QUATERNION_H
#define UTIL_QUATERNION_H

#include <G3D/Matrix3.h>
#include <G3D/g3dmath.h>

namespace RBX {

class Quaternion
{
public:
	float x; // 0x00
	float y; // 0x04
	float z; // 0x08
	float w; // 0x0c

	Quaternion& operator=(const Quaternion& other);
	void toRotationMatrix(G3D::Matrix3& rot) const;

	// FUNCTION: WEBSERVICE 0x10126ba0
	float maxComponent() const
	{
		float aw = G3D::abs(w);
		float az = G3D::abs(z);
		float ay = G3D::abs(y);
		float ax = G3D::abs(x);

		float hi = aw > az ? aw : az;
		float lo = ay > ax ? ay : ax;

		return lo < hi ? hi : lo;
	}
};

} // namespace RBX

#endif // UTIL_QUATERNION_H
