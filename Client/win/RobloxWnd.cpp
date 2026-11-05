#include "RobloxWnd.h"

static ATL::CCriticalSection g_unk0x0095448c;

// FUNCTION: ROBLOX 0x0045fc50
void CRobloxWnd::FUN_0045fc50(bool active)
{
	if (!active && m_unk0x7c) {
		m_unk0x7c->FUN_0046ca90();
	}
}

// FUNCTION: ROBLOX 0x00460120
void CRobloxWnd::AdjustThrottle(double frameRate)
{
	ATL::CCritSecLock lock(g_unk0x0095448c);
	m_unk0x88 = frameRate;
}

// STUB: ROBLOX 0x00460bd0
CRobloxWnd::CRobloxWnd(bool p_unk0x80)
{
	STUB(0x460bd0);
}

// FUNCTION: ROBLOX 0x00460d10
void CRobloxWnd::InvalidateRunView()
{
	InvalidateRgn(NULL, FALSE);
}

// STUB: ROBLOX 0x00462be0
std::string CRobloxWnd::getMetric(const std::string& metric) const
{
	STUB(0x462be0);
	return std::string();
}

// STUB: ROBLOX 0x00464000
CRobloxWnd::~CRobloxWnd()
{
	STUB(0x464000);
}
