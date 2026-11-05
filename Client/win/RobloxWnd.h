#ifndef ROBLOXWND_H
#define ROBLOXWND_H

#include "DxUserInput.h"
#include "decomp.h"

// SIZE 0xb8
class CRobloxWnd : public CWnd
{
public:
	CRobloxWnd(bool p_unk0x80);
	virtual ~CRobloxWnd();

	void FUN_0045fc50(bool active);

private:
	undefined m_unk0x54[0x7c - 0x54]; // 0x54
	DxUserInput* m_unk0x7c;           // 0x7c
	undefined m_unk0x80[0xb8 - 0x80]; // 0x80
};

DECOMP_SIZE_ASSERT(CRobloxWnd, 0xb8)

#endif // ROBLOXWND_H
