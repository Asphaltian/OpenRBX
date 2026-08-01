#ifndef V8WORLD_IPIPELINED_H
#define V8WORLD_IPIPELINED_H

#include "decomp.h"
#include "v8kernel/IStage.h"

namespace RBX {

class Kernel;

// SIZE 0x08
class __declspec(novtable) IPipelined
{
public:
	IPipelined() : currentStage(0) {}

	virtual ~IPipelined() {} // vtable+0x00

	IStage* getCurrentStage() const { return currentStage; }

	IStage* getStage(IStage::StageType stageType);

	void putInPipeline(IStage* stage);
	void removeFromPipeline(IStage* stage);
	void putInStage(IStage* stage);
	void removeFromStage(IStage* stage);

	Kernel* getKernel();

	bool downstreamOfStage(IStage* stage);

	virtual void putInKernel(Kernel* kernel); // vtable+0x04
	virtual void removeFromKernel();          // vtable+0x08

private:
	IStage* currentStage; // 0x04
};

DECOMP_SIZE_ASSERT(IPipelined, 0x08)

// FUNCTION: WEBSERVICE 0x10118120
inline bool IPipelined::downstreamOfStage(IStage* stage)
{
	return currentStage->getStageType() > stage->getStageType();
}

} // namespace RBX

#endif // V8WORLD_IPIPELINED_H
