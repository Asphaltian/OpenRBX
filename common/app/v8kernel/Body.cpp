#include "v8kernel/Body.h"

#include "decomp.h"
#include "util/Math.h"
#include "v8kernel/Link.h"
#include "v8kernel/SimBody.h"

#include <cstddef>

namespace RBX {

// FUNCTION: WEBSERVICE 0x1009ad20
void Body::updatePV() const
{
	if (parent != NULL && stateIndex != root->getStateIndex()) {
		const PV& parentPv = parent->getPV();

		pv = parentPv.pvAtLocalCoord(link != NULL ? link->getChildInParent() : meInParent);

		stateIndex = root->getStateIndex();
	}
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

// FUNCTION: WEBSERVICE 0x10104a80
void Body::step(float dt, bool throttling)
{
	if (throttling && canThrottle) {
		simBody->resetAccumulators();
		return;
	}

	simBody->step(dt);

	pv = cofm == NULL ? simBody->getPV() : simBody->getOwnerPV();

	stateIndex = getNextStateIndex();
}

// FUNCTION: WEBSERVICE 0x10104b50
void Body::setPv(const PV& value)
{
	if (parent == NULL) {

		pv = value;

		if (simBody != NULL) {
			simBody->makeDirty();
		}

		advanceStateIndex();
	}
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

// FUNCTION: WEBSERVICE 0x10104db0
void Body::setMeInParent(const CoordinateFrame& value)
{
	if (link != NULL) {

		link->setBody(NULL);
		link = NULL;
	}

	if (parent != NULL) {
		meInParent = value;
		makeCofmDirty();
		root->advanceStateIndex();
	}
}

// FUNCTION: WEBSERVICE 0x10104e60
void Body::setMeInParent(Link* value)
{
	if (link != NULL && link != value) {
		link->setBody(NULL);
	}

	if (parent != NULL) {
		link = value;
		value->setBody(this);
		makeCofmDirty();
		root->advanceStateIndex();
	}
}

// FUNCTION: WEBSERVICE 0x10104ee0
void Body::setCoordinateFrame(const CoordinateFrame& value)
{
	setPv(PV(value, getPV().velocity));
}

// FUNCTION: WEBSERVICE 0x10104f60
Vector3 Body::getBranchCofmPos() const
{
	return cofm != NULL ? getPV().position.pointToWorldSpace(cofm->getCofmInBody()) : getPV().position.translation;
}

// FUNCTION: WEBSERVICE 0x10104fc0
CoordinateFrame Body::getBranchCofmCoordinateFrame() const
{
	return CoordinateFrame(getCoordinateFrame().rotation, getBranchCofmPos());
}

// FUNCTION: WEBSERVICE 0x10105010
void Body::resetRoot(Body* value)
{
	root = value;

	for (int i = 0; i < numChildren(); ++i) {
		getChild(i)->resetRoot(value);
	}
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

// FUNCTION: WEBSERVICE 0x10105150
float Body::kineticEnergy() const
{
	return ((getIWorld() * pv.velocity.rotational).dot(pv.velocity.rotational) +
			pv.velocity.linear.dot(pv.velocity.linear) * mass) *
		   0.5f;
}

// FUNCTION: WEBSERVICE 0x10105240
void Body::onChildRemoved(Body* child)
{
	children.fastRemove(child);

	if (numChildren() == 0) {
		delete cofm;
		cofm = NULL;
	}

	makeCofmDirty();
}

// FUNCTION: WEBSERVICE 0x101052d0
void Body::onChildAdded(Body* child)
{
	makeCofmDirty();

	children.fastAppend(child);

	if (cofm == NULL) {
		cofm = new Cofm(this);
	}
}

// FUNCTION: WEBSERVICE 0x10105390
void Body::setParent(Body* value)
{
	if (parent != value) {

		if (link != NULL) {
			link->setBody(NULL);
			link = NULL;
		}

		if (parent != NULL) {
			parent->onChildRemoved(this);
		}
		else {
			delete simBody;
			simBody = NULL;
		}

		parent = value;

		if (value != NULL) {
			value->onChildAdded(this);
		}
		else {
			simBody = new SimBody(this);
		}

		Body* newRoot = calcRoot();

		newRoot->advanceStateIndex();

		resetRoot(newRoot);
	}
}

// FUNCTION: WEBSERVICE 0x101054a0
Body::Body()
	: root(NULL), parent(NULL), index(-1), cofm(NULL), simBody(NULL), canThrottle(true), link(NULL),
	  moment(Matrix3::identity()), mass(0.0f), stateIndex(getNextStateIndex())
{
	root = this;
	simBody = new SimBody(this);
}

// FUNCTION: WEBSERVICE 0x101055a0
Body::~Body()
{
	if (parent != NULL) {
		setParent(NULL);
	}

	delete simBody;
	simBody = NULL;
}

// FUNCTION: WEBSERVICE 0x10105620
Body* Body::getWorldBody()
{
	static Body b;

	return &b;
}

} // namespace RBX
