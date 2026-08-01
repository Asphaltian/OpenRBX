#include "v8kernel/Constants.h"

#include "decomp.h"

#include <G3D/Vector3int16.h>

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

// STUB: WEBSERVICE 0x1010aa00
float Constants::getJointKMultiplier(const Vector3& size, bool isBall)
{
	G3D::Vector3int16 grid(size);

	if (isBall) {
		switch (grid.x) {
		case 1:
			return 0.23f;
		case 2:
			return 1.49f;
		case 3:
			return 4.43f;
		case 4:
			return 11.5f;
		default:
			return (float) (grid.x * grid.x * grid.x) * 0.175f;
		}
	}

	int x = grid.x;

	switch (x) {
	case 1:
		switch (grid.y) {
		case 1:
			switch (grid.z) {
			case 1:
				return 0.91f;
			case 2:
				return 1.61f;
			case 3:
				return 2.0f;
			case 4:
				return 2.13f;
			default:
				return (float) grid.z * 0.4f;
			}
		case 2: {
			int z = grid.z;

			switch (z) {
			case 2:
				return 3.5f;
			case 3:
				return 4.16f;
			case 4:
				return 4.79f;
			}

			return 15.0f <= (float) z ? (float) z * 0.75f : (float) z * 0.9f;
		}
		case 3:
			return 7.0f <= (float) grid.z ? (float) grid.z * 1.18f : (float) grid.z * 1.66f;
		case 4:
			return 7.0f <= (float) grid.z ? (float) grid.z * 1.53f : (float) grid.z * 2.26f;
		default:
			return ((float) grid.y * 0.3f + 0.66f) * (float) grid.z;
		}
	case 2:
		break;
	default:
		return (float) (grid.z * grid.y * x) * 0.25f;
	}

	int y = grid.y;
	float value;

	switch (y) {
	case 2: {
		int z = grid.z;

		switch (z) {
		case 2:
			return 7.34f;
		case 3:
			return 9.9f;
		case 4:
			return 11.22f;
		}

		value = (float) z;

		if (value < 15.0f) {
			return value * 1.9f;
		}

		break;
	}
	case 3: {
		int z = grid.z;

		switch (z) {
		case 3:
			return 15.0f;
		case 4:
			return 19.0f;
		}

		value = (float) z;

		if (value < 15.0f) {
			return value + value;
		}

		break;
	}
	default:
		return ((float) y * 0.66f) * grid.z;
	}

	return value * 1.5f;
}

// STUB: WEBSERVICE 0x1010adb0
float Constants::getJointK(const Vector3& size, bool isBall)
{
	STUB(0x1010adb0);
	return 0;
}

} // namespace RBX
