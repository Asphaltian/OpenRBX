#include "v8world/AssemblyStage2.h"

#include "decomp.h"
#include "v8world/Assembly2.h"
#include "v8world/Joint.h"
#include "v8world/Primitive.h"
#include "v8world/SleepStage2.h"

#include <cstddef>

namespace RBX {

// FUNCTION: WEBSERVICE 0x100d7a70 FOLDED
IStage::StageType AssemblyStage::getStageType()
{
	return ASSEMBLY_STAGE;
}

SleepStage* AssemblyStage::getSleepStage()
{
	return static_cast<SleepStage*>(getDownstream());
}

// FUNCTION: WEBSERVICE 0x10126de0
void AssemblyStage::wakeAssembly(Assembly* assembly)
{
	getSleepStage()->onWakeUpRequest(assembly, false);
}

// STUB: WEBSERVICE 0x10126e00
void AssemblyStage::stepUi(int frameCount)
{
	STUB(0x10126e00);
}

// FUNCTION: WEBSERVICE 0x10126ee0
void AssemblyStage::onAssemblyAdded(Assembly* assembly)
{
	assembly->putInPipeline(this);

	assemblies.insert(assembly);

	getSleepStage()->onAssemblyAdded(assembly);
}

// STUB: WEBSERVICE 0x10126f20
void AssemblyStage::onEdgeAdded(Edge* edge)
{
	STUB(0x10126f20);
}

// STUB: WEBSERVICE 0x10127000
AssemblyStage::~AssemblyStage()
{
}

// FUNCTION: WEBSERVICE 0x101270c0
void AssemblyStage::onAssemblyRemoving(Assembly* assembly)
{
	getSleepStage()->onAssemblyRemoving(assembly);

	assemblies.erase(assembly);

	assembly->removeFromStage(this);
}

// STUB: WEBSERVICE 0x101270f0
void AssemblyStage::onEdgeRemoving(Edge* edge)
{
	STUB(0x101270f0);
}

// STUB: WEBSERVICE 0x10127180
AssemblyStage::AssemblyStage(IStage* upstream, World* world) : IWorldStage(upstream, new SleepStage(this, world), world)
{
}

} // namespace RBX
