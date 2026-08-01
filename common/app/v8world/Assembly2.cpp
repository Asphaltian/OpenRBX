#include "v8world/Assembly2.h"

#include "decomp.h"
#include "v8kernel/Body.h"
#include "v8kernel/Kernel.h"
#include "v8world/Clump2.h"
#include "v8world/IMoving.h"
#include "v8world/Joint.h"
#include "v8world/Primitive.h"

#include <cstddef>

namespace RBX {

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

	for (Assembly* ancestor = parent; ancestor != NULL; ancestor = ancestor->parent) {
		ancestor->maxRadius.setDirty();
		ancestor->canSleep.setDirty();
	}
}

} // namespace RBX
