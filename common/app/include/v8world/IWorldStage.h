#ifndef V8WORLD_IWORLDSTAGE_H
#define V8WORLD_IWORLDSTAGE_H

#include "decomp.h"
#include "v8kernel/IStage.h"

namespace RBX {

class Edge;
class World;

// SIZE 0x10
class __declspec(novtable) IWorldStage : public IStage
{
public:
	enum MetricType
	{
		NUM_CONTACTSTAGE_CONTACTS = 0,
		NUM_STEPPING_CONTACTS = 1,
		NUM_TOUCHING_CONTACTS = 2,
		MAX_TREE_DEPTH = 3,
	};

	IWorldStage(IStage* upstream, IStage* downstream, World* world) : IStage(upstream, downstream), world(world) {}

	virtual void onEdgeAdded(Edge* edge);    // vtable+0x10
	virtual void onEdgeRemoving(Edge* edge); // vtable+0x14

	virtual int getMetric(MetricType metricType) { return getDownstreamWS()->getMetric(metricType); } // vtable+0x18

	IWorldStage* getUpstreamWS() { return static_cast<IWorldStage*>(getUpstream()); }
	IWorldStage* getDownstreamWS() { return static_cast<IWorldStage*>(getDownstream()); }

	World* getWorld() { return world; }

private:
	World* world; // 0x0c
};

DECOMP_SIZE_ASSERT(IWorldStage, 0x10)

} // namespace RBX

#endif // V8WORLD_IWORLDSTAGE_H
