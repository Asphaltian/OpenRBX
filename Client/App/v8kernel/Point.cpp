#include "v8kernel/Point.h"

#include "placeholder.h"
#include "v8kernel/Body.h"

#include <cstddef>

namespace RBX {

Point::Point(Body* _body) : numOwners(1), body(_body != NULL ? _body : Body::getWorldBody())
{
}

void Point::forceToBody()
{
	body->accumulateForce(force, worldPos);
}

void Point::step()
{
	worldPos = body->getPV().position.pointToWorldSpace(localPos);
	force = Vector3::zero();
}

void Point::setWorldPos(const Vector3& _worldPos)
{
	worldPos = _worldPos;
	localPos = body->getPV().position.pointToObjectSpace(worldPos);
}

} // namespace RBX
