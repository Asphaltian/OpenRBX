#include "util/ExponentialRunningAverage.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x10122ff0
void floatERA::reset()
{
	avg = 0.0f;
}

// FUNCTION: WEBSERVICE 0x10123000
float floatERA::pushAndGetAverage(float value)
{
	avg = (value - avg) * weight + avg;
	return avg;
}

// FUNCTION: WEBSERVICE 0x10123020
float floatERA::getAverage() const
{
	return avg;
}

// FUNCTION: WEBSERVICE 0x10123030
void Vector3ERA::reset()
{
	z = 0.0f;
	y = 0.0f;
	x = 0.0f;
}

// FUNCTION: WEBSERVICE 0x10123040
Vector3 Vector3ERA::pushAndGetAverage(Vector3 value)
{
	x = (value.x - x) * weight + x;
	y = (value.y - y) * weight + y;
	z = (value.z - z) * weight + z;

	return Vector3(x, y, z);
}

// FUNCTION: WEBSERVICE 0x10123080
Vector3 Vector3ERA::getAverage() const
{
	return Vector3(x, y, z);
}

} // namespace RBX
