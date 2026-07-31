#include "v8world/IPipelined.h"

#include "v8kernel/Kernel.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x1010b9b0
IStage* IPipelined::getStage(IStage::StageType stageType)
{
	IStage* stage = currentStage;

	while (stage->getStageType() != stageType) {
		if (stageType < stage->getStageType()) {
			stage = stage->getUpstream();
		}
		else {
			stage = stage->getDownstream();
		}
	}

	return stage;
}

// FUNCTION: WEBSERVICE 0x101ae590 FOLDED
void IPipelined::putInPipeline(IStage* stage)
{
	currentStage = stage;
}

// FUNCTION: WEBSERVICE 0x1010b9f0
void IPipelined::removeFromPipeline(IStage* stage)
{
	currentStage = 0;
}

// FUNCTION: WEBSERVICE 0x101ae590 FOLDED
void IPipelined::putInStage(IStage* stage)
{
	currentStage = stage;
}

// FUNCTION: WEBSERVICE 0x1010ba00
void IPipelined::removeFromStage(IStage* stage)
{
	currentStage = currentStage->getUpstream();
}

// FUNCTION: WEBSERVICE 0x1010ba10
IStage* IPipelined::getKernel()
{
	return getStage(IStage::KERNEL_STAGE);
}

// FUNCTION: WEBSERVICE 0x101ae590 FOLDED
void IPipelined::putInKernel(Kernel* kernel)
{
	currentStage = kernel;
}

// FUNCTION: WEBSERVICE 0x1010ba20
void IPipelined::removeFromKernel()
{
	currentStage = currentStage->getUpstream();
}

} // namespace RBX
