#include "RobloxView.h"

#include "ActiveDocView.h"
#include "MainFrm.h"
#include "RobloxDoc.h"

#include <G3D/System.h>
#include <algorithm>

IMPLEMENT_DYNCREATE(CRobloxView, CView)

BEGIN_MESSAGE_MAP(CRobloxView, CView)
ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
ON_WM_SIZE()
ON_WM_CREATE()
ON_WM_ERASEBKGND()
ON_WM_DESTROY()
ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// STUB: ROBLOX 0x0045d500
void CRobloxView::onEvent(const RBX::Network::Players* source, RBX::SuperSafeChanged event)
{
	STUB(0x45d500);
}

// FUNCTION: ROBLOX 0x0045d520
InsertModelFromRobloxVerb::InsertModelFromRobloxVerb(CRobloxView* view, RBX::DataModel* dataModel)
	: RBX::Verb(dataModel, "ToggleToolbox"), m_unk0x0c(view), m_unk0x10(dataModel), m_unk0x18(G3D::System::getTick())
{
}

// FUNCTION: ROBLOX 0x0045d5b0
bool InsertModelFromRobloxVerb::isEnabled() const
{
	if (G3D::System::getTick() < m_unk0x18 + 3.0) {
		return false;
	}
	return !RBX::Network::Players::clientIsPresent(m_unk0x10, true);
}

// FUNCTION: ROBLOX 0x0045d5f0
bool InsertModelFromRobloxVerb::isChecked() const
{
	return m_unk0x0c->m_unk0x1a8 != 0;
}

// FUNCTION: ROBLOX 0x0045d610
void InsertModelFromRobloxVerb::doIt(RBX::IDataState* dataState)
{
	m_unk0x0c->PostMessage(WM_COMMAND, 0x80fe);
}

// FUNCTION: ROBLOX 0x0045d630
ToggleIDEModeVerb::ToggleIDEModeVerb(RBX::VerbContainer* container)
	: RBX::Verb(container, "TogglePlayMode"), m_unk0x10(G3D::System::getTick())
{
}

// FUNCTION: ROBLOX 0x0045d6b0
bool ToggleIDEModeVerb::isEnabled() const
{
	if (G3D::System::getTick() < m_unk0x10 + 3.0) {
		return false;
	}
	return !static_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd)->m_unk0xed;
}

// FUNCTION: ROBLOX 0x0045d6f0
bool ToggleIDEModeVerb::isChecked() const
{
	return !static_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd)->m_unk0xec;
}

// FUNCTION: ROBLOX 0x0045d710
void ToggleIDEModeVerb::doIt(RBX::IDataState* dataState)
{
	if (isEnabled()) {
		AfxGetApp()->m_pMainWnd->PostMessage(WM_COMMAND, 0x80f5);
	}
}

// FUNCTION: ROBLOX 0x0045d740
void LockPlayModeVerb::doIt(RBX::IDataState* dataState)
{
	AfxGetApp()->m_pMainWnd->PostMessage(WM_COMMAND, 0x80ff);
}

// FUNCTION: ROBLOX 0x0045d770
bool ToggleFullscreenVerb::isChecked() const
{
	return static_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd)->m_unk0xe0;
}

// FUNCTION: ROBLOX 0x0045d790
void ToggleFullscreenVerb::doIt(RBX::IDataState* dataState)
{
	AfxGetApp()->m_pMainWnd->PostMessage(WM_COMMAND, 0x80f4);
}

// FUNCTION: ROBLOX 0x0045d7c0
void HelpCommand::doIt(RBX::IDataState* dataState)
{
	m_unk0x0c->PostMessage(WM_COMMAND, 0x80fd);
}

// FUNCTION: ROBLOX 0x0045d7e0
int CRobloxView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	CRect rect;
	GetClientRect(&rect);
	return m_unk0x88.Create(NULL, "Roblox", WS_CHILD | WS_VISIBLE, rect, this, 1) ? 0 : -1;
}

// FUNCTION: ROBLOX 0x0045d880
void CRobloxView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);
	GetParentFrame()->SetActiveView(this, FALSE);
	m_unk0x88.SetFocus();
}

// FUNCTION: ROBLOX 0x0045d8e0
boost::shared_ptr<RBX::DataModel> CRobloxView::FUN_00403290() const
{
	return m_unk0x140;
}

// FUNCTION: ROBLOX 0x0045d920
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

// FUNCTION: ROBLOX 0x0045da00
void CRobloxView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	FUN_0045d920();
}

// FUNCTION: ROBLOX 0x0045da20
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

// STUB: ROBLOX 0x0045de90
void CRobloxView::onEvent(const RBX::ServiceProvider* source, RBX::ServiceRemoving event)
{
	STUB(0x45de90);
}

// FUNCTION: ROBLOX 0x0045e2e0
CRobloxView::~CRobloxView()
{
}

// FUNCTION: ROBLOX 0x0045e4a0
CRobloxView::CRobloxView() : RBX::VerbContainer(NULL), m_unk0x88(false), m_unk0x1a8(0)
{
	m_unk0x148.LoadMenu(0x67);
}

// STUB: ROBLOX 0x0045e630
void CRobloxView::onEvent(const RBX::ServiceProvider* source, RBX::ServiceAdded event)
{
	STUB(0x45e630);
}

void CRobloxView::OnDraw(CDC* pDC)
{
}

// STUB: ROBLOX 0x0045e900
void CRobloxView::FUN_0045e900()
{
	RBX::DataModel::Lock lock(m_unk0x140);
	m_unk0x1ac.reset();
	m_unk0x1b0.reset();
	m_unk0x1b4.reset();
	m_unk0x1b8.reset();
	m_unk0x1bc.reset();
	RBX::stopListening(m_unk0x140, static_cast<RBX::Listener<RBX::ServiceProvider, RBX::ServiceAdded>*>(this));
	RBX::stopListening(m_unk0x140, static_cast<RBX::Listener<RBX::ServiceProvider, RBX::ServiceRemoving>*>(this));

	RBX::Network::Players* players;
	{
		boost::shared_ptr<RBX::DataModel> dataModel = FUN_00403290();
		players = dataModel ? dataModel->find<RBX::Network::Players>() : NULL;
	}
	if (players) {
		players->RBX::Notifier<RBX::Network::Players, RBX::SuperSafeChanged>::removeListener(this);
	}
}

// STUB: ROBLOX 0x0045eab0
void CRobloxView::OnInitialUpdate()
{
	FUN_0045e900();
	m_unk0x88.FUN_00463c30(NULL);
	m_unk0x140.reset();
	CView::OnInitialUpdate();

	CRobloxDoc* document = static_cast<CRobloxDoc*>(GetDocument());
	m_unk0x88.FUN_00463c30(document->m_unk0x78);
	m_unk0x140 = document->m_unk0x78->FUN_0046edc0();

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

	RBX::Verb* verb;
	verb = m_unk0x140->getWorkspace()->getVerb("CameraPanLeft");
	m_unk0x58[0x8027] = verb ? &verb->getName() : &RBX::Name::getNullName();
	verb = m_unk0x140->getWorkspace()->getVerb("CameraPanRight");
	m_unk0x58[0x8028] = verb ? &verb->getName() : &RBX::Name::getNullName();
	verb = m_unk0x140->getWorkspace()->getVerb("CameraTiltUp");
	m_unk0x58[0x8029] = verb ? &verb->getName() : &RBX::Name::getNullName();
	verb = m_unk0x140->getWorkspace()->getVerb("CameraTiltDown");
	m_unk0x58[0x802a] = verb ? &verb->getName() : &RBX::Name::getNullName();
	verb = m_unk0x140->getWorkspace()->getVerb("CameraZoomIn");
	m_unk0x58[0x802b] = verb ? &verb->getName() : &RBX::Name::getNullName();
	verb = m_unk0x140->getWorkspace()->getVerb("CameraZoomOut");
	m_unk0x58[0x802c] = verb ? &verb->getName() : &RBX::Name::getNullName();
	verb = m_unk0x140->getWorkspace()->getVerb("CameraCenter");
	m_unk0x58[0x80a6] = verb ? &verb->getName() : &RBX::Name::getNullName();
	verb = m_unk0x140->getWorkspace()->getVerb("CameraZoomExtents");
	m_unk0x58[0x802d] = verb ? &verb->getName() : &RBX::Name::getNullName();
	verb = m_unk0x140->getWorkspace()->getVerb("CameraModelView");
	m_unk0x58[0x80d7] = verb ? &verb->getName() : &RBX::Name::getNullName();
	m_unk0x58[0x802f] = &RBX::Name::lookup("FixedCamera");
	m_unk0x58[0x8030] = &RBX::Name::lookup("AttachCamera");
	m_unk0x58[0x80a3] = &RBX::Name::lookup("TrackCamera");
	m_unk0x58[0x8032] = &RBX::Name::lookup("FollowCamera");
	m_unk0x58[0x80a2] = &RBX::Name::lookup("WatchCamera");
	m_unk0x58[0x8043] = &RBX::Name::lookup("SelectionRotate");
	m_unk0x58[0x8044] = &RBX::Name::lookup("SelectionTilt");

	m_unk0x88.FUN_0045faf0();
}

BOOL CRobloxView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

// FUNCTION: ROBLOX 0x0045f280
void CRobloxView::OnDestroy()
{
	FUN_0045e900();
	CView::OnDestroy();
}
