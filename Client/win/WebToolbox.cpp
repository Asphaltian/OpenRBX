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

// FUNCTION: ROBLOX 0x0046eac0
HRESULT CWebToolbox::OnDocWindowActivate(BOOL fActivate)
{
	if (m_unk0xf4) {
		m_unk0xf4->SetFocus();
	}

	return CSecureHtmlView::OnDocWindowActivate(FALSE);
}

// FUNCTION: ROBLOX 0x0046eae0
HRESULT CWebToolbox::OnFrameWindowActivate(BOOL fActivate)
{
	if (m_unk0xf4) {
		m_unk0xf4->SetFocus();
	}

	return CSecureHtmlView::OnFrameWindowActivate(FALSE);
}

// FUNCTION: ROBLOX 0x0042d6c0 FOLDED
HRESULT CWebToolbox::OnGetHostInfo(DOCHOSTUIINFO* pInfo)
{
	HRESULT hr = CSecureHtmlView::OnGetHostInfo(pInfo);
	if (hr == S_OK) {
		pInfo->dwFlags |= DOCHOSTUIFLAG_DIALOG | DOCHOSTUIFLAG_DISABLE_HELP_MENU | DOCHOSTUIFLAG_NO3DBORDER |
						  DOCHOSTUIFLAG_DISABLE_SCRIPT_INACTIVE | DOCHOSTUIFLAG_ENABLE_FORMS_AUTOCOMPLETE |
						  DOCHOSTUIFLAG_THEME | DOCHOSTUIFLAG_LOCAL_MACHINE_ACCESS_CHECK |
						  DOCHOSTUIFLAG_DISABLE_UNTRUSTEDPROTOCOL;
	}

	return hr;
}

// FUNCTION: ROBLOX 0x0046eb00
void CWebToolbox::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CSecureHtmlView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (m_unk0xf4) {
		m_unk0xf4->SetFocus();
	}
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

// FUNCTION: ROBLOX 0x0046eb70
void CWebToolbox::OnInitialUpdate()
{
	CSecureHtmlView::OnInitialUpdate();

	CString url = FUN_00407190() + "/IDE/ClientToolbox.aspx";
	Navigate(url);
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

// FUNCTION: ROBLOX 0x0046ed60
HRESULT CWebToolbox::OnGetExternal(LPDISPATCH* lppDispatch)
{
	if (m_unk0xf4) {
		m_unk0xf4->SetFocus();
	}

	CComPtr<IDispatch> dispatch(m_unk0xf8);
	return dispatch.CopyTo(lppDispatch);
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
