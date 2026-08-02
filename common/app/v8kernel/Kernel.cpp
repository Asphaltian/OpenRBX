#include "v8kernel/Kernel.h"

#include "v8kernel/KernelData.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x100d4f20
int Kernel::numConnectors() const
{
	return kernelData->connectors.size();
}

// FUNCTION: WEBSERVICE 0x100d5310
int Kernel::numBodies() const
{
	return kernelData->bodies.size();
}

// FUNCTION: WEBSERVICE 0x100d5320
int Kernel::numPoints() const
{
	return kernelData->points.size();
}

// FUNCTION: WEBSERVICE 0x100d57e0
void Kernel::insertBody(Body* body)
{
	kernelData->bodies.fastAppend(body);
}

// FUNCTION: WEBSERVICE 0x100d5800
void Kernel::removeBody(Body* body)
{
	kernelData->bodies.fastRemove(body);
}

} // namespace RBX
