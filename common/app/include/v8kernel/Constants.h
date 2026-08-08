#ifndef V8KERNEL_CONSTANTS_H
#define V8KERNEL_CONSTANTS_H

#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

class Constants
{
public:
	static const int worldStepsPerUiStep();
	static const int kernelStepsPerWorldStep();
	static const int worldStepsPerSec();
	static const int kernelStepsPerSec();
	static const int kernelStepsPerUiStep();

	static const float uiDt();
	static const float worldDt();
	static const float kernelDt();

	static const float getElasticMultiplier(float elasticity);

private:
	static const float getJointKMultiplier(const Vector3& size, bool isBall);

public:
	static const float getKmsMaxJointForce(float studsA, float studsB);

	static const float getJointK(const Vector3& size, bool isBall);

	static const Vector3& getKmsGravity()
	{
		static Vector3 g(0.0f, -9.81f, 0.0f);

		return g;
	}
};

} // namespace RBX

#endif // V8KERNEL_CONSTANTS_H
