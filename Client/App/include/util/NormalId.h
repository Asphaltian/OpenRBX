#ifndef UTIL_NORMALID_H
#define UTIL_NORMALID_H

#include <G3D/Matrix3.h>
#include <G3D/Vector3.h>

namespace RBX {

using G3D::Matrix3;
using G3D::Vector3;

enum NormalId
{
	NORM_X = 0,
	NORM_Y = 1,
	NORM_Z = 2,
	NORM_X_NEG = 3,
	NORM_Y_NEG = 4,
	NORM_Z_NEG = 5,
	NORM_UNDEFINED = 6,
};

NormalId intToNormalId(int value);
NormalId normalIdOpposite(NormalId normalId);

template <int normalId>
Vector3 uvwToObject(const Vector3& uvw);

Vector3 uvwToObject(const Vector3& uvw, NormalId normalId);
Vector3 objectToUvw(const Vector3& object, NormalId normalId);
const Vector3& normalIdToVector3(NormalId normalId);
const Matrix3& normalIdToMatrix3(NormalId normalId);
Vector3 normalIdToVector3Internal(NormalId normalId);
Matrix3 normalIdToMatrix3Internal(NormalId normalId);
Vector3 mapToUvw_Legacy(const Vector3& uvw, NormalId normalId);
NormalId Vector3ToNormalId(const Vector3& normal);
NormalId Matrix3ToNormalId(const Matrix3& matrix);

} // namespace RBX

#endif // UTIL_NORMALID_H
