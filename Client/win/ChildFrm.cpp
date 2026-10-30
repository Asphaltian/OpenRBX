#include "ChildFrm.h"

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

// clang-format off
BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()
// clang-format on

CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}

// FUNCTION: ROBLOX 0x0040f830
BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.y = 0;
	cs.x = 0;
	cs.cy = 0x7fff;
	cs.cx = 0x7fff;

	if (!CMDIChildWnd::PreCreateWindow(cs)) {
		return FALSE;
	}

	cs.style |= WS_CLIPCHILDREN;
	return TRUE;
}

// FUNCTION: ROBLOX 0x0040f870
void CChildFrame::ActivateFrame(int nCmdShow)
{
	if (GetParent()->GetWindow(GW_CHILD) == this) {
		nCmdShow = SW_SHOWMAXIMIZED;
	}

	CMDIChildWnd::ActivateFrame(nCmdShow);
}

IMPLEMENT_DYNCREATE(CRbxChildFrame, CChildFrame)

// clang-format off
BEGIN_MESSAGE_MAP(CRbxChildFrame, CChildFrame)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()
// clang-format on

// FUNCTION: ROBLOX 0x0040f8c0
int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	m_hMenuShared = NULL;

	CMenu* menu = GetSystemMenu(FALSE);
	menu->RemoveMenu(SC_RESTORE, MF_BYCOMMAND);
	menu->RemoveMenu(SC_MINIMIZE, MF_BYCOMMAND);
	menu->RemoveMenu(SC_MAXIMIZE, MF_BYCOMMAND);
	menu->RemoveMenu(SC_MOVE, MF_BYCOMMAND);
	menu->RemoveMenu(SC_SIZE, MF_BYCOMMAND);

	CWnd* view = GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);
	view->ModifyStyle(WS_BORDER, 0);
	view->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
	return 0;
}

// STUB: ROBLOX 0x0040fe00
void CRbxChildFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	STUB(0x40fe00);
	CChildFrame::OnGetMinMaxInfo(lpMMI);
}
