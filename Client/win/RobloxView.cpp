#include "RobloxView.h"

#include "ActiveDocView.h"
#include "MainFrm.h"
#include "RobloxDoc.h"

#include <G3D/System.h>

IMPLEMENT_DYNCREATE(CRobloxView, CView)

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

// FUNCTION: ROBLOX 0x0045d8e0
boost::shared_ptr<RBX::DataModel> CRobloxView::FUN_00403290() const
{
	return m_unk0x140;
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
