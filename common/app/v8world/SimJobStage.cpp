#include "v8world/SimJobStage.h"

#include "decomp.h"
#include "v8kernel/Kernel.h"
#include "v8world/Assembly2.h"
#include "v8world/Primitive.h"

#include <algorithm>
#include <cstddef>
#include <functional>

namespace RBX {

// FUNCTION: WEBSERVICE 0x1011c900
void SimJobStage::onEdgeRemoving(Edge* edge)
{
	edge->removeFromKernel();

	edge->removeFromStage(this);
}

// FUNCTION: WEBSERVICE 0x1011c990
SimJobStage::SimJobStage(IStage* upstream, World* world) : IWorldStage(upstream, new Kernel(this), world)
{
}

// FUNCTION: WEBSERVICE 0x1011ca60
void SimJobStage::notifyMovingPrimitives()
{
	std::for_each(mechanisms.begin(), mechanisms.end(), std::mem_fun(&Mechanism::notifyMovingPrimitives));
}

// FUNCTION: WEBSERVICE 0x1011cb40
void SimJobStage::destroyMechanism(Mechanism* mechanism)
{
	Mechanism* next = mechanisms.size() > 1 ? nextMechanism(mechanisms, mechanism) : NULL;

	MechanismTracker::transferTrackers(mechanism, next);

	mechanisms.erase(mechanism->myIt);

	delete mechanism;
}

// STUB: WEBSERVICE 0x1011cba0
void SimJobStage::onAssemblyAdded(Assembly* a)
{
	a->putInPipeline(this);

	Mechanism* mechanism = new Mechanism();
	mechanism->insertAssembly(a);

	mechanisms.push_back(mechanism);
	mechanism->myIt = --mechanisms.end();

	a->putInKernel(getKernel());
}

// FUNCTION: WEBSERVICE 0x1011cc80
void SimJobStage::onAssemblyRemoving(Assembly* a)
{
	a->removeFromKernel();

	Mechanism* mechanism = a->getMechanism();

	mechanism->removeAssembly(a);

	if (mechanism->getAssemblies().size() == 0) {
		destroyMechanism(mechanism);
	}

	a->removeFromStage(this);
}

// STUB: WEBSERVICE 0x1011ccc0
void SimJobStage::combineMechanisms(Edge* edge)
{
	Assembly* a0 = edge->getPrimitive(0)->getAssembly();
	Assembly* a1 = edge->getPrimitive(1)->getAssembly();

	Mechanism* m0 = a0->getMechanism();
	Mechanism* m1 = a1->getMechanism();

	if (m0 != m1) {
		Mechanism* bigger;
		Mechanism* smaller;

		if (m0->getAssemblies().size() > m1->getAssemblies().size()) {
			bigger = m0;
			smaller = m1;
		}
		else {
			bigger = m1;
			smaller = m0;
		}

		bigger->absorb(smaller);

		destroyMechanism(smaller);
	}
}

// FUNCTION: WEBSERVICE 0x1011cd20
void SimJobStage::onEdgeAdded(Edge* edge)
{
	edge->putInPipeline(this);

	if (edge->getEdgeType() == Edge::JOINT) {
		Assembly* a0 = edge->getPrimitive(0)->getAssembly();
		Assembly* a1 = edge->getPrimitive(1)->getAssembly();

		if (a0->inOrDownstreamOfStage(this) && a1->inOrDownstreamOfStage(this)) {
			combineMechanisms(edge);
		}
	}

	edge->putInKernel(getKernel());
}

} // namespace RBX
