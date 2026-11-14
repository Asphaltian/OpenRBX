#include "NullDoc.h"

IMPLEMENT_DYNCREATE(CBrowserFrameWnd, CXTPFrameWnd)

int CBrowserFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPFrameWnd::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	CMenu* menu = GetMenu();
	if (menu && ::IsMenu(menu->m_hMenu)) {
		menu->DestroyMenu();
		::SetMenu(m_hWnd, NULL);
	}

	CWnd* pane = GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);
	pane->ModifyStyle(WS_BORDER, 0, SWP_FRAMECHANGED);
	pane->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
	return 0;
}

// clang-format off
BEGIN_MESSAGE_MAP(CBrowserFrameWnd, CXTPFrameWnd)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()
// clang-format on

IMPLEMENT_DYNCREATE(CNullDoc, CDocument)

void CNullDoc::FUN_004374b0(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

// clang-format off
BEGIN_MESSAGE_MAP(CNullDoc, CDocument)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, FUN_004374b0)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, FUN_004374b0)
	ON_UPDATE_COMMAND_UI(0x80b6, FUN_004374b0)
END_MESSAGE_MAP()
// clang-format on
