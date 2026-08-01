#include "util/Math.h"

#include "decomp.h"

namespace RBX {
namespace Math {

// FUNCTION: WEBSERVICE 0x100debd0
Matrix3 fromDiagonal(const Vector3& diagonal)
{
	return Matrix3(diagonal.x, 0.0f, 0.0f, 0.0f, diagonal.y, 0.0f, 0.0f, 0.0f, diagonal.z);
}

// STUB: WEBSERVICE 0x100df600
Vector3 vector3Abs(const Vector3& value)
{
	STUB(0x100df600);
	return Vector3::zero();
}

} // namespace Math
} // namespace RBX
