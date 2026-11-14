#include "v8kernel/Link.h"

#include "util/Math.h"
#include "v8kernel/Body.h"

#include <cstddef>

namespace RBX {

void Link::dirty()
{
	if (body != NULL) {
		body->getRoot()->advanceStateIndex();
	}
}

void RevoluteLink::computeChildInParent(CoordinateFrame& answer) const
{
	CoordinateFrame rotatedParentCoord(Math::rotateAboutZ(parentCoord.rotation, jointAngle), parentCoord.translation);

	answer = rotatedParentCoord * childCoordInverse;
}

Link::Link() : body(NULL), stateIndex(Body::getNextStateIndex())
{
}

const CoordinateFrame& Link::getChildInParent() const
{
	int index = body->getParent()->getStateIndex();

	if (stateIndex != index) {
		computeChildInParent(childInParent);

		stateIndex = index;
	}

	return childInParent;
}

void Link::reset(const CoordinateFrame& parentC, const CoordinateFrame& childC)
{
	parentCoord = parentC;
	childCoord = childC;
	childCoordInverse = childC.inverse();
}

Link::~Link()
{
}

} // namespace RBX
