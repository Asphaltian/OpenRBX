#ifndef V8KERNEL_KERNEL_H
#define V8KERNEL_KERNEL_H

#include "decomp.h"
#include "util/IndexArray.h"
#include "v8kernel/Connector.h"
#include "v8kernel/IStage.h"

namespace RBX {

class Body;
class KernelData;

// SIZE 0x30
class Kernel : public IStage
{
public:
	int numBodies() const;
	int numPoints() const;
	int numConnectors() const;

	void insertBody(Body* body);
	void removeBody(Body* body);

private:
	static int numKernels;

	bool inStepCode;                                                      // 0x0c
	KernelData* kernelData;                                               // 0x10
	IndexArray<Connector, &Connector::getKernelIndex> realTimeConnectors; // 0x14
	int maxBodies;                                                        // 0x20
	int maxPoints;                                                        // 0x24
	int maxConnectors;                                                    // 0x28
	undefined m_unk0x2c[0x30 - 0x2c];                                     // 0x2c
};

DECOMP_SIZE_ASSERT(Kernel, 0x30)

} // namespace RBX

#endif // V8KERNEL_KERNEL_H
