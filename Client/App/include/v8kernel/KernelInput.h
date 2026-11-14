#ifndef V8KERNEL_KERNELINPUT_H
#define V8KERNEL_KERNELINPUT_H

#include "placeholder.h"
#include "v8kernel/Constants.h"

namespace RBX {

class KernelInput
{
public:
	KernelInput() : lastGoal(0.0f), currentGoal(0.0f), increment(0.0f), latchToZero(false) {}

	void setGoal(float value)
	{
		if (value == lastGoal) {
			currentGoal = value;
			increment = 0.0f;
		}
		else {
			lastGoal = value;
			increment = (value - currentGoal) / steps();
		}
	}

	void setDelta(float value)
	{
		increment = value / steps();

		if (increment == 0.0f) {
			latchToZero = true;
		}
	}

	float get()
	{
		float answer = currentGoal;

		currentGoal = increment + currentGoal;

		return answer;
	}

private:
	int steps() { return Constants::kernelStepsPerUiStep(); }

	float lastGoal;
	float currentGoal;
	float increment;
	bool latchToZero;

	friend class RotateConnector;
};

} // namespace RBX

#endif // V8KERNEL_KERNELINPUT_H
