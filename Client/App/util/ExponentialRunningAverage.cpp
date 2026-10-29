#include "util/ExponentialRunningAverage.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x10122ff0
// FUNCTION: ROBLOX 0x00663060
void floatERA::reset()
{
	avg = 0.0f;
}

// FUNCTION: WEBSERVICE 0x10123000
// FUNCTION: ROBLOX 0x00663070
float floatERA::pushAndGetAverage(float value)
{
	avg = (value - avg) * weight + avg;
	return avg;
}

// FUNCTION: WEBSERVICE 0x10123020
// FUNCTION: ROBLOX 0x00663090
float floatERA::getAverage()
{
	return avg;
}

// FUNCTION: WEBSERVICE 0x10123030
// FUNCTION: ROBLOX 0x006630a0
void Vector3ERA::reset()
{
	z = 0.0f;
	y = 0.0f;
	x = 0.0f;
}

// FUNCTION: WEBSERVICE 0x10123040
// FUNCTION: ROBLOX 0x006630b0
Vector3 Vector3ERA::pushAndGetAverage(Vector3 value)
{
	x = (value.x - x) * weight + x;
	y = (value.y - y) * weight + y;
	z = (value.z - z) * weight + z;

	return Vector3(x, y, z);
}

// FUNCTION: WEBSERVICE 0x10123080
// FUNCTION: ROBLOX 0x006630f0
Vector3 Vector3ERA::getAverage()
{
	return Vector3(x, y, z);
}

} // namespace RBX
