#ifndef UTIL_QUATERNION_H
#define UTIL_QUATERNION_H

#include <G3D/Matrix3.h>
#include <G3D/Vector3.h>
#include <G3D/g3dmath.h>
#include <algorithm>
#include <cmath>

namespace RBX {

class Quaternion
{
public:
	float x; // 0x00
	float y; // 0x04
	float z; // 0x08
	float w; // 0x0c

	Quaternion() {}
	Quaternion(const G3D::Vector3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
	Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	Quaternion(const G3D::Matrix3& rot);

	Quaternion& operator=(const Quaternion& other);
	void toRotationMatrix(G3D::Matrix3& rot) const;

	Quaternion operator+(const Quaternion& other) const
	{
		return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	Quaternion operator-(const Quaternion& other) const
	{
		return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	// FUNCTION: WEBSERVICE 0x10120fb0
	Quaternion operator*(const Quaternion& other) const
	{
		const G3D::Vector3& v0 = imag();
		const G3D::Vector3& v1 = other.imag();
		float w0 = w;
		float w1 = other.w;

		return Quaternion(v1 * w0 + v0 * w1 + v0.cross(v1), w0 * w1 - v1.dot(v0));
	}

	const G3D::Vector3& imag() const { return *reinterpret_cast<const G3D::Vector3*>(this); }

	Quaternion operator*(float value) const { return Quaternion(value * x, value * y, value * z, value * w); }

	Quaternion& operator+=(const Quaternion& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;

		return *this;
	}

	Quaternion& operator*=(float value)
	{
		x *= value;
		y *= value;
		z *= value;
		w *= value;

		return *this;
	}

	float dot(const Quaternion& other) const { return x * other.x + y * other.y + z * other.z + w * other.w; }

	float magnitude() const { return sqrtf(dot(*this)); }

	// FUNCTION: WEBSERVICE 0x10121060
	void normalize() { *this *= 1.0f / magnitude(); }

	// FUNCTION: WEBSERVICE 0x10126ba0
	float maxComponent() const { return std::max(std::max(fabsf(x), fabsf(y)), std::max(fabsf(z), fabsf(w))); }
};

} // namespace RBX

#endif // UTIL_QUATERNION_H
