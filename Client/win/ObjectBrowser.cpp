#include "ObjectBrowser.h"

#include "ClassTreeView.h"
#include "MemberTreeView.h"

IMPLEMENT_DYNCREATE(CClassTreeView, CXTTreeView)

// STUB: ROBLOX 0x00437590
CClassTreeView::~CClassTreeView()
{
}

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

// FUNCTION: ROBLOX 0x00437b30
CClassTreeView::CClassTreeView() : m_unk0xa8(0)
{
}

void CObjectBrowser::OnDraw(CDC* pDC)
{
}
