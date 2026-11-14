#include "v8kernel/Body.h"

#include "placeholder.h"
#include "util/Math.h"
#include "v8kernel/Link.h"
#include "v8kernel/SimBody.h"

#include <cstddef>

namespace RBX {

void Body::updatePV() const
{
	if (parent != NULL && stateIndex != root->getStateIndex()) {
		const PV& parentPv = parent->getPV();

		pv = parentPv.pvAtLocalCoord(link != NULL ? link->getChildInParent() : meInParent);

		stateIndex = root->getStateIndex();
	}
}

int Body::getNextStateIndex()
{
	static int p = 1;

	p = p + 1;

	if (p == 0x7fffffff) {
		p = 1;
	}

	return p;
}

void Body::advanceStateIndex()
{
	stateIndex = getNextStateIndex();
}

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

void Body::setPv(const PV& _pv)
{
	if (parent == NULL) {

		pv = _pv;

		if (simBody != NULL) {
			simBody->makeDirty();
		}

		advanceStateIndex();
	}
}

void Body::setVelocity(const Velocity& worldVelocity)
{
	if (parent == NULL) {
		pv.velocity = worldVelocity;
		advanceStateIndex();

		if (simBody != NULL) {
			simBody->makeDirty();
		}
	}
}

void Body::setMass(float _mass)
{
	if (mass != _mass) {
		makeCofmDirty();

		mass = _mass;
	}
}

void Body::setMoment(const Matrix3& _momentInBody)
{
	if (moment != _momentInBody) {
		makeCofmDirty();

		moment = _momentInBody;
	}
}

void Body::setMeInParent(const CoordinateFrame& _meInParent)
{
	if (link != NULL) {

		link->setBody(NULL);
		link = NULL;
	}

	if (parent != NULL) {
		meInParent = _meInParent;
		makeCofmDirty();
		root->advanceStateIndex();
	}
}

void Body::setMeInParent(Link* _link)
{
	if (link != NULL && link != _link) {
		link->setBody(NULL);
	}

	if (parent != NULL) {
		link = _link;
		_link->setBody(this);
		makeCofmDirty();
		root->advanceStateIndex();
	}
}

void Body::setCoordinateFrame(const CoordinateFrame& worldCoord)
{
	setPv(PV(worldCoord, getPV().velocity));
}

Vector3 Body::getBranchCofmPos() const
{
	return cofm != NULL ? getPV().position.pointToWorldSpace(cofm->getCofmInBody()) : getPV().position.translation;
}

CoordinateFrame Body::getBranchCofmCoordinateFrame() const
{
	return CoordinateFrame(getCoordinateFrame().rotation, getBranchCofmPos());
}

void Body::resetRoot(Body* newRoot)
{
	root = newRoot;

	for (int i = 0; i < numChildren(); ++i) {
		getChild(i)->resetRoot(newRoot);
	}
}

Matrix3 Body::getIWorldAtPoint(const Vector3& point) const
{
	return Math::getIWorldAtPoint(getPV().position.translation, point, getIWorld(), getMass());
}

Matrix3 Body::getBranchIWorldAtPoint(const Vector3& point) const
{
	return Math::getIWorldAtPoint(getBranchCofmPos(), point, getBranchIWorld(), getBranchMass());
}

float Body::kineticEnergy() const
{
	return ((getIWorld() * pv.velocity.rotational).dot(pv.velocity.rotational) +
			pv.velocity.linear.dot(pv.velocity.linear) * mass) *
		   0.5f;
}

void Body::onChildRemoved(Body* child)
{
	children.fastRemove(child);

	if (numChildren() == 0) {
		delete cofm;
		cofm = NULL;
	}

	makeCofmDirty();
}

void Body::onChildAdded(Body* child)
{
	makeCofmDirty();

	children.fastAppend(child);

	if (cofm == NULL) {
		cofm = new Cofm(this);
	}
}

void Body::setParent(Body* newParent)
{
	if (parent != newParent) {

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

		parent = newParent;

		if (newParent != NULL) {
			newParent->onChildAdded(this);
		}
		else {
			simBody = new SimBody(this);
		}

		Body* newRoot = calcRoot();

		newRoot->advanceStateIndex();

		resetRoot(newRoot);
	}
}

Body::Body()
	: root(NULL), parent(NULL), index(-1), cofm(NULL), simBody(NULL), canThrottle(true), link(NULL),
	  moment(Matrix3::identity()), mass(0.0f), stateIndex(getNextStateIndex())
{
	root = this;
	simBody = new SimBody(this);
}

Body::~Body()
{
	if (parent != NULL) {
		setParent(NULL);
	}

	delete simBody;
	simBody = NULL;
}

Body* Body::getWorldBody()
{
	static Body b;

	return &b;
}

} // namespace RBX
