#include "v8kernel/Constants.h"

#include "decomp.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x1010a920
int Constants::kernelStepsPerWorldStep()
{
	return 19;
}

// FUNCTION: WEBSERVICE 0x1010a930
int Constants::worldStepsPerSec()
{
	return 240;
}

// FUNCTION: WEBSERVICE 0x1010a940
int Constants::kernelStepsPerSec()
{
	return 4560;
}

// FUNCTION: WEBSERVICE 0x1010a950
int Constants::kernelStepsPerUiStep()
{
	return 152;
}

// FUNCTION: WEBSERVICE 0x1010a960
float Constants::uiDt()
{
	return 0.033333335f;
}

// FUNCTION: WEBSERVICE 0x1010a970
float Constants::worldDt()
{
	return 0.004166667f;
}

// FUNCTION: WEBSERVICE 0x1010a980
float Constants::kernelDt()
{
	return 0.00021929825f;
}

// FUNCTION: WEBSERVICE 0x1010a990
float Constants::getElasticMultiplier(float elasticity)
{
	if (elasticity < 0.05f) {
		return 0.28f;
	}

	if (elasticity < 0.26f) {
		return 0.42f;
	}

	if (elasticity < 0.51f) {
		return 0.57f;
	}

	if (elasticity < 0.76f) {
		return 0.8f;
	}

	return 1.0f;
}

// STUB: WEBSERVICE 0x1010adb0
float Constants::getJointK(const Vector3& size, bool isBall)
{
	STUB(0x1010adb0);
	return 0;
}

} // namespace RBX
