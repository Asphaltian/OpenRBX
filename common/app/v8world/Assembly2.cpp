#include "v8world/Assembly2.h"

#include "decomp.h"
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

// FUNCTION: WEBSERVICE 0x10102990
SleepInfo::SleepInfo() : state(Sim::WAKE_PENDING), sleepCount(0)
{
}

// FUNCTION: WEBSERVICE 0x10102a60
Sim::AssemblyState Assembly::getSleepStatus()
{
	Assembly* root = this;

	while (root->parent != NULL) {
		root = root->parent;
	}

	return root->sleepInfo != NULL ? root->sleepInfo->state : Sim::ANCHORED;
}

// FUNCTION: WEBSERVICE 0x10102a80
bool lessAssembly(const Assembly* a, const Assembly* b)
{
	if (a == b) {
		return false;
	}

	if (a->getRootPrimitive() == b->getRootPrimitive()) {
		return false;
	}

	return Guid::compare(a->getRootPrimitive()->getGuid(), b->getRootPrimitive()->getGuid()) == -1;
}

// FUNCTION: WEBSERVICE 0x10102ac0 FOLDED
Assembly* Assembly::getRootAssembly()
{
	return parent != NULL ? parent->getRootAssembly() : this;
}

// FUNCTION: WEBSERVICE 0x10102ad0
Joint* Assembly::getJointToParent(Primitive* primitive)
{
	Joint* joint = primitive->getFirstJoint();

	while (joint != NULL) {
		if (joint->getActive()) {
			Primitive* other = joint->getPrimitive(0);

			if (primitive == other) {
				other = joint->getPrimitive(1);
			}

			if (other == NULL || primitive->getBody()->getParent() == other->getBody()) {
				return joint;
			}
		}

		joint = primitive->getNextJoint(joint);
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x10102b10
void Assembly::putInKernel(Kernel* kernel)
{
	putInPipeline(kernel);
	kernel->insertBody(rootPrimitive->getBody());
}

// FUNCTION: WEBSERVICE 0x10102b40
void Assembly::removeFromKernel()
{
	getKernel()->removeBody(rootPrimitive->getBody());
	IPipelined::removeFromKernel();
}

// FUNCTION: WEBSERVICE 0x10102b60
Primitive* Assembly::getAssemblyPrimitive()
{
	return getRootAssembly()->rootPrimitive;
}

// FUNCTION: WEBSERVICE 0x10102b80
bool Assembly::getAnchored()
{
	return getRootAssembly()->rootPrimitive->getAnchorObject() != NULL;
}

// FUNCTION: WEBSERVICE 0x10102ba0
Mechanism* Assembly::getMechanism()
{
	return mechanism;
}

// FUNCTION: WEBSERVICE 0x10102bb0
bool Assembly::computeCanSleep()
{
	for (PrimIterator it(rootPrimitive, IN_ASSEMBLY); *it != NULL; ++it) {
		if (!(*it)->getCanSleep()) {
			return false;
		}
	}

	return true;
}

// FUNCTION: WEBSERVICE 0x10102bf0
void Assembly::notifyMoved()
{
	for (PrimIterator it(rootPrimitive, IN_ASSEMBLY); *it != NULL; ++it) {
		(*it)->getOwner()->notifyMoved();
	}
}

// FUNCTION: WEBSERVICE 0x10102c20
Assembly* Assembly::otherAssembly(Edge* edge)
{
	Assembly* assembly0 = edge->getPrimitive(0)->getAssembly();
	Assembly* assembly1 = edge->getPrimitive(1)->getAssembly();

	return assembly0 != this ? assembly0 : assembly1;
}

// FUNCTION: WEBSERVICE 0x10102d50
void Assembly::onPrimitivesChanged()
{
	maxRadius.setDirty();
	canSleep.setDirty();

	if (parent != NULL) {
		parent->onPrimitivesChanged();
	}
}

// FUNCTION: WEBSERVICE 0x10102de0
unsigned int Assembly::numMotors()
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

// FUNCTION: WEBSERVICE 0x10102e60
MotorJoint* Assembly::getMotorImp(unsigned int& index)
{
	Joint* joint = getJointToParent(rootPrimitive);

	if (MotorJoint::isMotorJoint(joint)) {
		if (index == 0) {
			return static_cast<MotorJoint*>(joint);
		}

		index--;
	}

	for (unsigned int i = 0; i < children.size(); i++) {
		MotorJoint* motor = children[i]->getMotorImp(index);

		if (motor != NULL) {
			return motor;
		}
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x10102f00
MotorJoint* Assembly::getMotor(unsigned int index)
{
	unsigned int i = index;

	return getMotorImp(i);
}

// FUNCTION: WEBSERVICE 0x10102f20
void Assembly::stepUi(int frameCount)
{
	unsigned int count = numMotors();

	for (unsigned int i = 0; i < count; i++) {
		unsigned int index = i;
		getMotorImp(index)->stepUi(frameCount);
	}
}

// FUNCTION: WEBSERVICE 0x10103060
void Assembly::onPrimitiveCanSleepChanged(Primitive* primitive)
{
	canSleep.setDirty();
}

// STUB: WEBSERVICE 0x10103750
void Assembly::addChild(Assembly* child)
{
	children.push_back(child);
	std::sort(children.begin(), children.end(), lessAssembly);
}

// FUNCTION: WEBSERVICE 0x101038a0
void Assembly::setParent(Assembly* value)
{
	if (value != parent) {
		if (parent != NULL) {
			fastRemoveShort(parent->children, this);
			parent->onPrimitivesChanged();
		}

		parent = value;

		if (parent != NULL) {
			parent->addChild(this);
		}

		onPrimitivesChanged();
	}
}

// FUNCTION: WEBSERVICE 0x10103910
void Assembly::addMotorChild(Primitive* parent, MotorJoint* joint, Primitive* child)
{
	Clump* clump = parent->getClump();

	child->setClumpDepth(parent->getClumpDepth() + 1);
	child->getClump()->setParent(clump);
	child->getBody()->setParent(parent->getBody());
	child->getBody()->setMeInParent(joint->resetLink());
}

// FUNCTION: WEBSERVICE 0x10103960
void Assembly::addGroundChild(Primitive* child)
{
	Clump* clump = child->getClump();

	child->setClumpDepth(1);
	child->getBody()->setParent(NULL);
	clump->setParent(NULL);
}

} // namespace RBX
