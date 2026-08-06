#ifndef UTIL_IRENDERABLE_H
#define UTIL_IRENDERABLE_H

#include "decomp.h"

namespace RBX {

class IRenderableBucket;

// SIZE 0x10
class __declspec(novtable) IRenderable
{
public:
	virtual ~IRenderable(); // vtable+0x00

private:
	undefined4 index2d;        // 0x04
	undefined4 index3d;        // 0x08
	IRenderableBucket* bucket; // 0x0c
};

DECOMP_SIZE_ASSERT(IRenderable, 0x10)

} // namespace RBX

#endif // UTIL_IRENDERABLE_H
