#ifndef V8WORLD_ASSEMBLY2_H
#define V8WORLD_ASSEMBLY2_H

#include "decomp.h"
#include "v8world/IPipelined.h"

#include <vector>

namespace RBX {

class Mechanism;
class Primitive;

namespace Sim {

enum AssemblyState
{
	ANCHORED = 0,
	RECURSIVE_WAKE_PENDING = 1,
	WAKE_PENDING = 2,
	AWAKE = 3,
	SLEEPING_CHECKING = 4,
	SLEEPING_DEEPLY = 5,
};

} // namespace Sim

// SIZE 0x24
class SleepInfo
{
public:
	Sim::AssemblyState state; // 0x00

private:
	int sleepCount;                   // 0x04
	undefined m_unk0x08[0x24 - 0x08]; // 0x08
};

DECOMP_SIZE_ASSERT(SleepInfo, 0x24)

// SIZE 0x50
class Assembly : public IPipelined
{
public:
	Assembly* getRootAssembly();
	Primitive* getAssemblyPrimitive();

	Mechanism* getMechanism();
	Sim::AssemblyState getSleepStatus();
	bool getAnchored();
	bool computeCanSleep();

	void notifyMoved();

	virtual void putInKernel(Kernel* kernel); // vtable+0x04
	virtual void removeFromKernel();          // vtable+0x08

private:
	SleepInfo* sleepInfo;             // 0x08
	Primitive* rootPrimitive;         // 0x0c
	Assembly* parent;                 // 0x10
	std::vector<Assembly*> children;  // 0x14
	Mechanism* mechanism;             // 0x24
	undefined m_unk0x28[0x50 - 0x28]; // 0x28
};

DECOMP_SIZE_ASSERT(Assembly, 0x50)

// SIZE 0x50
class Clump : public Assembly
{
};

DECOMP_SIZE_ASSERT(Clump, 0x50)

} // namespace RBX

#endif // V8WORLD_ASSEMBLY2_H
