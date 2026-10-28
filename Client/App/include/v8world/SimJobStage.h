#ifndef V8WORLD_SIMJOBSTAGE_H
#define V8WORLD_SIMJOBSTAGE_H

#include "decomp.h"
#include "v8world/IWorldStage.h"
#include "v8world/Mechanism.h"

#include <list>

namespace RBX {

class Assembly;

// VTABLE: WEBSERVICE 0x10247f04
// SIZE 0x1c
class SimJobStage : public IWorldStage
{
public:
	SimJobStage(IStage* upstream, World* world);

	virtual ~SimJobStage() {} // vtable+0x00

	// SYNTHETIC: WEBSERVICE 0x1011ca20
	// RBX::SimJobStage::`scalar deleting destructor'

	// FUNCTION: WEBSERVICE 0x100c66c0 FOLDED
	virtual StageType getStageType() { return SIMJOB_STAGE; } // vtable+0x04

	virtual void onEdgeAdded(Edge* edge);    // vtable+0x10
	virtual void onEdgeRemoving(Edge* edge); // vtable+0x14

	void onAssemblyAdded(Assembly* assembly);
	void onAssemblyRemoving(Assembly* assembly);

	void notifyMovingPrimitives();

	// FUNCTION: WEBSERVICE 0x1011c920
	Mechanism* nextMechanism(std::list<Mechanism*>& list, const Mechanism* current)
	{
		std::list<Mechanism*>::iterator it = current->myIt;
		it++;

		return it == list.end() ? *list.begin() : *it;
	}

	template <class Callback>
	void reportMechanisms(Callback& callback, MechanismTracker& tracker, const Mechanism* ignore)
	{
		if (mechanisms.size() == 0) {
			return;
		}

		if (!tracker.tracking()) {
			tracker.setMechanism(*mechanisms.begin());
		}

		unsigned int reported = 0;
		Mechanism* current = tracker.getMechanism();
		while (reported < mechanisms.size()) {
			reported++;
			Mechanism* mechanism = current;
			current = nextMechanism(mechanisms, current);

			if (mechanism != ignore) {
				if (!callback.report(*mechanism)) {
					break;
				}
			}
		}

		tracker.setMechanism(current);
	}

private:
	void combineMechanisms(Edge* edge);
	void destroyMechanism(Mechanism* mechanism);
	void insertMechanism(Mechanism* mechanism);
	void splitMechanisms(Assembly* a0, Assembly* a1);

	bool validateEdge(Edge* edge);

	std::list<Mechanism*> mechanisms; // 0x10
};

DECOMP_SIZE_ASSERT(SimJobStage, 0x1c)

// clang-format off
// TEMPLATE: WEBSERVICE 0x1019f800
// RBX::SimJobStage::reportMechanisms<RBX::Network::Replicator::JobSender>
// clang-format on

} // namespace RBX

#endif // V8WORLD_SIMJOBSTAGE_H
