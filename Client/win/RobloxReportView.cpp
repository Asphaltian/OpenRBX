#include "RobloxReportView.h"

#include "ActiveDocView.h"
#include "RobloxDoc.h"

IMPLEMENT_DYNCREATE(CRobloxReportDocView, CRobloxReportView)
IMPLEMENT_DYNAMIC(CRobloxReportView, CXTPReportView)
IMPLEMENT_DYNCREATE(CRobloxReportPaneView, CRobloxReportView)

CRobloxReportView::CRobloxReportView()
{
}

CRobloxReportView::~CRobloxReportView()
{
}

// FUNCTION: ROBLOX 0x0045afb0
void CRobloxReportDocView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (bActivate) {
		ActiveDocView::FUN_004054f0(true, static_cast<CRobloxDoc*>(GetDocument()));
	}
	else if (pActivateView != pDeactiveView) {
		ActiveDocView::FUN_004054f0(false, static_cast<CRobloxDoc*>(GetDocument()));
	}
}
