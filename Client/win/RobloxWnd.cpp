#include "RobloxWnd.h"

static ATL::CCriticalSection g_unk0x0095448c;

// FUNCTION: ROBLOX 0x0045f4d0
LRESULT CRobloxWnd::FUN_0045f4d0(WPARAM wParam, LPARAM lParam)
{
	SetFocus();
	return 0;
}

// FUNCTION: ROBLOX 0x0045f650
BOOL CRobloxWnd::OnEraseBkgnd(CDC* pDC)
{
	if (m_unk0xa0) {
		return CWnd::OnEraseBkgnd(pDC);
	}
	return TRUE;
}

// FUNCTION: ROBLOX 0x0045fc50
void CRobloxWnd::FUN_0045fc50(bool active)
{
	if (!active && m_unk0x7c) {
		m_unk0x7c->FUN_0046ca90();
	}
}

// FUNCTION: ROBLOX 0x0045fc70
void CRobloxWnd::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
	if (m_unk0x7c) {
		m_unk0x7c->FUN_0046be50(true);
	}
}

// FUNCTION: ROBLOX 0x00460120
void CRobloxWnd::AdjustThrottle(double frameRate)
{
	ATL::CCritSecLock lock(g_unk0x0095448c);
	m_unk0x88 = frameRate;
}

// STUB: ROBLOX 0x00460430
void CRobloxWnd::FUN_00460430()
{
	STUB(0x460430);
}

// FUNCTION: ROBLOX 0x004604c0
BOOL CRobloxWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass =
		AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC, LoadCursor(NULL, IDC_ARROW), NULL, NULL);
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	cs.cx = std::max<int>(cs.cx, 100);
	cs.cy = std::max<int>(cs.cy, 100);
	return CWnd::PreCreateWindow(cs);
}

// STUB: ROBLOX 0x00460530
void CRobloxWnd::OnKillFocus(CWnd* pNewWnd)
{
	FUN_00460430();
	CWnd::OnKillFocus(pNewWnd);
	if (m_unk0x7c) {
		m_unk0x7c->FUN_0046be50(false);
	}
}

// STUB: ROBLOX 0x00460560
void CRobloxWnd::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanging(lpwndpos);
	FUN_00460430();
}

// STUB: ROBLOX 0x00460580
void CRobloxWnd::OnClose()
{
	FUN_00460430();
	CWnd::OnClose();
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

// STUB: ROBLOX 0x00461050
void CRobloxWnd::OnTimer(UINT_PTR nIDEvent)
{
	STUB(0x461050);
}

// STUB: ROBLOX 0x00461690
int CRobloxWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	STUB(0x461690);
	return 0;
}

// STUB: ROBLOX 0x00461950
void CRobloxWnd::OnSize(UINT nType, int cx, int cy)
{
	STUB(0x461950);
}

// STUB: ROBLOX 0x00462a50
void CRobloxWnd::OnPaint()
{
	STUB(0x462a50);
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

// STUB: ROBLOX 0x00464180
void CRobloxWnd::OnDestroy()
{
	STUB(0x464180);
}

// clang-format off
BEGIN_MESSAGE_MAP(CRobloxWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_MESSAGE(WM_USER + 0xbb, FUN_0045f4d0)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_CLOSE()
END_MESSAGE_MAP()
// clang-format on
