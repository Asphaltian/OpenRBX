#ifndef V8WORLD_ASSEMBLY2_H
#define V8WORLD_ASSEMBLY2_H

#include "decomp.h"
#include "v8world/IPipelined.h"

namespace RBX {

class Primitive;
class SleepInfo;

// SIZE 0x50
class Assembly : public IPipelined
{
public:
	Assembly* getRootAssembly();
	Primitive* getAssemblyPrimitive();

	void notifyMoved();

private:
	SleepInfo* sleepInfo;             // 0x08
	Primitive* rootPrimitive;         // 0x0c
	Assembly* parent;                 // 0x10
	undefined m_unk0x14[0x50 - 0x14]; // 0x14
};

DECOMP_SIZE_ASSERT(Assembly, 0x50)

// SIZE 0x50
class Clump : public Assembly
{
};

DECOMP_SIZE_ASSERT(Clump, 0x50)

} // namespace RBX

#endif // V8WORLD_ASSEMBLY2_H
