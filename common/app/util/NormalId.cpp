#include "util/NormalId.h"

#include "decomp.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x101039d0
NormalId normalIdOpposite(NormalId normalId)
{
	return (NormalId) ((normalId + NORM_X_NEG) % 6);
}

// FUNCTION: WEBSERVICE 0x10103ab0
Vector3 normalIdToVector3Internal(NormalId normalId)
{
	Vector3 answer;
	answer.x = 0.0f;
	answer.y = 0.0f;
	answer.z = 0.0f;

	int axis = normalId % 3;

	if (normalId < 3) {
		(&answer.x)[axis] = 1.0f;
	}
	else {
		(&answer.x)[axis] = -1.0f;
	}

	return answer;
}

// FUNCTION: WEBSERVICE 0x10103b00
NormalId Vector3ToNormalId(const Vector3& normal)
{
	if (normal.x == 1.0f) {
		return NORM_X;
	}
	if (normal.y == 1.0f) {
		return NORM_Y;
	}
	if (normal.z == 1.0f) {
		return NORM_Z;
	}
	if (normal.x == -1.0f) {
		return NORM_X_NEG;
	}
	if (normal.y == -1.0f) {
		return NORM_Y_NEG;
	}

	return normal.z == -1.0f ? NORM_Z_NEG : NORM_UNDEFINED;
}

// FUNCTION: WEBSERVICE 0x10103b80
NormalId Matrix3ToNormalId(const Matrix3& matrix)
{
	return Vector3ToNormalId(matrix.getColumn(2));
}

// STUB: WEBSERVICE 0x10103de0
const Vector3& normalIdToVector3(NormalId normalId)
{
	STUB(0x10103de0);
	return Vector3::zero();
}

// STUB: WEBSERVICE 0x10104070
const Matrix3& normalIdToMatrix3(NormalId normalId)
{
	STUB(0x10104070);
	return Matrix3::zero();
}

} // namespace RBX
