#include "util/NormalId.h"

#include "decomp.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x1007cf30
NormalId intToNormalId(int value)
{
	return (NormalId) value;
}

// FUNCTION: WEBSERVICE 0x101039d0
NormalId normalIdOpposite(NormalId normalId)
{
	return (NormalId) ((normalId + NORM_X_NEG) % 6);
}

// FUNCTION: WEBSERVICE 0x101039f0
template <>
Vector3 uvwToObject<NORM_X>(const Vector3& uvw)
{
	return Vector3(uvw.z, uvw.y, -uvw.x);
}

// FUNCTION: WEBSERVICE 0x10103a10
template <>
Vector3 uvwToObject<NORM_Y>(const Vector3& uvw)
{
	return Vector3(-uvw.x, uvw.z, uvw.y);
}

// FUNCTION: WEBSERVICE 0x10103a30
template <>
Vector3 uvwToObject<NORM_Z>(const Vector3& uvw)
{
	return Vector3(uvw.x, uvw.y, uvw.z);
}

// FUNCTION: WEBSERVICE 0x10103a50
template <>
Vector3 uvwToObject<NORM_X_NEG>(const Vector3& uvw)
{
	return Vector3(-uvw.z, uvw.y, uvw.x);
}

// FUNCTION: WEBSERVICE 0x10103a70
template <>
Vector3 uvwToObject<NORM_Y_NEG>(const Vector3& uvw)
{
	return Vector3(uvw.x, -uvw.z, uvw.y);
}

// FUNCTION: WEBSERVICE 0x10103a90
template <>
Vector3 uvwToObject<NORM_Z_NEG>(const Vector3& uvw)
{
	return Vector3(-uvw.x, uvw.y, -uvw.z);
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

// FUNCTION: WEBSERVICE 0x10103ba0
Vector3 uvwToObject(const Vector3& uvw, NormalId normalId)
{
	switch (normalId) {
	case NORM_X:
		return uvwToObject<NORM_X>(uvw);
	case NORM_Y:
		return uvwToObject<NORM_Y>(uvw);
	case NORM_Z:
		return uvwToObject<NORM_Z>(uvw);
	case NORM_X_NEG:
		return uvwToObject<NORM_X_NEG>(uvw);
	case NORM_Y_NEG:
		return uvwToObject<NORM_Y_NEG>(uvw);
	case NORM_Z_NEG:
		return uvwToObject<NORM_Z_NEG>(uvw);
	}

	return Vector3::unitX();
}

// FUNCTION: WEBSERVICE 0x10103cc0
Vector3 objectToUvw(const Vector3& object, NormalId normalId)
{
	switch (normalId) {
	case NORM_X:
		return Vector3(-object.z, object.y, object.x);
	case NORM_Y:
		return Vector3(-object.x, object.z, object.y);
	case NORM_Z:
		return Vector3(object.x, object.y, object.z);
	case NORM_X_NEG:
		return Vector3(object.z, object.y, -object.x);
	case NORM_Y_NEG:
		return Vector3(object.x, object.z, -object.y);
	case NORM_Z_NEG:
		return Vector3(-object.x, object.y, -object.z);
	}

	return Vector3::unitX();
}

// STUB: WEBSERVICE 0x10103de0
const Vector3& normalIdToVector3(NormalId normalId)
{
	switch (normalId) {
	case NORM_X: {
		static const Vector3 x = normalIdToVector3Internal(NORM_X);
		return x;
	}
	case NORM_Y: {
		static const Vector3 y = normalIdToVector3Internal(NORM_Y);
		return y;
	}
	case NORM_Z: {
		static const Vector3 z = normalIdToVector3Internal(NORM_Z);
		return z;
	}
	case NORM_X_NEG: {
		static const Vector3 xn = normalIdToVector3Internal(NORM_X_NEG);
		return xn;
	}
	case NORM_Y_NEG: {
		static const Vector3 yn = normalIdToVector3Internal(NORM_Y_NEG);
		return yn;
	}
	case NORM_Z_NEG: {
		static const Vector3 zn = normalIdToVector3Internal(NORM_Z_NEG);
		return zn;
	}
	}

	return Vector3::zero();
}

// FUNCTION: WEBSERVICE 0x10103f50
Matrix3 normalIdToMatrix3Internal(NormalId normalId)
{
	const Vector3 x = uvwToObject(Vector3::unitX(), normalId);
	const Vector3 y = uvwToObject(Vector3::unitY(), normalId);
	const Vector3 z = uvwToObject(Vector3::unitZ(), normalId);

	return Matrix3(x.x, y.x, z.x, x.y, y.y, z.y, x.z, y.z, z.z);
}

// STUB: WEBSERVICE 0x10104070
const Matrix3& normalIdToMatrix3(NormalId normalId)
{
	switch (normalId) {
	case NORM_X: {
		static const Matrix3 x = normalIdToMatrix3Internal(NORM_X);
		return x;
	}
	case NORM_Y: {
		static const Matrix3 y = normalIdToMatrix3Internal(NORM_Y);
		return y;
	}
	case NORM_Z: {
		static const Matrix3 z = normalIdToMatrix3Internal(NORM_Z);
		return z;
	}
	case NORM_X_NEG: {
		static const Matrix3 xn = normalIdToMatrix3Internal(NORM_X_NEG);
		return xn;
	}
	case NORM_Y_NEG: {
		static const Matrix3 yn = normalIdToMatrix3Internal(NORM_Y_NEG);
		return yn;
	}
	case NORM_Z_NEG: {
		static const Matrix3 zn = normalIdToMatrix3Internal(NORM_Z_NEG);
		return zn;
	}
	}

	return Matrix3::identity();
}

// FUNCTION: WEBSERVICE 0x10104240
Vector3 mapToUvw_Legacy(const Vector3& uvw, NormalId normalId)
{
	return uvwToObject(uvw, normalId);
}

} // namespace RBX
