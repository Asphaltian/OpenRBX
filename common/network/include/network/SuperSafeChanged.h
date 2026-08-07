#ifndef NETWORK_SUPERSAFECHANGED_H
#define NETWORK_SUPERSAFECHANGED_H

#include "decomp.h"

namespace RBX {

// SIZE 0x1
class SuperSafeChanged
{
public:
	bool superSafeOn; // 0x00

	SuperSafeChanged(bool superSafeOn) : superSafeOn(superSafeOn) {}
};

DECOMP_SIZE_ASSERT(SuperSafeChanged, 0x1)

} // namespace RBX

#endif // NETWORK_SUPERSAFECHANGED_H
