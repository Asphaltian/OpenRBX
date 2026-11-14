#include "v8world/Assembly2.h"

#include "placeholder.h"
#include "util/Math.h"
#include "util/StlExtra.h"
#include "v8kernel/Body.h"
#include "v8kernel/Kernel.h"
#include "v8world/Clump2.h"
#include "v8world/IMoving.h"
#include "v8world/Joint.h"
#include "v8world/MotorJoint.h"
#include "v8world/Primitive.h"
#include "v8world/RigidJoint.h"

#include <cstddef>

namespace RBX {

SleepInfo::SleepInfo() : state(Sim::WAKE_PENDING), sleepCount(0)
{
}

SleepInfo::~SleepInfo()
{
}

PrimIterator Assembly::assemblyPrimBegin() const
{
	return PrimIterator::begin(rootPrimitive, PrimIterator::IN_ASSEMBLY);
}

PrimIterator Assembly::assemblyPrimEnd() const
{
	return PrimIterator::end(PrimIterator::IN_ASSEMBLY);
}

EdgeIterator Assembly::externalEdgeBegin() const
{
	return EdgeIterator::begin(rootPrimitive);
}

EdgeIterator Assembly::externalEdgeEnd() const
{
	return EdgeIterator::end();
}

Sim::AssemblyState Assembly::getSleepStatus() const
{
	const Assembly* root = this;

	while (root->parent != NULL) {
		root = root->parent;
	}

	return root->sleepInfo != NULL ? root->sleepInfo->state : Sim::ANCHORED;
}

bool lessAssembly(const Assembly* a0, const Assembly* a1)
{
	if (a0 == a1) {
		return false;
	}

	if (a0->getClumpPrimitive() == a1->getClumpPrimitive()) {
		return false;
	}

	return Guid::compare(&a0->getClumpPrimitive()->getGuid(), &a1->getClumpPrimitive()->getGuid()) == -1;
}

Assembly* Assembly::getRootAssembly()
{
	return parent != NULL ? parent->getRootAssembly() : this;
}

const Assembly* Assembly::getRootAssemblyConst() const
{
	return parent != NULL ? parent->getRootAssemblyConst() : this;
}

Joint* Assembly::getJointToParent(Primitive* p)
{
	Joint* joint = p->getFirstJoint();

	while (joint != NULL) {
		if (joint->getActive()) {
			Primitive* other = joint->getPrimitive(0);

			if (p == other) {
				other = joint->getPrimitive(1);
			}

			if (other == NULL || p->getBody()->getParent() == other->getBody()) {
				return joint;
			}
		}

		joint = p->getNextJoint(joint);
	}

	return NULL;
}

void Assembly::putInKernel(Kernel* kernel)
{
	putInPipeline(kernel);
	kernel->insertBody(rootPrimitive->getBody());
}

void Assembly::removeFromKernel()
{
	getKernel()->removeBody(rootPrimitive->getBody());
	IPipelined::removeFromKernel();
}

Primitive* Assembly::getAssemblyPrimitive()
{
	return getRootAssembly()->rootPrimitive;
}

const Primitive* Assembly::getAssemblyPrimitiveConst() const
{
	return getRootAssemblyConst()->rootPrimitive;
}

bool Assembly::getAnchored() const
{
	return getRootAssemblyConst()->rootPrimitive->getAnchorObject() != NULL;
}

Mechanism* Assembly::getMechanism()
{
	return mechanism;
}

bool Assembly::computeCanSleep() const
{
	for (PrimIterator it = PrimIterator::begin(rootPrimitive, PrimIterator::IN_ASSEMBLY); *it != NULL; ++it) {
		if (!(*it)->getCanSleep()) {
			return false;
		}
	}

	return true;
}

void Assembly::notifyMoved()
{
	for (PrimIterator it = PrimIterator::begin(rootPrimitive, PrimIterator::IN_ASSEMBLY); *it != NULL; ++it) {
		(*it)->getOwner()->notifyMoved();
	}
}

Assembly* Assembly::otherAssembly(Edge* edge) const
{
	Assembly* assembly0 = edge->getPrimitive(0)->getAssembly();
	Assembly* assembly1 = edge->getPrimitive(1)->getAssembly();

	return assembly0 != this ? assembly0 : assembly1;
}

void Assembly::onPrimitivesChanged()
{
	maxRadius.setDirty();
	canSleep.setDirty();

	if (parent != NULL) {
		parent->onPrimitivesChanged();
	}
}

void Assembly::addRigidChild(Primitive* parent, RigidJoint* r, Primitive* child)
{
	Clump* clump = parent->getClump();

	child->setClump(clump);
	child->setClumpDepth(parent->getClumpDepth() + 1);
	child->getBody()->setParent(parent->getBody());
	child->getBody()->setMeInParent(r->getChildInParent(parent, child));

	clump->onPrimitivesChanged();
}

unsigned int Assembly::numMotors() const
{
	unsigned int answer = 0;

	if (MotorJoint::isMotorJoint(getJointToParent(rootPrimitive))) {
		answer = 1;
	}

	for (unsigned int i = 0; i < children.size(); i++) {
		answer += children[i]->numMotors();
	}

	return answer;
}

const MotorJoint* Assembly::getMotorImp(unsigned int& motorId) const
{
	Joint* joint = getJointToParent(rootPrimitive);

	if (MotorJoint::isMotorJoint(joint)) {
		if (motorId == 0) {
			return static_cast<MotorJoint*>(joint);
		}

		motorId--;
	}

	for (unsigned int i = 0; i < children.size(); i++) {
		const MotorJoint* motor = children[i]->getMotorImp(motorId);

		if (motor != NULL) {
			return motor;
		}
	}

	return NULL;
}

MotorJoint* Assembly::getMotor(unsigned int index)
{
	unsigned int i = index;

	return const_cast<MotorJoint*>(getMotorImp(i));
}

const MotorJoint* Assembly::getMotorConst(unsigned int index) const
{
	unsigned int i = index;

	return getMotorImp(i);
}

void Assembly::stepUi(int uiStepId)
{
	unsigned int count = numMotors();

	for (unsigned int i = 0; i < count; i++) {
		unsigned int index = i;
		const_cast<MotorJoint*>(getMotorImp(index))->stepUi(uiStepId);
	}
}

float Assembly::computeMaxRadius() const
{
	float answer = 0;
	Vector3 offsetCenter = rootPrimitive->getBody()->getBranchCofmPos();

	for (PrimIterator it = PrimIterator::begin(rootPrimitive, PrimIterator::IN_ASSEMBLY); *it != NULL; ++it) {
		Primitive* primitive = *it;

		Vector3 offset = primitive->getBody()->getCoordinateFrame().translation - offsetCenter;
		Vector3 extent = Math::vector3Abs(offset);

		answer = G3D::max(answer, (extent + primitive->getGeometry()->getGridSize() * 0.5f).magnitude());
	}

	return answer;
}

void Assembly::onPrimitiveCanSleepChanged(Primitive* p)
{
	canSleep.setDirty();
}

Assembly::Assembly(Primitive* root)
	: sleepInfo(NULL), rootPrimitive(root), parent(NULL), mechanism(NULL), maxRadius(this, &Assembly::computeMaxRadius),
	  canSleep(this, &Assembly::computeCanSleep)
{
	root->setClump(static_cast<Clump*>(this));
}

void Assembly::addChild(Assembly* child)
{
	children.push_back(child);
	std::sort(children.begin(), children.end(), lessAssembly);
}

Assembly::~Assembly()
{
	for (PrimIterator it = PrimIterator::begin(rootPrimitive, PrimIterator::IN_CLUMP); *it != NULL; ++it) {
		Primitive* primitive = *it;

		primitive->setClump(NULL);
		primitive->setClumpDepth(-1);
	}

	if (parent != NULL) {
		fastRemoveShort(parent->children, this);
	}

	parent = NULL;

	for (unsigned int i = 0; i < children.size(); i++) {
		children[i]->parent = NULL;
	}
}

void Assembly::setParent(Assembly* newParent)
{
	if (newParent != parent) {
		if (parent != NULL) {
			fastRemoveShort(parent->children, this);
			parent->onPrimitivesChanged();
		}

		parent = newParent;

		if (parent != NULL) {
			parent->addChild(this);
		}

		onPrimitivesChanged();
	}
}

void Assembly::addMotorChild(Primitive* parent, MotorJoint* m, Primitive* child)
{
	Clump* clump = parent->getClump();

	child->setClumpDepth(parent->getClumpDepth() + 1);
	child->getClump()->setParent(clump);
	child->getBody()->setParent(parent->getBody());
	child->getBody()->setMeInParent(m->resetLink());
}

void Assembly::addGroundChild(Primitive* p)
{
	Clump* clump = p->getClump();

	p->setClumpDepth(1);
	p->getBody()->setParent(NULL);
	clump->setParent(NULL);
}

} // namespace RBX
