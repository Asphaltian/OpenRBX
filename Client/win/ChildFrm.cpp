#include "ChildFrm.h"

#include "MainFrm.h"
#include "RenderSettings.h"

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

void CRbxChildFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CChildFrame::OnGetMinMaxInfo(lpMMI);

	CMainFrame* frame = static_cast<CMainFrame*>(GetParentFrame());
	if (frame && frame->m_unk0xe0 && frame->GetCommandBars()->GetCount() == 0) {
		lpMMI->ptMaxSize.x += 40;
		lpMMI->ptMaxSize.y += 40;
		lpMMI->ptMaxTrackSize.x += 40;
		lpMMI->ptMaxTrackSize.y += 40;
	}

	if (!IsZoomed()) {
		G3D::Vector2int16 size = CRenderSettings::singleton().FUN_00448300();
		CRect rect(0, 0, size.x, size.y);
		CalcWindowRect(&rect);
		lpMMI->ptMaxTrackSize.x = rect.Width();
		lpMMI->ptMaxTrackSize.y = rect.Height();
	}
}
