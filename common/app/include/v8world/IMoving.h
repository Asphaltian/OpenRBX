#ifndef V8WORLD_IMOVING_H
#define V8WORLD_IMOVING_H

#include "decomp.h"

namespace RBX {

// SIZE 0x0c
class IMoving
{
public:
	void notifyMoved();

private:
	undefined m_unk0x00[0x0c - 0x00]; // 0x00
};

DECOMP_SIZE_ASSERT(IMoving, 0x0c)

} // namespace RBX

#endif // V8WORLD_IMOVING_H
