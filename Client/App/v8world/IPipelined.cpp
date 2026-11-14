#include "v8world/IPipelined.h"

#include "v8kernel/Kernel.h"

namespace RBX {

IStage* IPipelined::getStage(IStage::StageType stageType) const
{
	IStage* stage = currentStage;

	while (stage->getStageType() != stageType) {
		stage = stage->getStageType() > stageType ? stage->getUpstream() : stage->getDownstream();
	}

	return stage;
}

void IPipelined::removeFromPipeline(IStage* stage)
{
	currentStage = 0;
}

void IPipelined::removeFromStage(IStage* stage)
{
	currentStage = currentStage->getUpstream();
}

Kernel* IPipelined::getKernel() const
{
	return static_cast<Kernel*>(getStage(IStage::KERNEL_STAGE));
}

void IPipelined::removeFromKernel()
{
	currentStage = currentStage->getUpstream();
}

void IPipelined::putInPipeline(IStage* stage)
{
	currentStage = stage;
}

void IPipelined::putInStage(IStage* stage)
{
	currentStage = stage;
}

void IPipelined::putInKernel(Kernel* kernel)
{
	currentStage = kernel;
}

} // namespace RBX
