#ifndef V8WORLD_IPIPELINED_H
#define V8WORLD_IPIPELINED_H

#include "v8kernel/IStage.h"

namespace RBX {

class IPipelined
{
public:
	IPipelined() : currentStage(0) {}

	virtual ~IPipelined();

	void removeFromStage(IStage* stage);
	void removeFromKernel();
	void removeFromPipeline(IStage* stage);
	IStage* getStage(IStage::StageType stageType);
	IStage* getKernel();

private:
	IStage* currentStage; // 0x04
};

} // namespace RBX

#endif // V8WORLD_IPIPELINED_H
