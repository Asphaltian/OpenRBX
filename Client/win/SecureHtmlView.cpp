#include "SecureHtmlView.h"

#include "InstanceExplorer.h"

#include <atlconv.h>
#include <mshtmdid.h>
#include <mshtml.h>

IMPLEMENT_DYNAMIC(CSecureHtmlView, CHtmlView)

// FUNCTION: ROBLOX 0x0046a0c0
CSecureHtmlView::CSecureHtmlView() : m_unk0xf0(0)
{
}

// FUNCTION: ROBLOX 0x0046a0e0
CSecureHtmlView::~CSecureHtmlView()
{
}

// FUNCTION: ROBLOX 0x0046a0f0
BOOL CSecureHtmlView::OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar)
{
	USES_CONVERSION;

	if (dispid != DISPID_AMBIENT_DLCONTROL) {
		return CHtmlView::OnAmbientProperty(pSite, dispid, pvar);
	}

	if (m_unk0xf0) {
		V_VT(pvar) = VT_I4;
		V_I4(pvar) = DLCTL_DLIMAGES | DLCTL_VIDEOS | DLCTL_NO_JAVA | DLCTL_NO_RUNACTIVEXCTLS | DLCTL_NO_DLACTIVEXCTLS |
					 DLCTL_NO_FRAMEDOWNLOAD;
		return TRUE;
	}

	return CHtmlView::OnAmbientProperty(pSite, dispid, pvar);
}

// FUNCTION: ROBLOX 0x0046a160
void CSecureHtmlView::OnBeforeNavigate2(
	LPCTSTR lpszURL,
	DWORD nFlags,
	LPCTSTR lpszTargetFrameName,
	CByteArray& baPostedData,
	LPCTSTR lpszHeaders,
	BOOL* pbCancel
)
{
	if (FUN_0041e610(lpszURL)) {
		*pbCancel = TRUE;
		return;
	}
	CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}

// FUNCTION: ROBLOX 0x0046a1b0
BOOL CSecureHtmlView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(NULL, IDC_ARROW), NULL, NULL);
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CHtmlView::PreCreateWindow(cs);
}
