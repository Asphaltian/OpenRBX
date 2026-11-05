#include "RobloxWnd.h"

// FUNCTION: ROBLOX 0x0045fc50
void CRobloxWnd::FUN_0045fc50(bool active)
{
	if (!active && m_unk0x7c) {
		m_unk0x7c->FUN_0046ca90();
	}
}

// STUB: ROBLOX 0x00460bd0
CRobloxWnd::CRobloxWnd(bool p_unk0x80)
{
	STUB(0x460bd0);
}

// STUB: ROBLOX 0x00464000
CRobloxWnd::~CRobloxWnd()
{
	STUB(0x464000);
}
