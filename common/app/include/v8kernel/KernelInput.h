#ifndef V8KERNEL_KERNELINPUT_H
#define V8KERNEL_KERNELINPUT_H

#include "decomp.h"

namespace RBX {

// SIZE 0x10
class KernelInput
{
public:
	KernelInput() : lastGoal(0.0f), currentGoal(0.0f), increment(0.0f), latchToZero(false) {}

	void setGoal(float value);
	void setDelta(float value);

	float get()
	{
		float answer = currentGoal;

		currentGoal = increment + currentGoal;

		return answer;
	}

private:
	int steps();

	float lastGoal;    // 0x00
	float currentGoal; // 0x04
	float increment;   // 0x08
	bool latchToZero;  // 0x0c

	friend class RotateConnector;
};

DECOMP_SIZE_ASSERT(KernelInput, 0x10)

} // namespace RBX

#endif // V8KERNEL_KERNELINPUT_H
