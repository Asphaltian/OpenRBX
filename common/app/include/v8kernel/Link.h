#ifndef V8KERNEL_LINK_H
#define V8KERNEL_LINK_H

#include "decomp.h"

namespace RBX {

// SIZE 0xcc
class __declspec(novtable) Link
{
private:
	undefined m_unk0x00[0xcc - 0x00]; // 0x00
};

DECOMP_SIZE_ASSERT(Link, 0xcc)

// SIZE 0xd0
class RevoluteLink : public Link
{
private:
	undefined m_unk0xcc[0xd0 - 0xcc]; // 0xcc
};

DECOMP_SIZE_ASSERT(RevoluteLink, 0xd0)

} // namespace RBX

#endif // V8KERNEL_LINK_H
