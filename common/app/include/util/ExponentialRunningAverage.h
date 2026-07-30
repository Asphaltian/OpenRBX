#ifndef UTIL_EXPONENTIALRUNNINGAVERAGE_H
#define UTIL_EXPONENTIALRUNNINGAVERAGE_H

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

class floatERA
{
public:
	void reset(float value);
	float pushAndGetAverage(float value);
	float getAverage() const;

private:
	float weight; // 0x00
	float avg;    // 0x04
};

class Vector3ERA
{
public:
	void reset(const Vector3& value);
	Vector3 pushAndGetAverage(Vector3 value);
	Vector3 getAverage() const;

private:
	float weight; // 0x00
	float x;      // 0x04
	float y;      // 0x08
	float z;      // 0x0c
};

} // namespace RBX

#endif // UTIL_EXPONENTIALRUNNINGAVERAGE_H
