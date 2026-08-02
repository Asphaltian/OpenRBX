#ifndef V8KERNEL_CONSTANTS_H
#define V8KERNEL_CONSTANTS_H

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

class Constants
{
public:
	static int kernelStepsPerWorldStep();
	static int worldStepsPerSec();
	static int kernelStepsPerSec();
	static int kernelStepsPerUiStep();

	static float uiDt();
	static float worldDt();
	static float kernelDt();

	static float getElasticMultiplier(float elasticity);

	static float getJointKMultiplier(const Vector3& size, bool isBall);

	static float getKmsMaxJointForce(float studsA, float studsB);

	static float getJointK(const Vector3& size, bool isBall);
};

} // namespace RBX

#endif // V8KERNEL_CONSTANTS_H
