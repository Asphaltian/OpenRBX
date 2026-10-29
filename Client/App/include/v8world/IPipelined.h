#ifndef V8WORLD_IPIPELINED_H
#define V8WORLD_IPIPELINED_H

#include "decomp.h"
#include "v8kernel/IStage.h"
#include "v8world/IWorldStage.h"

#include <cstddef>

namespace RBX {

class Kernel;
class World;

// SIZE 0x08
class __declspec(novtable) IPipelined
{
private:
	IStage* currentStage; // 0x04

public:
	void removeFromStage(IStage* stage);

private:
	void removeFromStage(IStage::StageType stageType);
	IStage* getStage(IStage::StageType stageType) const;

public:
	IPipelined() : currentStage(0) {}

	virtual ~IPipelined() {} // vtable+0x00

	void putInPipeline(IStage* stage);
	void removeFromPipeline(IStage* stage);
	void putInStage(IStage* stage);

	bool inPipeline() const { return currentStage != NULL; }
	bool inStage(IStage* stage) const { return currentStage == stage; }
	bool inStage(IStage::StageType stageType) const;
	bool inOrDownstreamOfStage(IStage* stage) const;
	bool downstreamOfStage(IStage* stage) const;
	bool inKernel() const { return currentStage->getStageType() == IStage::KERNEL_STAGE; }

	Kernel* getKernel() const;

	virtual void putInKernel(Kernel* kernel); // vtable+0x04
	virtual void removeFromKernel();          // vtable+0x08

	World* getWorld();
};

DECOMP_SIZE_ASSERT(IPipelined, 0x08)

inline World* IPipelined::getWorld()
{
	if (currentStage == NULL) {
		return NULL;
	}

	IStage* stage;

	if (currentStage->getStageType() != IStage::KERNEL_STAGE) {
		stage = currentStage;
	}
	else {
		stage = currentStage->getUpstream();
	}

	return static_cast<IWorldStage*>(stage)->getWorld();
}

// FUNCTION: WEBSERVICE 0x10118120
inline bool IPipelined::downstreamOfStage(IStage* iStage) const
{
	return currentStage->getStageType() > iStage->getStageType();
}

inline bool IPipelined::inOrDownstreamOfStage(IStage* stage) const
{
	return currentStage->getStageType() >= stage->getStageType();
}

} // namespace RBX

#endif // V8WORLD_IPIPELINED_H
