#include "util/ExponentialRunningAverage.h"

namespace RBX {

void floatERA::reset()
{
	avg = 0.0f;
}

float floatERA::pushAndGetAverage(float value)
{
	avg = (value - avg) * weight + avg;
	return avg;
}

float floatERA::getAverage()
{
	return avg;
}

void Vector3ERA::reset()
{
	z = 0.0f;
	y = 0.0f;
	x = 0.0f;
}

Vector3 Vector3ERA::pushAndGetAverage(Vector3 value)
{
	x = (value.x - x) * weight + x;
	y = (value.y - y) * weight + y;
	z = (value.z - z) * weight + z;

	return Vector3(x, y, z);
}

Vector3 Vector3ERA::getAverage()
{
	return Vector3(x, y, z);
}

} // namespace RBX
