#ifndef V8WORLD_IWORLDSTAGE_H
#define V8WORLD_IWORLDSTAGE_H

#include "placeholder.h"
#include "v8kernel/IStage.h"

namespace RBX {

class Edge;
class World;

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

	virtual void onEdgeAdded(Edge* edge);
	virtual void onEdgeRemoving(Edge* edge);

	virtual int getMetric(MetricType metricType) { return getDownstreamWS()->getMetric(metricType); }

	IWorldStage* getUpstreamWS() { return static_cast<IWorldStage*>(getUpstream()); }
	IWorldStage* getDownstreamWS() { return static_cast<IWorldStage*>(getDownstream()); }

	World* getWorld() { return world; }

private:
	World* world;
};

} // namespace RBX

#endif // V8WORLD_IWORLDSTAGE_H
