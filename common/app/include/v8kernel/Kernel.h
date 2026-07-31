#ifndef V8KERNEL_KERNEL_H
#define V8KERNEL_KERNEL_H

#include "decomp.h"
#include "v8kernel/IStage.h"

namespace RBX {

// SIZE 0x30
class Kernel : public IStage
{
private:
	undefined m_unk0x0c[0x30 - 0x0c]; // 0x0c
};

DECOMP_SIZE_ASSERT(Kernel, 0x30)

} // namespace RBX

#endif // V8KERNEL_KERNEL_H
