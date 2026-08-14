#ifndef V8KERNEL_KERNELINDEX_H
#define V8KERNEL_KERNELINDEX_H

#include "decomp.h"

namespace RBX {

// SIZE 0x04
class KernelIndex
{
public:
	KernelIndex() : kernelIndex(-1) {}
	~KernelIndex() {}

	bool inKernel() const { return kernelIndex >= 0; }

protected:
	mutable int kernelIndex; // 0x00
};

DECOMP_SIZE_ASSERT(KernelIndex, 0x04)

} // namespace RBX

#endif // V8KERNEL_KERNELINDEX_H
