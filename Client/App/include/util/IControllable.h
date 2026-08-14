#ifndef UTIL_ICONTROLLABLE_H
#define UTIL_ICONTROLLABLE_H

#include "decomp.h"

namespace RBX {

// SIZE 0x4
class __declspec(novtable) IControllable
{
public:
	virtual ~IControllable() {}              // vtable+0x00
	virtual bool isControllable() const = 0; // vtable+0x04
};

DECOMP_SIZE_ASSERT(IControllable, 0x4)

} // namespace RBX

#endif // UTIL_ICONTROLLABLE_H
