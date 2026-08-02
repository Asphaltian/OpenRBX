#include "v8kernel/Link.h"

#include "v8kernel/Body.h"

#include <cstddef>

namespace RBX {

// FUNCTION: WEBSERVICE 0x101027f0
void Link::dirty()
{
	if (body != NULL) {
		body->getRoot()->advanceStateIndex();
	}
}

// FUNCTION: WEBSERVICE 0x10102900
void Link::reset(const CoordinateFrame& parentC, const CoordinateFrame& childC)
{
	parentCoord = parentC;
	childCoord = childC;
	childCoordInverse = childC.inverse();
}

} // namespace RBX
