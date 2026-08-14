#ifndef V8KERNEL_KERNELDATA_H
#define V8KERNEL_KERNELDATA_H

#include "decomp.h"
#include "util/IndexArray.h"
#include "v8kernel/Body.h"
#include "v8kernel/Connector.h"
#include "v8kernel/Point.h"

namespace RBX {

// SIZE 0x30
class KernelData
{
public:
	IndexArray<Body, &Body::getKernelIndex> bodies;                      // 0x00
	IndexArray<Point, &Point::getKernelIndex> points;                    // 0x0c
	IndexArray<Connector, &Connector::getKernelIndex> connectors;        // 0x18
	IndexArray<Connector, &Connector::getKernelIndex> connectors2ndPass; // 0x24

	// FUNCTION: WEBSERVICE 0x100d5270
	~KernelData() {}
};

DECOMP_SIZE_ASSERT(KernelData, 0x30)

} // namespace RBX

#endif // V8KERNEL_KERNELDATA_H
