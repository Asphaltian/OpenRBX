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
float Cofm::getMass()
{
	updateIfDirty();

	return mass;
}

// FUNCTION: WEBSERVICE 0x10121720
void Cofm::updateIfDirty()
{
	if (dirty) {
		mass = body->getMass();

		Vector3 cofmPos = body->getPos() * body->getMass();

		for (int i = 0; i < body->numChildren(); ++i) {
			Body* child = body->getChild(i);

			mass += child->getBranchMass();

			cofmPos += child->getBranchCofmPos() * child->getBranchMass();
		}

		cofmPos = cofmPos / mass;

		cofmInBody = body->getCoordinateFrame().pointToObjectSpace(cofmPos);

		Matrix3 iWorld = body->getIWorldAtPoint(cofmPos);

		for (int i = 0; i < body->numChildren(); ++i) {
			iWorld = iWorld + body->getChild(i)->getBranchIWorldAtPoint(cofmPos);
		}

		moment = Math::momentToObjectSpace(iWorld, body->getCoordinateFrame().rotation);

		dirty = false;
	}
}

// FUNCTION: WEBSERVICE 0x10121970
const Vector3& Cofm::getCofmInBody()
{
	updateIfDirty();

	return cofmInBody;
}

// FUNCTION: WEBSERVICE 0x10121980
const Matrix3& Cofm::getMoment()
{
	updateIfDirty();

	return moment;
}

} // namespace RBX
