#include "RobloxView.h"

IMPLEMENT_DYNCREATE(CRobloxView, CView)

CRobloxView::CRobloxView()
{
}

CRobloxView::~CRobloxView()
{
}

void CRobloxView::OnDraw(CDC* pDC)
{
}

// FUNCTION: ROBLOX 0x0045d8e0
boost::shared_ptr<RBX::DataModel> CRobloxView::FUN_0045d8e0() const
{
	return m_unk0x140;
}
