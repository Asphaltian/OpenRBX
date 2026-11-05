#ifndef ROBLOXWND_H
#define ROBLOXWND_H

#include "decomp.h"

// SIZE 0xb8
class CRobloxWnd : public CWnd
{
public:
	CRobloxWnd(bool p_unk0x80);
	virtual ~CRobloxWnd();

private:
	undefined m_unk0x54[0xb8 - 0x54]; // 0x54
};

DECOMP_SIZE_ASSERT(CRobloxWnd, 0xb8)

#endif // ROBLOXWND_H
