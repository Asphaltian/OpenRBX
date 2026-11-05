#ifndef DXUSERINPUT_H
#define DXUSERINPUT_H

#include "decomp.h"

// SIZE 0x19c
class DxUserInput
{
public:
	void FUN_0046ca90();

private:
	undefined m_unk0x00[0x19c]; // 0x00
};

DECOMP_SIZE_ASSERT(DxUserInput, 0x19c)

#endif // DXUSERINPUT_H
