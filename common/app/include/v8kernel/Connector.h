#ifndef V8KERNEL_CONNECTOR_H
#define V8KERNEL_CONNECTOR_H

#include "decomp.h"
#include "v8kernel/KernelIndex.h"

namespace RBX {

// SIZE 0x08
class __declspec(novtable) Connector : public KernelIndex
{
public:
	virtual ~Connector();                           // vtable+0x00
	virtual void computeForce(bool throttling) = 0; // vtable+0x04
	virtual bool canThrottle();                     // vtable+0x08
	virtual bool getBroken();                       // vtable+0x0c
	virtual float potentialEnergy();                // vtable+0x10

	int& getKernelIndex() const { return kernelIndex; }
};

DECOMP_SIZE_ASSERT(Connector, 0x08)

} // namespace RBX

#endif // V8KERNEL_CONNECTOR_H
