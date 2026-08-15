#ifndef RBXVIEWBASE_VIEWBASE_H
#define RBXVIEWBASE_VIEWBASE_H

#include "decomp.h"

namespace RBX {

// SIZE 0x4
class ViewBase
{
public:
	virtual ~ViewBase(); // vtable+0x00
};

DECOMP_SIZE_ASSERT(ViewBase, 0x4)

} // namespace RBX

#endif // RBXVIEWBASE_VIEWBASE_H
