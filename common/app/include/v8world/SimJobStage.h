#ifndef V8WORLD_SIMJOBSTAGE_H
#define V8WORLD_SIMJOBSTAGE_H

#include "decomp.h"
#include "v8world/IWorldStage.h"

#include <list>

namespace RBX {

class Assembly;
class Mechanism;

// SIZE 0x1c
class SimJobStage : public IWorldStage
{
public:
	SimJobStage(IStage* upstream, World* world);

	virtual ~SimJobStage(); // vtable+0x00

	// FUNCTION: WEBSERVICE 0x100c66c0 FOLDED
	virtual StageType getStageType() { return SIMJOB_STAGE; } // vtable+0x04

	virtual void onEdgeAdded(Edge* edge);    // vtable+0x10
	virtual void onEdgeRemoving(Edge* edge); // vtable+0x14

	DECOMP_NOINLINE void onAssemblyAdded(Assembly* assembly);
	DECOMP_NOINLINE void onAssemblyRemoving(Assembly* assembly);

	void notifyMovingPrimitives();

	Mechanism* nextMechanism(std::list<Mechanism*>& mechanisms, const Mechanism* mechanism);

private:
	void combineMechanisms(Edge* edge);
	void destroyMechanism(Mechanism* mechanism);
	void insertMechanism(Mechanism* mechanism);
	void splitMechanisms(Assembly* a0, Assembly* a1);

	bool validateEdge(Edge* edge);

	std::list<Mechanism*> mechanisms; // 0x10
};

DECOMP_SIZE_ASSERT(SimJobStage, 0x1c)

} // namespace RBX

#endif // V8WORLD_SIMJOBSTAGE_H
