#ifndef V8KERNEL_CONSTANTS_H
#define V8KERNEL_CONSTANTS_H

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

class Constants
{
public:
	static int worldStepsPerUiStep();
	static int kernelStepsPerWorldStep();
	static int worldStepsPerSec();
	static int kernelStepsPerSec();
	static int kernelStepsPerUiStep();

	static float uiDt();
	static float worldDt();
	static float kernelDt();

	static float getElasticMultiplier(float elasticity);

private:
	static float getJointKMultiplier(const Vector3& size, bool isBall);

public:
	static float getKmsMaxJointForce(float studsA, float studsB);

	static float getJointK(const Vector3& size, bool isBall);

	static const Vector3& getKmsGravity()
	{
		static Vector3 g(0.0f, -9.81f, 0.0f);

		return g;
	}
};

} // namespace RBX

#endif // V8KERNEL_CONSTANTS_H
