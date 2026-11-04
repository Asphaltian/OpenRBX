#include "ObjectBrowser.h"

#include "MemberTreeView.h"

IMPLEMENT_DYNCREATE(CObjectBrowser, CView)
IMPLEMENT_DYNCREATE(CMemberTreeView, CXTTreeView)

// FUNCTION: ROBLOX 0x00437620
CObjectBrowser::CObjectBrowser()
{
}

// FUNCTION: ROBLOX 0x00437690
CObjectBrowser::~CObjectBrowser()
{
}

// FUNCTION: ROBLOX 0x00437860
CMemberTreeView::CMemberTreeView() : m_unk0xa8(1), m_unk0xac(0)
{
}

// STUB: ROBLOX 0x004378e0
CMemberTreeView::~CMemberTreeView()
{
}

void CObjectBrowser::OnDraw(CDC* pDC)
{
}
