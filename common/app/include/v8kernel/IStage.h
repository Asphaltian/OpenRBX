#ifndef V8KERNEL_ISTAGE_H
#define V8KERNEL_ISTAGE_H

namespace RBX {

class IStage;
class Kernel;

class IStage
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

	virtual ~IStage();
	virtual StageType getStageType() const = 0;
	virtual void stepWorld(int worldStepId, int uiStepId, bool throttling) = 0;
	virtual Kernel* getKernel() = 0;

	IStage* getUpstream() { return upstream; }
	IStage* getDownstream() { return downstream; }

private:
	IStage* upstream;   // 0x04
	IStage* downstream; // 0x08
};

} // namespace RBX

#endif // V8KERNEL_ISTAGE_H
