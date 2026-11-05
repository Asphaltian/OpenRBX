#include "RobloxView.h"

#include "ActiveDocView.h"
#include "RobloxDoc.h"

IMPLEMENT_DYNCREATE(CRobloxView, CView)

// STUB: ROBLOX 0x0045d500
void CRobloxView::onEvent(const RBX::Network::Players* source, RBX::SuperSafeChanged event)
{
	STUB(0x45d500);
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
