#ifndef V8WORLD_SIMJOBSTAGE_H
#define V8WORLD_SIMJOBSTAGE_H

#include "placeholder.h"
#include "v8world/IWorldStage.h"
#include "v8world/Mechanism.h"

#include <list>

namespace RBX {

class Assembly;

class SimJobStage : public IWorldStage
{
public:
	SimJobStage(IStage* upstream, World* world);

	virtual ~SimJobStage() {}

	virtual StageType getStageType() { return SIMJOB_STAGE; }

	virtual void onEdgeAdded(Edge* edge);
	virtual void onEdgeRemoving(Edge* edge);

	void onAssemblyAdded(Assembly* assembly);
	void onAssemblyRemoving(Assembly* assembly);

	void notifyMovingPrimitives();

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

	std::list<Mechanism*> mechanisms;
};

} // namespace RBX

#endif // V8WORLD_SIMJOBSTAGE_H
