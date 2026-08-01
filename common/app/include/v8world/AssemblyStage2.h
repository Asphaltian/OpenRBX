#ifndef V8WORLD_ASSEMBLYSTAGE2_H
#define V8WORLD_ASSEMBLYSTAGE2_H

#include "decomp.h"
#include "v8world/IWorldStage.h"

#include <set>

namespace RBX {

class Assembly;
class Joint;

// SIZE 0x28
class AssemblyStage : public IWorldStage
{
public:
	virtual StageType getStageType() const;                                 // vtable+0x04
	virtual void stepWorld(int worldStepId, int uiStepId, bool throttling); // vtable+0x08

	virtual void onEdgeAdded(Edge* edge);         // vtable+0x10
	virtual void onEdgeRemoving(Edge* edge);      // vtable+0x14
	virtual int getMetric(MetricType metricType); // vtable+0x18

	void onAssemblyAdded(Assembly* assembly);
	void onAssemblyRemoving(Assembly* assembly);

	void wakeAssembly(Assembly* assembly);

	void stepUi(int frameCount);

private:
	std::set<Assembly*> assemblies; // 0x10
	std::set<Joint*> joints;        // 0x1c
};

DECOMP_SIZE_ASSERT(AssemblyStage, 0x28)

} // namespace RBX

#endif // V8WORLD_ASSEMBLYSTAGE2_H
