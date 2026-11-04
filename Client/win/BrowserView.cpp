#include "BrowserView.h"

#include "MainFrm.h"
#include "Roblox.h"

#include <algorithm>

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

// FUNCTION: ROBLOX 0x0040a290
void CBrowserView::OnSize(UINT nType, int cx, int cy)
{
	CSize size(0, 0);

	if (m_unk0x108.GetSafeHwnd()) {
		size = m_unk0x108.CalcDockingLayout(cx, LM_HIDEWRAP | LM_COMMIT | LM_HORZDOCK | LM_HORZ | LM_STRETCH);
		m_unk0x108.MoveWindow(0, 0, cx, size.cy);
		m_unk0x108.Invalidate(FALSE);
	}

	if (m_wndBrowser.GetSafeHwnd()) {
		m_wndBrowser.SetWindowPos(NULL, 0, size.cy, cx, cy - size.cy, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW);
	}
}

// FUNCTION: ROBLOX 0x0040a320
void CBrowserView::FUN_0040a320(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_unk0x2b4);
}

// FUNCTION: ROBLOX 0x0040a340
void CBrowserView::FUN_0040a340(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_unk0x2b5);
}

// FUNCTION: ROBLOX 0x0040a360
void CBrowserView::FUN_0040a360(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_unk0x104 != NULL);
}

// FUNCTION: ROBLOX 0x0040a380
void CBrowserView::FUN_0040a380(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_unk0x2b8.IsEmpty());
}

// FUNCTION: ROBLOX 0x0040a8a0
int CBrowserView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSecureHtmlView::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	m_unk0x108.CreateToolBar(WS_TABSTOP | WS_VISIBLE | WS_CHILD | CBRS_TOOLTIPS, this);
	m_unk0x108.SetOwner(this);

	if (XTPPaintManager()) {
		XTPPaintManager()->InternalAddRef();
		m_unk0x108.SetPaintManager(XTPPaintManager());
	}

	XTPImageManager()->InternalAddRef();
	m_unk0x108.SetImageManager(XTPImageManager());

	m_unk0x108.LoadToolBar(0x93, FALSE);
	return 0;
}

// STUB: ROBLOX 0x0040a960
int CBrowserView::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	STUB(0x40a960);
	return FALSE;
}

// FUNCTION: ROBLOX 0x0040aa90
void CBrowserView::FUN_0040aa90()
{
	if (m_unk0x2b8.Compare("")) {
		Navigate(m_unk0x2b8);
	}
}

// FUNCTION: ROBLOX 0x0040aad0
void CBrowserView::FUN_0040aad0()
{
	Refresh();
}

// FUNCTION: ROBLOX 0x0040aae0
void CBrowserView::FUN_0040aae0()
{
	GoForward();
}

// FUNCTION: ROBLOX 0x0040aaf0
void CBrowserView::FUN_0040aaf0()
{
	CString url = m_unk0x104->GetEditText();

	if (m_unk0x104->FindString(-1, url) == -1) {
		m_unk0x104->InsertString(0, url);

		if (m_unk0x104->GetCount() > 10) {
			m_unk0x104->DeleteItem(m_unk0x104->GetCount() - 1);
		}
	}

	Navigate(url);
	m_unk0x104->GetEditCtrl()->SetSel(0, -1);
}

// FUNCTION: ROBLOX 0x0040ac30
void CBrowserView::FUN_0040ac30()
{
	GoBack();
}

// FUNCTION: ROBLOX 0x0040ac40
void CBrowserView::FUN_0040ac40()
{
	Stop();
}

// FUNCTION: ROBLOX 0x0040ac50
void CBrowserView::FUN_0040ac50(CCmdUI* pCmdUI)
{
	VARIANT_BOOL busy;

	pCmdUI->Enable(m_pBrowserApp->get_Busy(&busy) == S_OK && busy == VARIANT_TRUE ? TRUE : FALSE);
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

// FUNCTION: ROBLOX 0x0040b0f0
void CBrowserView::FUN_0040b0f0(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* notify = reinterpret_cast<NMXTPCONTROL*>(pNMHDR);

	if (notify->pControl->GetType() == xtpControlComboBox) {
		FUN_0040aaf0();
		*pResult = 1;
	}
}

// clang-format off
BEGIN_MESSAGE_MAP(CBrowserView, CSecureHtmlView)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND(0x80bb, FUN_0040ac40)
	ON_UPDATE_COMMAND_UI(0x80bb, FUN_0040ac50)
	ON_COMMAND(0x80bd, FUN_0040aa90)
	ON_UPDATE_COMMAND_UI(0x80bd, FUN_0040a380)
	ON_COMMAND(0x80bc, FUN_0040aad0)
	ON_UPDATE_COMMAND_UI(0x80bc, FUN_00453eb0)
	ON_COMMAND(0x80ba, FUN_0040ac30)
	ON_UPDATE_COMMAND_UI(0x80ba, FUN_0040a320)
	ON_COMMAND(0x80b9, FUN_0040aae0)
	ON_UPDATE_COMMAND_UI(0x80b9, FUN_0040a340)
	ON_COMMAND(0x80ca, FUN_0040aaf0)
	ON_UPDATE_COMMAND_UI(0x80ca, FUN_0040a360)
	ON_XTP_EXECUTE(0x80c9, FUN_0040b0f0)
	ON_XTP_CREATECONTROL()
END_MESSAGE_MAP()
// clang-format on

// FUNCTION: ROBLOX 0x0040ce30
CBrowserView::CBrowserView() : m_unk0x104(NULL), m_unk0x2b4(0), m_unk0x2b5(0)
{
	FUN_004334d0(XTPImageManager());
}

// FUNCTION: ROBLOX 0x0040cee0
CBrowserView::~CBrowserView()
{
}

// FUNCTION: ROBLOX 0x0040cf90
void CBrowserView::OnNavigateComplete2(LPCTSTR strURL)
{
	CSecureHtmlView::OnNavigateComplete2(strURL);

	if (m_unk0x104) {
		if (std::find(m_unk0xf8.begin(), m_unk0xf8.end(), std::string(strURL)) == m_unk0xf8.end()) {
			m_unk0x104->SetEditText(strURL);
		}
	}
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

// FUNCTION: ROBLOX 0x00453eb0 FOLDED
void CBrowserView::FUN_00453eb0(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

// FUNCTION: ROBLOX 0x00787c20 FOLDED
BOOL CBrowserView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
