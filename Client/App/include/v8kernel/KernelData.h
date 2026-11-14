#ifndef V8KERNEL_KERNELDATA_H
#define V8KERNEL_KERNELDATA_H

#include "placeholder.h"
#include "util/IndexArray.h"
#include "v8kernel/Body.h"
#include "v8kernel/Connector.h"
#include "v8kernel/Point.h"

namespace RBX {

class KernelData
{
public:
	KernelData() {}

	IndexArray<Body, &Body::getKernelIndex> bodies;
	IndexArray<Point, &Point::getKernelIndex> points;
	IndexArray<Connector, &Connector::getKernelIndex> connectors;
	IndexArray<Connector, &Connector::getKernelIndex> connectors2ndPass;

	~KernelData() {}
};

} // namespace RBX

#endif // V8KERNEL_KERNELDATA_H
