#include "v8kernel/Cofm.h"

#include "decomp.h"
#include "util/Math.h"
#include "v8kernel/Body.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x101216f0
Cofm::Cofm(Body* body) : body(body), dirty(true), cofmInBody(0, 0, 0)
{
}

// FUNCTION: WEBSERVICE 0x10121710
const float Cofm::getMass() const
{
	updateIfDirty();

	return mass;
}

// FUNCTION: WEBSERVICE 0x10121720
void Cofm::updateIfDirty() const
{
	if (dirty) {
		mass = body->getMass();

		Vector3 cofmWorld = body->getPos() * body->getMass();

		for (int i = 0; i < body->numChildren(); ++i) {
			Body* child = body->getChild(i);

			mass += child->getBranchMass();

			cofmWorld += child->getBranchCofmPos() * child->getBranchMass();
		}

		cofmWorld = cofmWorld / mass;

		cofmInBody = body->getCoordinateFrame().pointToObjectSpace(cofmWorld);

		Matrix3 iWorldSum = body->getIWorldAtPoint(cofmWorld);

		for (int i = 0; i < body->numChildren(); ++i) {
			iWorldSum = iWorldSum + body->getChild(i)->getBranchIWorldAtPoint(cofmWorld);
		}

		moment = Math::momentToObjectSpace(iWorldSum, body->getCoordinateFrame().rotation);

		dirty = false;
	}
}

// FUNCTION: WEBSERVICE 0x10121970
const Vector3& Cofm::getCofmInBody() const
{
	updateIfDirty();

	return cofmInBody;
}

// FUNCTION: WEBSERVICE 0x10121980
const Matrix3& Cofm::getMoment() const
{
	updateIfDirty();

	return moment;
}

} // namespace RBX
