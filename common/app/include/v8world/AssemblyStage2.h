#ifndef V8WORLD_ASSEMBLYSTAGE2_H
#define V8WORLD_ASSEMBLYSTAGE2_H

#include "decomp.h"
#include "v8world/IWorldStage.h"

#include <set>

namespace RBX {

class Assembly;
class Joint;
class SleepStage;

// VTABLE: WEBSERVICE 0x102485b4
// SIZE 0x28
class AssemblyStage : public IWorldStage
{
public:
	AssemblyStage(IStage* upstream, World* world);

	virtual ~AssemblyStage(); // vtable+0x00

	// SYNTHETIC: WEBSERVICE 0x10127260
	// RBX::AssemblyStage::`scalar deleting destructor'

	virtual StageType getStageType(); // vtable+0x04

	virtual void onEdgeAdded(Edge* edge);    // vtable+0x10
	virtual void onEdgeRemoving(Edge* edge); // vtable+0x14

	void onAssemblyAdded(Assembly* assembly);
	void onAssemblyRemoving(Assembly* assembly);

	void wakeAssembly(Assembly* assembly);

	void stepUi(int frameCount);

private:
	void onJointAdded(Joint* joint);
	void onJointRemoving(Joint* joint);

	SleepStage* getSleepStage();

	std::set<Assembly*> assemblies; // 0x10
	std::set<Joint*> joints;        // 0x1c
};

DECOMP_SIZE_ASSERT(AssemblyStage, 0x28)

} // namespace RBX

#endif // V8WORLD_ASSEMBLYSTAGE2_H
