#include "BrowserView.h"

#include "BrowserViewExternal.h"
#include "LogManager.h"
#include "MainFrm.h"
#include "Roblox.h"
#include "util/Http.h"

#include <algorithm>

IMPLEMENT_DYNCREATE(CIDEBrowserView, CBrowserView)
IMPLEMENT_DYNCREATE(CBrowserView, CSecureHtmlView)
IMPLEMENT_DYNCREATE(CPlayBrowserView, CBrowserView)
IMPLEMENT_DYNCREATE(CBrowserDoc, CNullDoc)

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

void CBrowserView::OnDocumentComplete(LPCTSTR lpszURL)
{
	CSecureHtmlView::OnDocumentComplete(lpszURL);
}

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

void CBrowserView::FUN_0040a320(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_unk0x2b4);
}

void CBrowserView::FUN_0040a340(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_unk0x2b5);
}

void CBrowserView::FUN_0040a360(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_unk0x104 != NULL);
}

void CBrowserView::FUN_0040a380(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_unk0x2b8.IsEmpty());
}

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

int CBrowserView::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->bToolBar) {
		CXTPToolBar* toolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreateControl->pCommandBar);

		if (toolBar && lpCreateControl->nID == 0x80c9) {
			if (!m_unk0x104) {
				m_unk0x104 = new CXTPControlComboBox();
			}

			m_unk0x104->SetDropDownListStyle();
			m_unk0x104->SetCaption("Address");
			m_unk0x104->SetStyle(xtpButtonCaption);
			m_unk0x104->SetFlags(xtpFlagManualUpdate | xtpFlagControlStretched);
			m_unk0x104->SetEnabled(TRUE);

			lpCreateControl->pControl = m_unk0x104;
			return TRUE;
		}
	}

	return FALSE;
}

void CBrowserView::FUN_0040aa90()
{
	if (m_unk0x2b8.Compare("")) {
		Navigate(m_unk0x2b8);
	}
}

void CBrowserView::FUN_0040aad0()
{
	Refresh();
}

void CBrowserView::FUN_0040aae0()
{
	GoForward();
}

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

void CBrowserView::FUN_0040ac30()
{
	GoBack();
}

void CBrowserView::FUN_0040ac40()
{
	Stop();
}

void CBrowserView::FUN_0040ac50(CCmdUI* pCmdUI)
{
	VARIANT_BOOL busy;

	pCmdUI->Enable(m_pBrowserApp->get_Busy(&busy) == S_OK && busy == VARIANT_TRUE ? TRUE : FALSE);
}

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

CBrowserView* CBrowserView::FUN_0040b6d0(CRect rect, CWnd* parent, CString url, IDispatch* external)
{
	CFrameWnd* frame = new CFrameWnd;
	frame->Create(NULL, NULL, WS_OVERLAPPEDWINDOW, CRect(0, 0, 0, 0), parent);
	frame->ModifyStyleEx(WS_EX_CLIENTEDGE, WS_EX_TOOLWINDOW);

	CBrowserView* view = dynamic_cast<CBrowserView*>(
		XTFuncCreateView(frame, RUNTIME_CLASS(CBrowserView), NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER)
	);
	view->m_unk0x2b0 = external;

	LPCTSTR address = url;
	view->m_unk0x2b8 = address;
	view->Navigate(address);

	parent->ClientToScreen(&rect);
	frame->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER);
	frame->InitialUpdateFrame(NULL, TRUE);

	return view;
}

HRESULT FUN_0040bfb0(CComObjectNoLock<CBrowserViewExternal>** p_unk0x04, void* p_unk0x08)
{
	HRESULT result = E_OUTOFMEMORY;
	CComObjectNoLock<CBrowserViewExternal>* object = NULL;

	ATLTRY(object = new CComObjectNoLock<CBrowserViewExternal>(p_unk0x08))

	if (object != NULL) {
		object->SetVoid(p_unk0x08);
		object->InternalFinalConstructAddRef();
		result = object->FinalConstruct();
		object->InternalFinalConstructRelease();
	}

	*p_unk0x04 = object;
	return result;
}

HRESULT CBrowserView::OnGetExternal(LPDISPATCH* lppDispatch)
{
	if (!m_unk0x2b0) {
		CComObjectNoLock<CBrowserViewExternal>* external;
		HRESULT result = FUN_0040bfb0(&external, NULL);

		if (result != S_OK) {
			return result;
		}

		external->m_unk0x1c = dynamic_cast<CIDEBrowserView*>(this) ? VARIANT_TRUE : VARIANT_FALSE;
		m_unk0x2b0 = external;
	}

	return m_unk0x2b0.CopyTo(lppDispatch);
}

void CBrowserView::OnBeforeNavigate2(
	LPCTSTR lpszURL,
	DWORD nFlags,
	LPCTSTR lpszTargetFrameName,
	CByteArray& baPostedData,
	LPCTSTR lpszHeaders,
	BOOL* pbCancel
)
{
	CSecureHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
	if (*pbCancel) {
		return;
	}

	CString url(lpszURL);
	if (strlen(lpszTargetFrameName) != 0) {
		m_unk0xf8.push_back(lpszURL);
		if (m_unk0xf8.size() > 10) {
			m_unk0xf8.pop_front();
		}
	}
	else if (url.Compare("about:blank") && !RBX::Http::isScript(lpszURL) && !RBX::Http::isRobloxSite(lpszURL)) {
		bool external = url.Left(16) != "res://mshtml.dll";
		if (external) {
			external = url.Left(17) != "res://ieframe.dll";
			if (external) {
				external = url.Left(17) != "res://shdoclc.dll";
				if (external) {
					Navigate(lpszURL, 0, "_blank");
					*pbCancel = TRUE;
				}
			}
		}
	}
}

CBrowserView::CBrowserView() : m_unk0x104(NULL), m_unk0x2b4(0), m_unk0x2b5(0)
{
	FUN_004334d0(XTPImageManager());
}

CBrowserView::~CBrowserView()
{
}

void CBrowserView::OnNavigateComplete2(LPCTSTR strURL)
{
	CSecureHtmlView::OnNavigateComplete2(strURL);

	if (m_unk0x104) {
		if (std::find(m_unk0xf8.begin(), m_unk0xf8.end(), std::string(strURL)) == m_unk0xf8.end()) {
			m_unk0x104->SetEditText(strURL);
		}
	}
}

CIDEBrowserView::CIDEBrowserView()
{
	m_unk0x2b8 = FUN_00407190() + "/IDE/Start.aspx";
}

CPlayBrowserView::CPlayBrowserView()
{
	m_unk0x2b8 = FUN_00407190() + "/Start.aspx";
}

STDMETHODIMP CBrowserViewExternal::get_IsRobloxIDE(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	*pVal = m_unk0x1c;
	return S_OK;
}

STDMETHODIMP CBrowserViewExternal::GetApp(IDispatch** result)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	CComPtr<IApp> app;
	HRESULT hr = app.CoCreateInstance(CLSID_App);

	if (hr != S_OK) {
		CString message;
		message.Format("IBrowserViewExternal::GetApp-0x%X", hr);
		return LogManager::ReportCOMError(CLSID_BrowserViewExternal, message, hr);
	}

	CComPtr<IDispatch> dispatch(app);
	return dispatch.CopyTo(result);
}

OBJECT_ENTRY_AUTO(__uuidof(BrowserViewExternal), CBrowserViewExternal)

void CBrowserView::FUN_00453eb0(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

BOOL CBrowserView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
