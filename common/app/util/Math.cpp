#include "util/Math.h"

namespace RBX {
namespace Math {

// FUNCTION: WEBSERVICE 0x100debd0
Matrix3 fromDiagonal(const Vector3& diagonal)
{
	return Matrix3(diagonal.x, 0.0f, 0.0f, 0.0f, diagonal.y, 0.0f, 0.0f, 0.0f, diagonal.z);
}

} // namespace Math
} // namespace RBX
