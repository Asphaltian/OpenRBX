#ifndef V8WORLD_ASSEMBLYSTAGE2_H
#define V8WORLD_ASSEMBLYSTAGE2_H

#include "placeholder.h"
#include "v8world/IWorldStage.h"

#include <set>

namespace RBX {

class Assembly;
class Joint;
class SleepStage;

class AssemblyStage : public IWorldStage
{
public:
	AssemblyStage(IStage* upstream, World* world);

	virtual ~AssemblyStage();

	virtual StageType getStageType();

	virtual void onEdgeAdded(Edge* edge);
	virtual void onEdgeRemoving(Edge* edge);

	void onAssemblyAdded(Assembly* assembly);
	void onAssemblyRemoving(Assembly* assembly);

	void wakeAssembly(Assembly* assembly);

	void stepUi(int uiStepId);

private:
	void onJointAdded(Joint* joint);
	void onJointRemoving(Joint* joint);

	std::set<Assembly*> assemblies;
	std::set<Joint*> joints;
};

} // namespace RBX

#endif // V8WORLD_ASSEMBLYSTAGE2_H
