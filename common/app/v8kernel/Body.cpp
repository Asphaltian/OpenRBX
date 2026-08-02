#include "v8kernel/Body.h"

#include "decomp.h"
#include "util/Math.h"

#include <cstddef>

namespace RBX {

// STUB: WEBSERVICE 0x1009ad20
DECOMP_NOINLINE void Body::updatePV() const
{
	STUB(0x1009ad20);
}

// FUNCTION: WEBSERVICE 0x101049f0
int Body::getNextStateIndex()
{
	static int p = 1;

	p = p + 1;

	if (p == 0x7fffffff) {
		p = 1;
	}

	return p;
}

// FUNCTION: WEBSERVICE 0x10104a10
void Body::advanceStateIndex()
{
	stateIndex = getNextStateIndex();
}

// FUNCTION: WEBSERVICE 0x10104a40
void Body::makeCofmDirty()
{
	if (cofm == NULL || !cofm->getIsDirty()) {

		if (parent != NULL) {

			parent->makeCofmDirty();
		}
		else {

			if (simBody != NULL) {
				simBody->makeDirty();
			}
		}

		if (cofm != NULL) {
			cofm->makeDirty();
		}
	}
}

// STUB: WEBSERVICE 0x10104b50
DECOMP_NOINLINE void Body::setPv(const PV& value)
{
	STUB(0x10104b50);
}

// FUNCTION: WEBSERVICE 0x10104be0
void Body::setVelocity(const Velocity& velocity)
{
	if (parent == NULL) {
		pv.velocity = velocity;
		advanceStateIndex();

		if (simBody != NULL) {
			simBody->makeDirty();
		}
	}
}

// FUNCTION: WEBSERVICE 0x10104c60
void Body::setMass(float value)
{
	if (mass != value) {
		makeCofmDirty();

		mass = value;
	}
}

// FUNCTION: WEBSERVICE 0x10104cc0
void Body::setMoment(const Matrix3& value)
{
	if (moment != value) {
		makeCofmDirty();

		moment = value;
	}
}

// STUB: WEBSERVICE 0x10104db0
void Body::setMeInParent(const CoordinateFrame& value)
{
	STUB(0x10104db0);
}

// STUB: WEBSERVICE 0x10104e60
void Body::setMeInParent(Link* link)
{
	STUB(0x10104e60);
}

// STUB: WEBSERVICE 0x10104ee0
void Body::setCoordinateFrame(const CoordinateFrame& value)
{
	setPv(PV(value, getPV().velocity));
}

// STUB: WEBSERVICE 0x10104f60
Vector3 Body::getBranchCofmPos() const
{
	return cofm == NULL ? getPV().position.translation : getPV().position.pointToWorldSpace(cofm->getCofmInBody());
}

// FUNCTION: WEBSERVICE 0x10105040
Matrix3 Body::getIWorldAtPoint(const Vector3& point) const
{
	return Math::getIWorldAtPoint(getPV().position.translation, point, getIWorld(), getMass());
}

// FUNCTION: WEBSERVICE 0x101050c0
Matrix3 Body::getBranchIWorldAtPoint(const Vector3& point) const
{
	return Math::getIWorldAtPoint(getBranchCofmPos(), point, getBranchIWorld(), getBranchMass());
}

// STUB: WEBSERVICE 0x10105390
void Body::setParent(Body* value)
{
	STUB(0x10105390);
}

// STUB: WEBSERVICE 0x101054a0
Body::Body()
{
	STUB(0x101054a0);
}

// STUB: WEBSERVICE 0x101055a0
Body::~Body()
{
	STUB(0x101055a0);
}

} // namespace RBX
