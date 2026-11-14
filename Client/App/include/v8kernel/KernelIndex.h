#ifndef V8KERNEL_KERNELINDEX_H
#define V8KERNEL_KERNELINDEX_H

#include "placeholder.h"

namespace RBX {

class KernelIndex
{
public:
	KernelIndex() : kernelIndex(-1) {}
	~KernelIndex() {}

	bool inKernel() const { return kernelIndex >= 0; }

protected:
	mutable int kernelIndex;
};

} // namespace RBX

#endif // V8KERNEL_KERNELINDEX_H
