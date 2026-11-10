#include "WebToolbox.h"

#include "Roblox.h"
#include "RobloxWnd.h"

IMPLEMENT_DYNCREATE(CWebToolbox, CSecureHtmlView)

// FUNCTION: ROBLOX 0x0046ea00
void CWebToolbox::FUN_0046ea00(int category)
{
	CString url;
	url.Format("%s/IDE/ClientToolbox.aspx?Category=%d", static_cast<LPCTSTR>(FUN_00407190()), category);
	Navigate(url);
}

// FUNCTION: ROBLOX 0x0046eb30
void CWebToolbox::OnSetFocus(CWnd* pOldWnd)
{
	CSecureHtmlView::OnSetFocus(pOldWnd);

	if (m_unk0xf4) {
		m_unk0xf4->SetFocus();
	}
}

// FUNCTION: ROBLOX 0x0046eb50
int CWebToolbox::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	if (m_unk0xf4) {
		m_unk0xf4->SetFocus();
	}

	return MA_NOACTIVATE;
}

// FUNCTION: ROBLOX 0x0046ec10
void CWebToolbox::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CSecureHtmlView::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMaxSize.y = 50;
}

// FUNCTION: ROBLOX 0x0046ec30 FOLDED
void CWebToolbox::OnLButtonDown(UINT nFlags, CPoint point)
{
	CSecureHtmlView::OnLButtonDown(nFlags, point);

	if (m_unk0xf4) {
		m_unk0xf4->SetFocus();
	}
}

// FUNCTION: ROBLOX 0x0046ec30 FOLDED
void CWebToolbox::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CSecureHtmlView::OnActivate(nState, pWndOther, bMinimized);

	if (m_unk0xf4) {
		m_unk0xf4->SetFocus();
	}
}

// clang-format off
BEGIN_MESSAGE_MAP(CWebToolbox, CSecureHtmlView)
	ON_WM_GETMINMAXINFO()
	ON_WM_SETFOCUS()
	ON_WM_MOUSEACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()
// clang-format on
