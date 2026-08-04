#include "v8world/SimJobStage.h"

#include "decomp.h"

#include <cstddef>

namespace RBX {

// STUB: WEBSERVICE 0x1011c900
void SimJobStage::onEdgeRemoving(Edge* edge)
{
	STUB(0x1011c900);
}

// STUB: WEBSERVICE 0x1011c920
Mechanism* SimJobStage::nextMechanism(std::list<Mechanism*>& mechanisms, const Mechanism* mechanism)
{
	STUB(0x1011c920);
	return NULL;
}

// STUB: WEBSERVICE 0x1011c990
SimJobStage::SimJobStage(IStage* upstream, World* world) : IWorldStage(upstream, NULL, world)
{
	STUB(0x1011c990);
}

// STUB: WEBSERVICE 0x1011ca60
void SimJobStage::notifyMovingPrimitives()
{
	STUB(0x1011ca60);
}

// STUB: WEBSERVICE 0x1011cb40
void SimJobStage::destroyMechanism(Mechanism* mechanism)
{
	STUB(0x1011cb40);
}

// STUB: WEBSERVICE 0x1011cba0
void SimJobStage::onAssemblyAdded(Assembly* assembly)
{
	STUB(0x1011cba0);
}

// STUB: WEBSERVICE 0x1011cc80
void SimJobStage::onAssemblyRemoving(Assembly* assembly)
{
	STUB(0x1011cc80);
}

// STUB: WEBSERVICE 0x1011ccc0
void SimJobStage::combineMechanisms(Edge* edge)
{
	STUB(0x1011ccc0);
}

// STUB: WEBSERVICE 0x1011cd20
void SimJobStage::onEdgeAdded(Edge* edge)
{
	STUB(0x1011cd20);
}

SimJobStage::~SimJobStage()
{
}

} // namespace RBX
