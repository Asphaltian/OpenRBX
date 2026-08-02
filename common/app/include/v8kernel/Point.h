#ifndef V8KERNEL_POINT_H
#define V8KERNEL_POINT_H

#include "decomp.h"
#include "v8kernel/KernelIndex.h"

#include <G3D/Vector3.h>

namespace RBX {

class Body;

using G3D::Vector3;

// SIZE 0x34
class __declspec(novtable) Point : public KernelIndex
{
public:
	int& getKernelIndex() const { return kernelIndex; }

protected:
	virtual ~Point(); // vtable+0x00

	Body* body;       // 0x0c
	Vector3 localPos; // 0x10
	Vector3 worldPos; // 0x1c
	Vector3 force;    // 0x28

private:
	int numOwners; // 0x08
};

DECOMP_SIZE_ASSERT(Point, 0x34)

} // namespace RBX

#endif // V8KERNEL_POINT_H
