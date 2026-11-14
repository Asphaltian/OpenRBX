#ifndef UTIL_EXPONENTIALRUNNINGAVERAGE_H
#define UTIL_EXPONENTIALRUNNINGAVERAGE_H

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

class floatERA
{
public:
	floatERA(float weight) : weight(weight) { reset(); }

	void reset();
	float pushAndGetAverage(float value);
	float getAverage();

private:
	float weight;
	float avg;
};

class Vector3ERA
{
public:
	Vector3ERA(float weight) : weight(weight) { reset(); }

	void reset();
	Vector3 pushAndGetAverage(Vector3 value);
	Vector3 getAverage();

private:
	float weight;
	float x;
	float y;
	float z;
};

} // namespace RBX

#endif // UTIL_EXPONENTIALRUNNINGAVERAGE_H
