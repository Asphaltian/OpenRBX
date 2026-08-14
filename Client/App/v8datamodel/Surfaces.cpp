#include "v8datamodel/Surfaces.h"

namespace RBX {

// STUB: WEBSERVICE 0x10100de0
const Surface& Surfaces::operator[](NormalId normalId) const
{
	switch (normalId) {
	case NORM_X:
		return Right;
	case NORM_Z:
		return Back;
	case NORM_X_NEG:
		return Left;
	case NORM_Y_NEG:
		return Bottom;
	case NORM_Z_NEG:
		return Front;
	default:
		return Top;
	}
}

} // namespace RBX
