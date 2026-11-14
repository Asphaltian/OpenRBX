#ifndef V8KERNEL_ISTAGE_H
#define V8KERNEL_ISTAGE_H

#include <cstddef>

namespace RBX {

class IStage;
class Kernel;

class __declspec(novtable) IStage
{
public:
	enum StageType
	{
		JOINT_STAGE = 0,
		CLUMP_STAGE = 1,
		TREE_STAGE = 2,
		ASSEMBLY_STAGE = 3,
		COLLISION_STAGE = 4,
		SLEEP_STAGE = 5,
		SEPARATE_STAGE = 6,
		SIMJOB_STAGE = 7,
		KERNEL_STAGE = 8,
	};

	IStage(IStage* upstream, IStage* downstream) : upstream(upstream), downstream(downstream) {}

	virtual ~IStage() { delete downstream; }
	virtual StageType getStageType() = 0;

	virtual void stepWorld(int worldStepId, int uiStepId, bool throttling)
	{
		getDownstream()->stepWorld(worldStepId, uiStepId, throttling);
	}

	virtual Kernel* getKernel() { return getDownstream()->getKernel(); }

	IStage* getUpstream() { return upstream; }
	IStage* getDownstream() { return downstream; }

	IStage* findStage(StageType stageType)
	{
		IStage* stage = this;

		while (stage->getStageType() != stageType) {
			stage = stage->getDownstream();
		}

		return stage;
	}

private:
	IStage* upstream;
	IStage* downstream;
};

} // namespace RBX

#endif // V8KERNEL_ISTAGE_H
