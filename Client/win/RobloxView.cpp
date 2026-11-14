#include "RobloxView.h"

#include "ActiveDocView.h"
#include "BrowserView.h"
#include "MainFrm.h"
#include "Roblox.h"
#include "RobloxDoc.h"

#include <G3D/System.h>
#include <algorithm>

IMPLEMENT_DYNCREATE(CRobloxView, CView)

template <class T>
static T* findService(RBX::DataModel* dataModel)
{
	return dataModel ? dataModel->find<T>() : NULL;
}

BEGIN_MESSAGE_MAP(CRobloxView, CView)
ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
ON_WM_SIZE()
ON_WM_CREATE()
ON_WM_ERASEBKGND()
ON_WM_DESTROY()
ON_MESSAGE(0x46b, FUN_0045e740)
ON_MESSAGE(0x46c, FUN_0045de20)
ON_COMMAND(0x8020, FUN_0045dbe0)
ON_COMMAND(0x80d6, FUN_0045dbf0)
ON_WM_SETFOCUS()
ON_UPDATE_COMMAND_UI(0x80fe, FUN_0045d8c0)
ON_COMMAND(0x80fe, FUN_0045de50)
ON_COMMAND(0x80fd, FUN_0045dc00)
ON_COMMAND(0x800c, FUN_0045de70)
END_MESSAGE_MAP()

BOOL CRobloxView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CRobloxView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
}

void CRobloxView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
}

void CRobloxView::onEvent(const RBX::Network::Players* source, RBX::SuperSafeChanged event)
{
	PostMessage(0x46b);
}

InsertModelFromRobloxVerb::InsertModelFromRobloxVerb(CRobloxView* view, RBX::DataModel* dataModel)
	: RBX::Verb(dataModel, "ToggleToolbox"), m_unk0x0c(view), m_unk0x10(dataModel), m_unk0x18(G3D::System::getTick())
{
}

bool InsertModelFromRobloxVerb::isEnabled() const
{
	if (G3D::System::getTick() < m_unk0x18 + 3.0) {
		return false;
	}
	return !RBX::Network::Players::clientIsPresent(m_unk0x10, true);
}

bool InsertModelFromRobloxVerb::isChecked() const
{
	return m_unk0x0c->m_unk0x1a8 != 0;
}

void InsertModelFromRobloxVerb::doIt(RBX::IDataState* dataState)
{
	m_unk0x0c->PostMessage(WM_COMMAND, 0x80fe);
}

ToggleIDEModeVerb::ToggleIDEModeVerb(RBX::VerbContainer* container)
	: RBX::Verb(container, "TogglePlayMode"), m_unk0x10(G3D::System::getTick())
{
}

bool ToggleIDEModeVerb::isEnabled() const
{
	if (G3D::System::getTick() < m_unk0x10 + 3.0) {
		return false;
	}
	return !static_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd)->m_unk0xed;
}

bool ToggleIDEModeVerb::isChecked() const
{
	return !static_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd)->m_unk0xec;
}

void ToggleIDEModeVerb::doIt(RBX::IDataState* dataState)
{
	if (isEnabled()) {
		AfxGetApp()->m_pMainWnd->PostMessage(WM_COMMAND, 0x80f5);
	}
}

void LockPlayModeVerb::doIt(RBX::IDataState* dataState)
{
	AfxGetApp()->m_pMainWnd->PostMessage(WM_COMMAND, 0x80ff);
}

bool ToggleFullscreenVerb::isChecked() const
{
	return static_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd)->m_unk0xe0;
}

void ToggleFullscreenVerb::doIt(RBX::IDataState* dataState)
{
	AfxGetApp()->m_pMainWnd->PostMessage(WM_COMMAND, 0x80f4);
}

void HelpCommand::doIt(RBX::IDataState* dataState)
{
	m_unk0x0c->PostMessage(WM_COMMAND, 0x80fd);
}

int CRobloxView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	CRect rect;
	GetClientRect(&rect);
	return m_unk0x88.Create(NULL, "Roblox", WS_CHILD | WS_VISIBLE, rect, this, 1) ? 0 : -1;
}

void CRobloxView::OnActivateFrame(UINT nState, CFrameWnd* pFrameWnd)
{
	if (nState) {
		m_unk0x88.FUN_0045fc50(true);
	}
	else {
		m_unk0x88.FUN_0045fc50(false);
	}

	CView::OnActivateFrame(nState, pFrameWnd);
}

void CRobloxView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);
	GetParentFrame()->SetActiveView(this, FALSE);
	m_unk0x88.SetFocus();
}

void CRobloxView::FUN_0045d8c0(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_unk0x1a8 != NULL);
}

boost::shared_ptr<RBX::DataModel> CRobloxView::FUN_00403290() const
{
	return m_unk0x140;
}

void CRobloxView::FUN_0045d920()
{
	CRect rect;
	GetClientRect(&rect);
	int height = rect.Height();
	int width = rect.Width();

	int viewWidth = width;
	int viewHeight = height;
	if (m_unk0x1a8 && m_unk0x1a8->m_hWnd) {
		viewWidth -= 0xe1;
	}
	if (m_unk0x150.m_hWnd) {
		viewHeight = height - 0x14;
	}

	int cy = std::max(viewHeight, 0);
	int cx = std::max(viewWidth, 0);

	if (m_unk0x88.m_hWnd) {
		m_unk0x88.MoveWindow(0, 0, cx, cy, TRUE);
	}
	if (m_unk0x1a8 && m_unk0x1a8->m_hWnd) {
		m_unk0x1a8->MoveWindow(cx, 0, 0xe1, height, TRUE);
	}
	if (m_unk0x150.m_hWnd) {
		m_unk0x150.MoveWindow(0, cy, cx, 0x14, TRUE);
	}
}

void CRobloxView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	FUN_0045d920();
}

void CRobloxView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	if (bActivate) {
		ActiveDocView::FUN_004054f0(true, static_cast<CRobloxDoc*>(GetDocument()));
		m_unk0x88.FUN_0045fc50(true);
	}
	else if (pActivateView != pDeactiveView) {
		ActiveDocView::FUN_004054f0(false, static_cast<CRobloxDoc*>(GetDocument()));
		m_unk0x88.FUN_0045fc50(false);
	}
}

void CRobloxView::FUN_0045da90(int p_unk0x04)
{
	CString filter;
	filter.LoadString(184);

	CFileDialog dialog(TRUE, "rbxm", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, this);
	if (dialog.DoModal() == IDOK) {
		static_cast<CRobloxDoc*>(GetDocument())
			->m_unk0x78->FUN_004702b0(RBX::ContentId::fromFile(static_cast<LPCTSTR>(dialog.GetPathName())), false);
	}
}

void CRobloxView::FUN_0045dbe0()
{
	FUN_0045da90(2);
}

void CRobloxView::FUN_0045dbf0()
{
	FUN_0045da90(0);
}

void CRobloxView::FUN_0045dc00()
{
	CRect rect;
	m_unk0x88.GetClientRect(&rect);
	rect.InflateRect(-12, -12);
	rect.right = rect.Width() / 2;
	rect.bottom = std::min(480, rect.Height()) - rect.top;
	CBrowserView::FUN_0040b6d0(rect, &m_unk0x88, FUN_00407190() + "/Game/Help.aspx", NULL);
}

void CRobloxView::FUN_0045dd10(bool show, bool activate)
{
	bool shown = m_unk0x1a8 != NULL;
	if (show == shown) {
		return;
	}

	if (show) {
		m_unk0x1a8 = static_cast<CWebToolbox*>(RUNTIME_CLASS(CWebToolbox)->CreateObject());
		m_unk0x1a8->m_unk0xf8 = static_cast<CRobloxDoc*>(GetDocument())->m_unk0x78;
		m_unk0x1a8->m_unk0xf4 = &m_unk0x88;

		CRect rect(0, 0, 0, 0);
		if (!m_unk0x1a8->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, this, 0x413)) {
			m_unk0x1a8 = NULL;
			return;
		}

		if (activate) {
			m_unk0x1a8->SendMessage(WM_INITIALUPDATE);
		}
	}
	else {
		m_unk0x1a8->DestroyWindow();
		m_unk0x1a8 = NULL;
	}

	FUN_0045d920();
}

LRESULT CRobloxView::FUN_0045de20(WPARAM wParam, LPARAM lParam)
{
	if (m_unk0x1a8) {
		m_unk0x1a8->DestroyWindow();
		m_unk0x1a8 = NULL;
		FUN_0045d920();
	}

	return 0;
}

void CRobloxView::FUN_0045de50()
{
	FUN_0045dd10(m_unk0x1a8 == NULL, true);
}

void CRobloxView::FUN_0045de70()
{
	FUN_0045dd10(true, true);
	m_unk0x1a8->FUN_0046ea00(9);
}

void CRobloxView::onEvent(const RBX::ServiceProvider* source, RBX::ServiceRemoving event)
{
	if (event.service->getClassName() == "NetworkClient") {
		PostMessage(0x46c);
	}

	if (event.service->getClassName() == "NetworkClient" || event.service->getClassName() == "Players") {
		PostMessage(0x46b);
	}

	if (event.service->getClassName() == "Players") {
		static_cast<RBX::Network::Players*>(event.service.get())
			->RBX::Notifier<RBX::Network::Players, RBX::SuperSafeChanged>::removeListener(this);
	}
}

CRobloxView::~CRobloxView()
{
}

CRobloxView::CRobloxView() : RBX::VerbContainer(NULL), m_unk0x88(false), m_unk0x1a8(0)
{
	m_unk0x148.LoadMenu(0x67);
}

void CRobloxView::onEvent(const RBX::ServiceProvider* source, RBX::ServiceAdded event)
{
	if (event.service->getClassName() == "NetworkClient" || event.service->getClassName() == "Players") {
		PostMessage(0x46b);
	}

	if (event.service->getClassName() == "Players") {
		static_cast<RBX::Network::Players*>(event.service.get())
			->RBX::Notifier<RBX::Network::Players, RBX::SuperSafeChanged>::addListener(this);
	}
}

void CRobloxView::OnDraw(CDC* pDC)
{
}

LRESULT CRobloxView::FUN_0045e740(WPARAM wParam, LPARAM lParam)
{
	if (m_hWnd) {
		RBX::DataModel::Lock lock(m_unk0x140);

		bool showPane = false;
		RBX::Network::Players* players = findService<RBX::Network::Players>(FUN_00403290().get());
		if (players) {
			if (RBX::Network::Players::clientIsPresent(FUN_00403290().get(), true) && !players->superSafeOn()) {
				showPane = true;
			}
		}

		bool paneShown = m_unk0x150.m_hWnd != NULL;
		if (showPane != paneShown) {
			if (showPane) {
				m_unk0x150.FUN_00436ff0(this, FUN_00403290().get());
			}
			else {
				m_unk0x150.DestroyWindow();
			}

			FUN_0045d920();
		}
	}

	return 0;
}

void CRobloxView::FUN_0045e900()
{
	RBX::DataModel::Lock lock(m_unk0x140);
	m_unk0x1ac.reset();
	m_unk0x1b0.reset();
	m_unk0x1b4.reset();
	m_unk0x1b8.reset();
	m_unk0x1bc.reset();
	RBX::stopListening<RBX::ServiceProvider, RBX::ServiceAdded>(m_unk0x140, this);
	RBX::stopListening<RBX::ServiceProvider, RBX::ServiceRemoving>(m_unk0x140, this);

	RBX::Network::Players* players = findService<RBX::Network::Players>(FUN_00403290().get());
	if (players) {
		players->RBX::Notifier<RBX::Network::Players, RBX::SuperSafeChanged>::removeListener(this);
	}
}

void CRobloxView::OnInitialUpdate()
{
	try {
		FUN_0045e900();
		m_unk0x88.FUN_00463c30(NULL);
		m_unk0x140.reset();
		CView::OnInitialUpdate();

		m_unk0x88.FUN_00463c30(static_cast<CRobloxDoc*>(GetDocument())->m_unk0x78);
		m_unk0x140 = static_cast<CRobloxDoc*>(GetDocument())->m_unk0x78->FUN_0046edc0();

		RBX::DataModel::Lock lock(m_unk0x140);
		if (m_unk0x1a8) {
			m_unk0x1a8->m_unk0xf8 = static_cast<CRobloxDoc*>(GetDocument())->m_unk0x78;
		}
		RBX::startListening(m_unk0x140, static_cast<RBX::Listener<RBX::ServiceProvider, RBX::ServiceAdded>*>(this));
		RBX::startListening(m_unk0x140, static_cast<RBX::Listener<RBX::ServiceProvider, RBX::ServiceRemoving>*>(this));
		setVerbParent(m_unk0x140->getWorkspace());

		m_unk0x1b0.reset(new InsertModelFromRobloxVerb(this, m_unk0x140.get()));
		m_unk0x1ac.reset(new HelpCommand(m_unk0x140.get(), this));
		m_unk0x1b4.reset(new ToggleFullscreenVerb(m_unk0x140.get()));
		m_unk0x1b8.reset(new ToggleIDEModeVerb(m_unk0x140.get()));
		m_unk0x1bc.reset(new LockPlayModeVerb(m_unk0x140.get()));

		bind(0x8027, m_unk0x140->getWorkspace()->getVerb("CameraPanLeft"));
		bind(0x8028, m_unk0x140->getWorkspace()->getVerb("CameraPanRight"));
		bind(0x8029, m_unk0x140->getWorkspace()->getVerb("CameraTiltUp"));
		bind(0x802a, m_unk0x140->getWorkspace()->getVerb("CameraTiltDown"));
		bind(0x802b, m_unk0x140->getWorkspace()->getVerb("CameraZoomIn"));
		bind(0x802c, m_unk0x140->getWorkspace()->getVerb("CameraZoomOut"));
		bind(0x80a6, m_unk0x140->getWorkspace()->getVerb("CameraCenter"));
		bind(0x802d, m_unk0x140->getWorkspace()->getVerb("CameraZoomExtents"));
		bind(0x80d7, m_unk0x140->getWorkspace()->getVerb("CameraModelView"));
		bind(0x802f, RBX::Name::lookup("FixedCamera"));
		bind(0x8030, RBX::Name::lookup("AttachCamera"));
		bind(0x80a3, RBX::Name::lookup("TrackCamera"));
		bind(0x8032, RBX::Name::lookup("FollowCamera"));
		bind(0x80a2, RBX::Name::lookup("WatchCamera"));
		bind(0x8043, RBX::Name::lookup("SelectionRotate"));
		bind(0x8044, RBX::Name::lookup("SelectionTilt"));

		m_unk0x88.FUN_0045faf0();
	}
	catch (std::exception& e) {
		FUN_0044ebb0(e);
	}
}

BOOL CRobloxView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CRobloxView::OnDestroy()
{
	FUN_0045e900();
	CView::OnDestroy();
}
