#ifndef UTIL_MATH_H
#define UTIL_MATH_H

#include <G3D/Matrix3.h>
#include <G3D/Vector3.h>

namespace RBX {

using G3D::Matrix3;
using G3D::Vector3;

namespace Math {

Matrix3 fromDiagonal(const Vector3& diagonal);

} // namespace Math
} // namespace RBX

#endif // UTIL_MATH_H
