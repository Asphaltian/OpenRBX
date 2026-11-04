#include "BrowserView.h"

#include "MainFrm.h"
#include "Roblox.h"

IMPLEMENT_DYNCREATE(CIDEBrowserView, CBrowserView)
IMPLEMENT_DYNCREATE(CBrowserView, CSecureHtmlView)
IMPLEMENT_DYNCREATE(CPlayBrowserView, CBrowserView)
IMPLEMENT_DYNCREATE(CBrowserDoc, CNullDoc)

// FUNCTION: ROBLOX 0x0040a1b0
HRESULT CBrowserView::OnGetHostInfo(DOCHOSTUIINFO* pInfo)
{
	HRESULT result = CSecureHtmlView::OnGetHostInfo(pInfo);

	if (result == S_OK) {
		pInfo->dwFlags |= DOCHOSTUIFLAG_DISABLE_HELP_MENU | DOCHOSTUIFLAG_NO3DBORDER |
						  DOCHOSTUIFLAG_DISABLE_SCRIPT_INACTIVE | DOCHOSTUIFLAG_ENABLE_FORMS_AUTOCOMPLETE |
						  DOCHOSTUIFLAG_THEME | DOCHOSTUIFLAG_LOCAL_MACHINE_ACCESS_CHECK |
						  DOCHOSTUIFLAG_DISABLE_UNTRUSTEDPROTOCOL;
	}

	return result;
}

// FUNCTION: ROBLOX 0x0040a1d0
void CBrowserView::OnTitleChange(LPCTSTR lpszText)
{
	CSecureHtmlView::OnTitleChange(lpszText);

	CDocument* document = m_pDocument;
	if (document) {
		document->SetTitle(lpszText);
	}
	else if (GetParentFrame()) {
		GetParentFrame()->SetWindowText(lpszText);
	}
}

// FUNCTION: ROBLOX 0x0040a220
void CBrowserView::OnDocumentComplete(LPCTSTR lpszURL)
{
	CSecureHtmlView::OnDocumentComplete(lpszURL);
}

// FUNCTION: ROBLOX 0x0040a230
void CBrowserView::OnCommandStateChange(long nCommand, BOOL bEnable)
{
	CSecureHtmlView::OnCommandStateChange(nCommand, bEnable);

	switch (nCommand) {
	case CSC_NAVIGATEBACK:
		m_unk0x2b4 = bEnable != FALSE;
		break;
	case CSC_NAVIGATEFORWARD:
		m_unk0x2b5 = bEnable != FALSE;
		break;
	case CSC_UPDATECOMMANDS:
		m_unk0x108.Invalidate();
		break;
	}
}

// FUNCTION: ROBLOX 0x0040b0a0
void CBrowserView::OnInitialUpdate()
{
	CSecureHtmlView::OnInitialUpdate();

	if (m_pDocument) {
		static_cast<CBrowserDoc*>(m_pDocument)->m_unk0x54 = this;
	}

	if (m_unk0x2b8.Compare("")) {
		Navigate(m_unk0x2b8);
	}
}

// FUNCTION: ROBLOX 0x0040ce30
CBrowserView::CBrowserView() : m_unk0x104(0), m_unk0x2b4(0), m_unk0x2b5(0)
{
	FUN_004334d0(XTPImageManager());
}

// FUNCTION: ROBLOX 0x0040cee0
CBrowserView::~CBrowserView()
{
}

// FUNCTION: ROBLOX 0x0040d090
CIDEBrowserView::CIDEBrowserView()
{
	m_unk0x2b8 = FUN_00407190() + "/IDE/Start.aspx";
}

// FUNCTION: ROBLOX 0x0040d130
CPlayBrowserView::CPlayBrowserView()
{
	m_unk0x2b8 = FUN_00407190() + "/Start.aspx";
}
