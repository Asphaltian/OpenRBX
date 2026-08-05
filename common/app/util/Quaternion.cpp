#include "util/Quaternion.h"

#include "decomp.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x10129fd0
Quaternion& Quaternion::operator=(const Quaternion& other)
{
	w = other.w;
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

// STUB: WEBSERVICE 0x10129ff0
Quaternion::Quaternion(const G3D::Matrix3& rot)
{
	STUB(0x10129ff0);
}

// FUNCTION: WEBSERVICE 0x1012a150
void Quaternion::toRotationMatrix(G3D::Matrix3& rot) const
{
	float tx = x * 2.0f;
	float ty = y * 2.0f;
	float tz = z * 2.0f;

	float twx = x * tx;
	float txy = x * ty;
	float txz = x * tz;
	float twxw = w * tx;
	float twy = w * ty;
	float wz = w * tz;
	float tyy = y * ty;
	float yz = y * tz;
	float zz = z * tz;

	rot[0][0] = 1.0f - (zz + tyy);
	rot[0][1] = txy - wz;
	rot[0][2] = twy + txz;
	rot[1][0] = wz + txy;
	rot[1][1] = 1.0f - (zz + twx);
	rot[1][2] = yz - twxw;
	rot[2][0] = txz - twy;
	rot[2][1] = yz + twxw;
	rot[2][2] = 1.0f - (tyy + twx);
}

} // namespace RBX
