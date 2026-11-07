#include "MainFrm.h"

#include "PropGrid.h"
#include "RBXImage.h"
#include "RenderSettings.h"
#include "Roblox.h"
#include "RobloxReportView.h"
#include "SelectionCaption.h"
#include "SettingsDialog.h"
#include "StandardOutputView.h"
#include "WebToolbox.h"
#include "util/FileSystem.h"
#include "util/Log.h"
#include "util/StandardOut.h"
#include "util/scopedassign.h"
#include "v8datamodel/Accoutrement.h"
#include "v8datamodel/ToolsPart.h"

#include <G3D/Color3uint8.h>
#include <G3D/GImage.h>
#include <GLG3D/Texture.h>
#include <GLG3D/VARArea.h>

// GLOBAL: ROBLOX 0x0095391d
bool g_unk0x0095391d;

// GLOBAL: ROBLOX 0x009187f0
static UINT indicators[] = {ID_SEPARATOR, 0xbb, 0xbe, 0xbc, 0xbd};

// GLOBAL: ROBLOX 0x00918808
static UINT hiddenCommands[] = {0xe100, 0xe102, 0x8025, 0x80b7, 0xe123, 0xe120, 0x8005, 0x8010, 0x8011,
								0x805d, 0x805e, 0xe800, 0x88b9, 0xe801, 0x8042, 0x8018, 0x800f, 0x809f,
								0x80bf, 0x803d, 0x8061, 0x80a5, 0x8013, 0x8014, 0x8047, 0xe130};

IMPLEMENT_DYNAMIC(CMainFrame, CXTPMDIFrameWnd)

IMPLEMENT_DYNCREATE(MyXTPCommandBars, CXTPCommandBars)

// FUNCTION: ROBLOX 0x00432550
bool CMainFrame::FUN_00432550(int nCmdShow)
{
	m_unk0xf4.LoadWindowPos(this, "CMainFrameV2");

	if (nCmdShow) {
		nCmdShow = m_unk0xf4.showCmd;
	}

	return ShowWindow(nCmdShow) != FALSE;
}

// FUNCTION: ROBLOX 0x00432590
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CXTPMDIFrameWnd::PreCreateWindow(cs)) {
		return FALSE;
	}

	cs.lpszClass = AfxRegisterWndClass(0, NULL, NULL, AfxGetApp()->LoadIcon(128));
	return TRUE;
}

// FUNCTION: ROBLOX 0x004325e0
void CMainFrame::FUN_004325e0(UINT nID, CRect rect, XTPDockingPaneDirection direction)
{
	CXTPDockingPane* pane = m_unk0x120.FindPane(nID);

	if (pane == NULL) {
		pane = m_unk0x120.CreatePane(nID, rect, direction);
	}

	m_unk0x120.ShowPane(pane);
	pane->SetFocus();
}

// FUNCTION: ROBLOX 0x00432650
void CMainFrame::FUN_00432650()
{
	FUN_004325e0(0xc2, CRect(0, 0, 250, 300), xtpPaneDockRight);
}

// FUNCTION: ROBLOX 0x00432680
void CMainFrame::FUN_00432680()
{
	bool created = m_unk0x120.FindPane(0xc1) == NULL;
	FUN_004325e0(0xc1, CRect(0, 0, 250, 300), xtpPaneDockRight);

	if (created) {
		CXTPDockingPane* neighbour = m_unk0x120.FindPane(0xc2);

		if (neighbour != NULL) {
			m_unk0x120.DockPane(m_unk0x120.FindPane(0xc1), xtpPaneDockBottom, neighbour);
		}
	}
}

IMPLEMENT_XTP_CONTROL(CPatchedControlComboBox, CXTPControlComboBox)

// FUNCTION: ROBLOX 0x00432760
CWrapperView::CWrapperView()
{
}

// FUNCTION: ROBLOX 0x004327c0
BOOL CWrapperView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CView::PreCreateWindow(cs)) {
		return FALSE;
	}

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return TRUE;
}

// FUNCTION: ROBLOX 0x004327f0
BOOL CWrapperView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) {
		return TRUE;
	}

	return m_unk0x58.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// FUNCTION: ROBLOX 0x00432840
BOOL CWrapperView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL result = CView::OnWndMsg(message, wParam, lParam, pResult);
	if (result && *pResult != 0) {
		return TRUE;
	}

	switch (message) {
	case WM_CREATE: {
		CRect rect(0, 0, 100, 100);
		if (!m_unk0x58.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, this, 0)) {
			*pResult = -1;
			return TRUE;
		}

		m_unk0x58.ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_FRAMECHANGED);
		return TRUE;
	}
	case WM_SIZE:
		if (m_unk0x58.m_hWnd != NULL) {
			m_unk0x58.MoveWindow(0, 0, LOWORD(lParam), HIWORD(lParam));
		}

		return TRUE;
	}

	return result;
}

void CWrapperView::OnDraw(CDC* pDC)
{
}

IMPLEMENT_DYNCREATE(CWrapperView, CView)

// FUNCTION: ROBLOX 0x00432980
void CMainFrame::FUN_00432980()
{
	static_cast<CRobloxApp*>(AfxGetApp())->FUN_0044e250();
}

// FUNCTION: ROBLOX 0x00432990
void CMainFrame::FUN_00432990(CCmdUI* pCmdUI)
{
	pCmdUI->SetText("");
}

// FUNCTION: ROBLOX 0x004329b0
void CMainFrame::FUN_004329b0(CCmdUI* pCmdUI)
{
	std::string mem = RBX::Log::formatMem(G3D::VARArea::sizeOfAllVARAreasInMemory());
	CString text;
	text.Format("VAR %s", mem.c_str());
	pCmdUI->SetText(text);
}

// FUNCTION: ROBLOX 0x00432a80
void CMainFrame::FUN_00432a80(CCmdUI* pCmdUI)
{
	std::string mem = RBX::Log::formatMem(G3D::Texture::sizeOfAllTexturesInMemory());
	CString text;
	text.Format("TEX %s", mem.c_str());
	pCmdUI->SetText(text);
}

// FUNCTION: ROBLOX 0x00432b50
void CMainFrame::FUN_00432b50()
{
	CSettingsDialog::FUN_0046b3f0(this);
}

// FUNCTION: ROBLOX 0x00432b60
void CMainFrame::FUN_00432b60()
{
	FUN_004325e0(0x8c, CRect(0, 0, 150, 400), xtpPaneDockLeft);
}

// FUNCTION: ROBLOX 0x00432b90
void CMainFrame::FUN_00432b90(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_unk0xec);
}

// FUNCTION: ROBLOX 0x00432bb0
void CMainFrame::ActivateFrame(int nCmdShow)
{
	CXTPMDIFrameWnd::ActivateFrame(nCmdShow);
}

// FUNCTION: ROBLOX 0x00432bc0
void CMainFrame::FUN_00432bc0(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_unk0xe0);
}

// FUNCTION: ROBLOX 0x00432be0
void CMainFrame::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CXTPMDIFrameWnd::OnWindowPosChanging(lpwndpos);

	if (m_unk0xe0 && !m_unk0xe1) {
		lpwndpos->flags |= SWP_NOSIZE | SWP_NOMOVE;
	}
}

// STUB: ROBLOX 0x004331e0
static bool FUN_004331e0(CXTPImageManager* imageManager, UINT id, LPCTSTR name, XTPImageState state)
{
	HINSTANCE instance = AfxFindResourceHandle(name, "PNG");
	if (instance == NULL) {
		return false;
	}
	HRSRC resource = FindResource(instance, name, "PNG");
	if (resource == NULL) {
		return false;
	}
	HGLOBAL global = LoadResource(instance, resource);
	if (global == NULL) {
		return false;
	}
	const G3D::uint8* data = static_cast<const G3D::uint8*>(LockResource(global));
	if (data == NULL) {
		return false;
	}

	G3D::GImage image(data, SizeofResource(instance, resource), G3D::GImage::PNG);
	for (int x = 0; x < image.width; x++) {
		for (int y = 0; y < image.height; y++) {
			G3D::uint8* pixel = image.byte() + (y * image.width + x) * 4;
			std::swap(pixel[0], pixel[2]);
		}
	}

	RBXImage bitmap;
	if (!bitmap.FUN_00445f50(
			image.width, image.height, image.channels * 8, image.channels == 4 ? RBXImage::c_createAlphaChannel : 0
		)) {
		return false;
	}
	bitmap.FUN_00445c70();
	for (int y = 0; y < image.height; y++) {
		memcpy(bitmap.FUN_00445c40(0, y), image.byte() + y * image.width * 4, image.width * 4);
	}
	BOOL result = imageManager->SetIcon(CXTPImageManagerIconHandle((HBITMAP) bitmap), id, CSize(0, 0), state);
	FreeResource(global);
	return result != FALSE;
}

// FUNCTION: ROBLOX 0x004333d0
static void FUN_004333d0(CXTPImageManager* imageManager, UINT id, LPCTSTR name, int size)
{
	CString resourceName;
	resourceName.Format("\"%s_%d\"", name, size);
	if (FUN_004331e0(imageManager, id, resourceName.MakeUpper(), xtpImageNormal)) {
		resourceName.Format("\"%s_%d_D\"", name, size);
		FUN_004331e0(imageManager, id, resourceName.MakeUpper(), xtpImageDisabled);
		resourceName.Format("\"%s_%d_H\"", name, size);
		FUN_004331e0(imageManager, id, resourceName.MakeUpper(), xtpImageHot);
	}
}

// FUNCTION: ROBLOX 0x004334d0
void FUN_004334d0(CXTPImageManager* imageManager)
{
	imageManager->RemoveIcon(0x80bb);
	FUN_004333d0(imageManager, 0x80bb, "stop", 16);
	FUN_004333d0(imageManager, 0x80bb, "stop", 32);
	imageManager->RemoveIcon(0x80bd);
	FUN_004333d0(imageManager, 0x80bd, "home_brown", 16);
	FUN_004333d0(imageManager, 0x80bd, "home_brown", 32);
	imageManager->RemoveIcon(0x80bc);
	FUN_004333d0(imageManager, 0x80bc, "refresh_doc", 16);
	FUN_004333d0(imageManager, 0x80bc, "refresh_doc", 32);
	imageManager->RemoveIcon(0x80ba);
	FUN_004333d0(imageManager, 0x80ba, "arrowleft_green", 16);
	FUN_004333d0(imageManager, 0x80ba, "arrowleft_green", 32);
	imageManager->RemoveIcon(0x80b9);
	FUN_004333d0(imageManager, 0x80b9, "arrowright_green", 16);
	FUN_004333d0(imageManager, 0x80b9, "arrowright_green", 32);
}

// FUNCTION: ROBLOX 0x004335e0
static void FUN_004335e0(CXTPImageManager* imageManager)
{
	FUN_004334d0(imageManager);
	imageManager->RemoveIcon(0xe100);
	FUN_004333d0(imageManager, 0xe100, "new_document", 16);
	FUN_004333d0(imageManager, 0xe100, "new_document", 32);
	imageManager->RemoveIcon(0xe101);
	FUN_004333d0(imageManager, 0xe101, "open_document", 16);
	FUN_004333d0(imageManager, 0xe101, "open_document", 32);
	imageManager->RemoveIcon(0xe103);
	FUN_004333d0(imageManager, 0xe103, "save", 16);
	FUN_004333d0(imageManager, 0xe103, "save", 32);
	imageManager->RemoveIcon(0xe12b);
	FUN_004333d0(imageManager, 0xe12b, "undo", 16);
	FUN_004333d0(imageManager, 0xe12b, "undo", 32);
	imageManager->RemoveIcon(0xe12c);
	FUN_004333d0(imageManager, 0xe12c, "redo", 16);
	FUN_004333d0(imageManager, 0xe12c, "redo", 32);
	imageManager->RemoveIcon(0xe120);
	FUN_004333d0(imageManager, 0xe120, "delete_x", 16);
	FUN_004333d0(imageManager, 0xe120, "delete_x", 32);
	imageManager->RemoveIcon(0xe122);
	FUN_004333d0(imageManager, 0xe122, "copy_clipboard", 16);
	FUN_004333d0(imageManager, 0xe122, "copy_clipboard", 32);
	imageManager->RemoveIcon(0xe123);
	FUN_004333d0(imageManager, 0xe123, "cut_clipboard", 16);
	FUN_004333d0(imageManager, 0xe123, "cut_clipboard", 32);
	imageManager->RemoveIcon(0xe125);
	FUN_004333d0(imageManager, 0xe125, "paste_clipboard", 16);
	FUN_004333d0(imageManager, 0xe125, "paste_clipboard", 32);
	imageManager->RemoveIcon(0x8040);
	FUN_004333d0(imageManager, 0x8040, "paintcan", 16);
	FUN_004333d0(imageManager, 0x8040, "paintcan", 32);
	imageManager->RemoveIcon(0x8073);
	FUN_004333d0(imageManager, 0x8073, "lock", 16);
	FUN_004333d0(imageManager, 0x8073, "lock", 32);
	imageManager->RemoveIcon(0x8010);
	FUN_004333d0(imageManager, 0x8010, "lock", 16);
	FUN_004333d0(imageManager, 0x8010, "lock", 32);
	imageManager->RemoveIcon(0x8011);
	FUN_004333d0(imageManager, 0x8011, "lock_open", 16);
	FUN_004333d0(imageManager, 0x8011, "lock_open", 32);
	imageManager->RemoveIcon(0x80da);
	FUN_004333d0(imageManager, 0x80da, "recycle", 16);
	FUN_004333d0(imageManager, 0x80da, "recycle", 32);
	imageManager->RemoveIcon(0x80f1);
	FUN_004333d0(imageManager, 0x80f1, "script_go", 16);
	FUN_004333d0(imageManager, 0x80f1, "script_go", 32);
	imageManager->RemoveIcon(0x80f3);
	FUN_004333d0(imageManager, 0x80f3, "report", 16);
	FUN_004333d0(imageManager, 0x80f3, "report", 32);
	imageManager->RemoveIcon(0x800f);
	FUN_004333d0(imageManager, 0x800f, "table", 16);
	FUN_004333d0(imageManager, 0x800f, "table", 32);
	imageManager->RemoveIcon(0x80e2);
	FUN_004333d0(imageManager, 0x80e2, "text_list_numbers", 16);
	FUN_004333d0(imageManager, 0x80e2, "text_list_numbers", 32);
	imageManager->RemoveIcon(0x80bf);
	FUN_004333d0(imageManager, 0x80bf, "world", 16);
	FUN_004333d0(imageManager, 0x80bf, "world", 32);
	imageManager->RemoveIcon(0x8101);
	FUN_004333d0(imageManager, 0x8101, "bug_error", 16);
	FUN_004333d0(imageManager, 0x8101, "bug_error", 32);
	imageManager->RemoveIcon(0x8100);
	FUN_004333d0(imageManager, 0x8100, "shield", 16);
	FUN_004333d0(imageManager, 0x8100, "shield", 32);
	imageManager->RemoveIcon(0x803a);
	FUN_004333d0(imageManager, 0x803a, "ungroup", 16);
	FUN_004333d0(imageManager, 0x803a, "ungroup", 32);
	imageManager->RemoveIcon(0x8039);
	FUN_004333d0(imageManager, 0x8039, "group", 16);
	FUN_004333d0(imageManager, 0x8039, "group", 32);
}

// FUNCTION: ROBLOX 0x00433b20
void CMainFrame::FUN_00433b20(CXTPPropExchange* pPX, BOOL p_unk0x08)
{
	CXTPCommandBars* commandBars = GetCommandBars();

	XTP_COMMANDBARS_PROPEXCHANGE_PARAM param;
	param.bSerializeControls = p_unk0x08;
	param.bLoadSilent = TRUE;

	CXTPPropExchangeSection commandBarsSection(pPX->GetSection("CommandBars"));
	commandBars->DoPropExchange(&commandBarsSection, &param);

	CXTPPropExchangeSection shortcuts(pPX->GetSection("Shortcuts"));
	commandBars->GetShortcutManager()->DoPropExchange(&shortcuts);

	CXTPPropExchangeSection options(pPX->GetSection("Options"));
	commandBars->GetCommandBarsOptions()->DoPropExchange(&options, TRUE);

	CXTPPropExchangeSection dockingPane(pPX->GetSection("DockingPane"));
	m_unk0x120.DoPropExchange(&dockingPane);
}

// FUNCTION: ROBLOX 0x00433c80
void CMainFrame::onEvent(const RBX::FillToolColor* source, RBX::BrickColor event)
{
	G3D::Color3uint8 color(event.color3());
	COLORREF rgb = RGB(color.r, color.g, color.b);

	CXTPControlPopupColor* control =
		static_cast<CXTPControlPopupColor*>(GetCommandBars()->FindControl(xtpControlPopup, 0x8040, TRUE, TRUE));

	if (control != NULL) {
		control->SetColor(rgb);
	}
}

// FUNCTION: ROBLOX 0x00433ce0
void CMainFrame::FUN_00433ce0()
{
	FUN_004325e0(0xbf, CRect(0, 0, 280, 400), xtpPaneDockLeft);
}

// FUNCTION: ROBLOX 0x00433d10
void CMainFrame::FUN_00433d10()
{
	FUN_004325e0(0xc0, CRect(0, 0, 640, 200), xtpPaneDockBottom);
}

// FUNCTION: ROBLOX 0x00433d50
void CMainFrame::FUN_00433d50()
{
	CXTPCommandBars* commandBars = GetCommandBars();

	if (commandBars != NULL) {
		CXTPCustomizeSheet sheet(commandBars);

		CXTPCustomizeOptionsPage options(&sheet);
		sheet.AddPage(&options);

		CXTPCustomizeCommandsPage* commands = sheet.GetCommandsPage();
		commands->AddCategories(129);
		commands->InsertAllCommandsCategory();
		commands->InsertBuiltInMenus(129);
		commands->InsertNewMenuCategory();

		sheet.DoModal();
	}
}

// FUNCTION: ROBLOX 0x00433e60
int CMainFrame::OnCreateCommandBar(LPCREATEBARSTRUCT lpCreatePopup)
{
	if (lpCreatePopup->bPopup && _tcscmp(lpCreatePopup->lpcstrCaption, "Camera Positioning") == 0) {
		CXTPPopupBar* popupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());
		popupBar->SetTearOffPopup(lpCreatePopup->lpcstrCaption, 1001, 400);
		lpCreatePopup->pCommandBar = popupBar;
		return TRUE;
	}

	return FALSE;
}

// FUNCTION: ROBLOX 0x00433ec0
BOOL CPatchedControlComboBox::Compare(CXTPControl* pOther)
{
	if (!CXTPControlComboBox::Compare(pOther)) {
		return FALSE;
	}

	return GetListBoxCtrl()->GetCount() == static_cast<CPatchedControlComboBox*>(pOther)->GetListBoxCtrl()->GetCount();
}

// FUNCTION: ROBLOX 0x00433fc0
int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->bToolBar) {
		CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreateControl->pCommandBar);
		if (pToolBar == NULL) {
			return FALSE;
		}

		if (lpCreateControl->nID == ID_EDIT_UNDO) {
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}

		if (lpCreateControl->nID == ID_EDIT_REDO) {
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}

		if (lpCreateControl->nID == 0x80db) {
			lpCreateControl->pControl = (CXTPControl*) CXTPControlLabel::CreateObject();
			lpCreateControl->pControl->SetFlags(lpCreateControl->pControl->GetFlags() & ~xtpFlagManualUpdate);
			lpCreateControl->pControl->SetStyle(xtpButtonCaption);
			return TRUE;
		}

		if (lpCreateControl->nID == 0x80f6) {
			lpCreateControl->pControl = (CXTPControl*) CXTPControlLabel::CreateObject();
			lpCreateControl->pControl->SetFlags(lpCreateControl->pControl->GetFlags() & ~xtpFlagManualUpdate);
			lpCreateControl->pControl->SetStyle(xtpButtonCaption);
			return TRUE;
		}

		if (lpCreateControl->nID == 0x80c3) {
			CPatchedControlComboBox* pComboBox = new CPatchedControlComboBox();
			pComboBox->SetDropDownListStyle();
			pComboBox->SetCaption("Command >");
			pComboBox->SetStyle(xtpButtonCaption);
			pComboBox->SetFlags(xtpFlagControlStretched);
			lpCreateControl->pControl = pComboBox;
			return TRUE;
		}

		if (lpCreateControl->nID == 0x8040) {
			CXTPControlPopupColor* pPopupColor = (CXTPControlPopupColor*) CXTPControlPopupColor::CreateObject();
			CXTPPopupToolBar* pColorBar = CXTPPopupToolBar::CreatePopupToolBar(GetCommandBars());
			pPopupColor->SetCommandBar(pColorBar);
			pColorBar->GetControls()->Add(new CRobloxControlColorSelector(), 0x8d);
			pColorBar->SetTearOffPopup("Color", 1005, 0);
			pColorBar->EnableCustomization();
			pColorBar->InternalRelease();
			lpCreateControl->pControl = pPopupColor;
			return TRUE;
		}
	}

	return FALSE;
}

// FUNCTION: ROBLOX 0x00434270
void CMainFrame::RestoreResolution()
{
	RBX::ScopedAssign<bool> restoring(m_unk0xe1, true);

	if (m_unk0xe4 == NULL) {
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_ERROR, "CMainFrame::RestoreResolution hm is NULL");
		return;
	}

	MONITORINFOEX info;
	info.cbSize = sizeof(info);

	if (GetMonitorInfo(m_unk0xe4, &info)) {
		ShowWindow(SW_HIDE);

		LONG result = ChangeDisplaySettingsEx(info.szDevice, NULL, NULL, 0, NULL);
		if (result != DISP_CHANGE_SUCCESSFUL) {
			RBX::StandardOut::singleton()->print(
				RBX::MESSAGE_ERROR, "CMainFrame::RestoreResolution ChangeDisplaySettings == %d", result
			);
		}
	}

	ModifyStyle(0, WS_CAPTION | WS_THICKFRAME);
}

// FUNCTION: ROBLOX 0x004343e0
void CMainFrame::getMonitorInfo()
{
	m_unk0xe4 = MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);

	if (m_unk0xe4 == NULL) {
		RBX::StandardOut::singleton()->print(
			RBX::MESSAGE_WARNING, "CMainFrameFrameWnd::getMonitorInfo MonitorFromWindow returned NULL"
		);
	}
}

// FUNCTION: ROBLOX 0x00434490
void CMainFrame::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	CXTPMDIFrameWnd::OnActivateApp(bActive, dwThreadID);

	if (m_unk0xe0 && !m_unk0xe1 && !bActive) {
		RestoreResolution();
		ShowWindow(SW_SHOWMINNOACTIVE);
	}
}

// FUNCTION: ROBLOX 0x00434970
bool CMainFrame::FUN_00434970(bool p_unk0x04)
{
	CXTPCommandBars* commandBars = GetCommandBars();

	CXTPCommandBar* menuBar = commandBars->SetMenu("Menu Bar", 128);
	if (menuBar == NULL) {
		return false;
	}

	menuBar->SetFlags(xtpFlagIgnoreSetMenuMessage | xtpFlagHideMDIButtons);

	CXTPToolBar* standard = commandBars->Add("Standard", xtpBarTop);
	if (standard == NULL || !standard->LoadToolBar(128, p_unk0x04)) {
		return false;
	}

	CXTPToolBar* mouse = commandBars->Add("Mouse", xtpBarTop);
	if (mouse == NULL || !mouse->LoadToolBar(0x87, p_unk0x04)) {
		return false;
	}

	CXTPToolBar* test = commandBars->Add("Test", xtpBarTop);
	if (test == NULL || !test->LoadToolBar(0x89, p_unk0x04)) {
		return false;
	}

	CXTPToolBar* camera = commandBars->Add("Camera", xtpBarTop);
	if (camera == NULL || !camera->LoadToolBar(0x91, p_unk0x04)) {
		return false;
	}

	CXTPToolBar* command = commandBars->Add("Command", xtpBarBottom);
	if (command == NULL || !command->LoadToolBar(0x96, p_unk0x04)) {
		return false;
	}

	command->EnableDocking(xtpFlagAlignTop | xtpFlagAlignBottom | xtpFlagFloating | xtpFlagHideWrap | xtpFlagStretched);
	command->SetVisible(FALSE);

	DockRightOf(test, standard);
	DockRightOf(mouse, test);

	commandBars->HideCommands(hiddenCommands, _countof(hiddenCommands));
	commandBars->GetCommandBarsOptions()->bAlwaysShowFullMenus = TRUE;
	CXTPCommandBar::s_nExpandDelay = 4500;
	commandBars->GetCommandBarsOptions()->bToolBarAccelTips = TRUE;
	return true;
}

// FUNCTION: ROBLOX 0x00434b80
void CMainFrame::FUN_00434b80()
{
	if (!m_unk0xec) {
		CXTPPropExchangeXMLNode px(FALSE, 0, "IDE");

		if (px.OnBeforeExchange()) {
			FUN_00433b20(&px, TRUE);
			px.SaveToFile(m_unk0xf0);
		}

		m_unk0x3c8.ShowWindow(SW_HIDE);
		m_unk0x26c.ShowWorkspace(FALSE);
		GetCommandBars()->RemoveAll();
		m_unk0x120.DestroyAll();
		RecalcLayout();
		m_unk0xec = true;
	}
}

// FUNCTION: ROBLOX 0x00434c80
void CMainFrame::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
{
	GetCommandBars()->OnSetPreviewMode(bPreview);

	BOOL showWorkspace = FALSE;
	if (!bPreview) {
		showWorkspace = m_unk0xec == false;
	}

	m_unk0x26c.ShowWorkspace(showWorkspace);
	m_unk0x120.OnSetPreviewMode(bPreview);
	CXTPMDIFrameWnd::OnSetPreviewMode(bPreview, pState);
}

// FUNCTION: ROBLOX 0x00434ce0
void CMainFrame::OnClose()
{
	if (!m_unk0xec) {
		CXTPPropExchangeXMLNode px(FALSE, 0, "IDE");

		if (px.OnBeforeExchange()) {
			FUN_00433b20(&px, TRUE);
			px.SaveToFile(m_unk0xf0);
		}
	}

	CXTPMDIFrameWnd::OnClose();
}

// FUNCTION: ROBLOX 0x00434d90
void CMainFrame::FUN_00434d90(CCmdUI* pCmdUI)
{
	if (pCmdUI->m_pOther) {
		CRobloxControlColorSelector* control = static_cast<CRobloxControlColorSelector*>(
			static_cast<CXTPCommandBar*>(pCmdUI->m_pOther)->GetControls()->GetAt(pCmdUI->m_nIndex)
		);
		G3D::Color3uint8 color(RBX::FillTool::color.get().color3());
		control->SetColor(RGB(color.r, color.g, color.b));
	}

	pCmdUI->Enable(TRUE);
}

// FUNCTION: ROBLOX 0x00434e20
void CMainFrame::OnDestroy()
{
	AfxGetApp()->WriteProfileInt("Settings", "fullscreen", m_unk0xe0 != false);

	if (m_unk0xe0) {
		RestoreResolution();
	}
	else {
		m_unk0xf4.SaveWindowPos(this, "CMainFrameV2");
	}

	if (m_unk0xec) {
		FUN_00434970(false);
	}

	CXTPMDIFrameWnd::OnDestroy();
	m_unk0x26c.Detach();
}

// FUNCTION: ROBLOX 0x00434ea0
void CMainFrame::FUN_00434ea0()
{
	if (!g_unk0x0095391d) {
		m_unk0xed = true;
		FUN_00434b80();
	}
}

// STUB: ROBLOX 0x00434ec0
DEVMODE FUN_00434ec0(LPCTSTR device, int pixels)
{
	DEVMODE mode;
	memset(&mode, 0, sizeof(mode));
	mode.dmSize = sizeof(mode);

	if (!EnumDisplaySettingsEx(device, ENUM_CURRENT_SETTINGS, &mode, 0)) {
		RBX::StandardOut::singleton()->print(
			RBX::MESSAGE_ERROR, "EnumDisplaySettings failed. GetLastError() == %d", GetLastError()
		);
	}

	double aspect = (double) mode.dmPelsWidth / (double) mode.dmPelsHeight;
	DEVMODE best = mode;

	DWORD i = 0;
	while (EnumDisplaySettingsEx(device, i++, &mode, 0)) {
		double ratio = (double) mode.dmPelsWidth / (double) mode.dmPelsHeight;
		if (fabs(aspect - ratio) / aspect <= 0.1 && best.dmBitsPerPel == mode.dmBitsPerPel &&
			best.dmDisplayFrequency == mode.dmDisplayFrequency &&
			abs(pixels - (int) (mode.dmPelsWidth * mode.dmPelsHeight)) <
				abs(pixels - (int) (best.dmPelsWidth * best.dmPelsHeight))) {
			best = mode;
		}
	}

	best.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	return best;
}

// FUNCTION: ROBLOX 0x00435230
void CMainFrame::FUN_00435230(bool p_unk0x04)
{
	if (m_unk0xec && !m_unk0xed) {
		m_unk0x3c8.ShowWindow(SW_SHOW);
		FUN_00434970(p_unk0x04);
		m_unk0x26c.ShowWorkspace(TRUE);

		CXTPPropExchangeXMLNode px(TRUE, 0, "IDE");

		if (px.LoadFromFile(m_unk0xf0)) {
			FUN_00433b20(&px, TRUE);
		}
		else {
			FUN_00432650();
			FUN_00432680();
		}

		m_unk0xec = false;
	}
}

// FUNCTION: ROBLOX 0x00435460
void CMainFrame::FUN_00435460(NMHDR* pNMHDR, LRESULT* pResult)
{
	G3D::Color3uint8 color(RBX::FillTool::color.get().color3());
	COLORREF rgb = RGB(color.r, color.g, color.b);

	CXTColorDialog dialog(rgb, rgb, CPS_XT_SHOW3DSELECTION, this);

	if (dialog.DoModal() == IDOK) {
		COLORREF selected = dialog.GetColor();
		RBX::FillTool::color.set(
			RBX::BrickColor::closest(G3D::Color3uint8(GetRValue(selected), GetGValue(selected), GetBValue(selected)))
		);
	}
}

// STUB: ROBLOX 0x00435570
void CMainFrame::FUN_00435570(NMHDR* pNMHDR, LRESULT* pResult)
{
	STUB(0x435570);
}

// FUNCTION: ROBLOX 0x004357d0
void CMainFrame::FUN_004357d0()
{
	if (m_unk0xec) {
		FUN_00435230(false);
	}
	else {
		FUN_00434b80();
	}
}

// STUB: ROBLOX 0x004357f0
LRESULT CMainFrame::FUN_004357f0(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW) {
		CXTPDockingPane* pane = (CXTPDockingPane*) lParam;
		if (!pane->IsValid()) {
			switch (pane->GetID()) {
			case 0xc1:
				if (!m_unk0x390 || !m_unk0x390->m_hWnd) {
					m_unk0x390.reset(new CXTSplitterWnd);
					if (!m_unk0x390->CreateStatic(this, 2, 1)) {
						return FALSE;
					}

					m_unk0x398.reset(new CSelectionCaption);
					CRect captionRect(0, 0, 0, 0);
					if (!m_unk0x398->Create(
							m_unk0x390.get(), "", 2, 0x10000201, captionRect, m_unk0x390->IdFromRowCol(0, 0)
						)) {
						return -1;
					}

					m_unk0x394.reset(new CDataModelPropGrid);
					CRect gridRect(0, 0, 0, 0);
					m_unk0x394->Create(gridRect, m_unk0x390.get(), m_unk0x390->IdFromRowCol(1, 0), 0);
					m_unk0x394->ShowToolBar(TRUE);
					m_unk0x394->SetTheme(xtpGridThemeWhidbey);
					m_unk0x394->FUN_004204e0();

					m_unk0x390->SetRowInfo(0, 28, 0);
					m_unk0x390->SetSplitterStyle(XT_SPLIT_NOSIZE);
				}
				pane->Attach(m_unk0x390.get());
				break;
			case 0xc2: {
				CWnd* view = pane->AttachView(this, RUNTIME_CLASS(CWrapperView));
				if (view && ::IsWindow(view->m_hWnd)) {
					view->SendMessage(WM_INITIALUPDATE);
				}
				return TRUE;
			}
			case 0xc0: {
				CWnd* view = pane->AttachView(this, RUNTIME_CLASS(CStandardOutputView));
				if (view && ::IsWindow(view->m_hWnd)) {
					view->SendMessage(WM_INITIALUPDATE);
				}
				return TRUE;
			}
			case 0x8c: {
				CWnd* view = pane->AttachView(this, RUNTIME_CLASS(CRobloxReportPaneView));
				if (view && ::IsWindow(view->m_hWnd)) {
					view->SendMessage(WM_INITIALUPDATE);
				}
				return TRUE;
			}
			case 0xbf: {
				CWnd* view = pane->AttachView(this, RUNTIME_CLASS(CWebToolbox));
				if (view && ::IsWindow(view->m_hWnd)) {
					view->SendMessage(WM_INITIALUPDATE);
				}
				return TRUE;
			}
			default:
				return FALSE;
			}
		}
		return TRUE;
	}
	return FALSE;
}

// FUNCTION: ROBLOX 0x00435b60
void CMainFrame::ChangeResolution()
{
	if (m_unk0xe4 == NULL) {
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_ERROR, "CMainFrame::ChangeResolution hm is NULL");
		return;
	}

	RBX::ScopedAssign<bool> changing(m_unk0xe1, true);

	MONITORINFOEX info;
	info.cbSize = sizeof(info);

	if (!GetMonitorInfo(m_unk0xe4, &info)) {
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_ERROR, "GetMonitorInfo failed");
		return;
	}

	G3D::Vector2int16 size = CRenderSettings::singleton().FUN_00448300();
	DEVMODE mode = FUN_00434ec0(info.szDevice, size.x * size.y);

	LONG result = ChangeDisplaySettingsEx(info.szDevice, &mode, NULL, CDS_FULLSCREEN, NULL);
	if (result == DISP_CHANGE_SUCCESSFUL) {
		ModifyStyle(WS_CAPTION | WS_THICKFRAME, 0);

		CRect rect = XTPMultiMonitor()->GetScreenArea(this);
		int cx = GetSystemMetrics(SM_CXBORDER);
		int cy = GetSystemMetrics(SM_CYBORDER);
		SetWindowPos(NULL, rect.left - cx, rect.top - cy, rect.Width() + 2 * cx, rect.Height() + 2 * cy, SWP_NOZORDER);
	}
	else {
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_ERROR, "ChangeDisplaySettings == %d", result);
	}
}

// FUNCTION: ROBLOX 0x00435e00
void CMainFrame::FUN_00435e00(bool p_unk0x04)
{
	if (m_unk0xe0 == p_unk0x04) {
		return;
	}

	m_unk0xe0 = p_unk0x04;

	if (p_unk0x04) {
		GetWindowRect(&m_unk0xf4.rcNormalPosition);
		CRect rect(m_unk0xf4.rcNormalPosition);
		ChangeResolution();
	}
	else {
		RestoreResolution();

		CRect rect(m_unk0xf4.rcNormalPosition);
		if (!rect.IsRectEmpty()) {
			SetWindowPos(
				NULL,
				rect.left,
				rect.top,
				rect.Width(),
				rect.Height(),
				SWP_NOOWNERZORDER | SWP_SHOWWINDOW | SWP_FRAMECHANGED
			);
		}
	}

	RecalcLayout();
}

// FUNCTION: ROBLOX 0x00435eb0
void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CXTPMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	if (m_unk0xe0 && !m_unk0xe1 && nState != WA_INACTIVE) {
		ChangeResolution();
		ShowWindow(SW_RESTORE);
	}
}

// FUNCTION: ROBLOX 0x00435f00
void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CXTPMDIFrameWnd::OnGetMinMaxInfo(lpMMI);

	if (m_unk0xe0) {
		lpMMI->ptMaxPosition.x = 0;
		lpMMI->ptMaxPosition.y = 0;
	}

	if (!m_unk0xe1 && !m_unk0xe0 && m_unk0xec) {
		G3D::Vector2int16 size = CRenderSettings::singleton().FUN_00448300();
		CRect rect(0, 0, size.x, size.y);
		CalcWindowRect(&rect);
		lpMMI->ptMaxSize.x = lpMMI->ptMaxTrackSize.x = rect.Width();
		lpMMI->ptMaxSize.y = lpMMI->ptMaxTrackSize.y = rect.Height();
	}
}

// FUNCTION: ROBLOX 0x00435fa0
void CMainFrame::FUN_00435fa0()
{
	FUN_00435e00(!m_unk0xe0);
}

// FUNCTION: ROBLOX 0x00435fc0
CMainFrame::~CMainFrame()
{
	CMDTARGET_RELEASE(m_unk0xe8);
	RBX::FillTool::color.removeListener(this);
}

// STUB: ROBLOX 0x00436170
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPMDIFrameWnd::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	if (!m_unk0x3c8.Create(this) || !m_unk0x3c8.SetIndicators(indicators, _countof(indicators))) {
		return 0;
	}

	if (!InitCommandBars(RUNTIME_CLASS(MyXTPCommandBars))) {
		return -1;
	}

	m_unk0x39c.SetTheme(xtThemeOffice2003);
	CXTPPaintManager::SetTheme(xtpThemeWhidbey);

	m_unk0x120.InstallDockingPanes(this);
	m_unk0x120.SetTheme(xtpPaneThemeVisualStudio2005);

	m_unk0x26c.Attach(this, FALSE);
	m_unk0x26c.GetPaintManager()->m_bShowIcons = FALSE;
	m_unk0x26c.EnableToolTips(xtpTabToolTipAlways);

	CXTPCommandBars* commandBars = GetCommandBars();

	if (commandBars != NULL) {
		commandBars->GetImageManager()->SetIcons(0x91);
		m_unk0x120.SetAlphaDockingContext(TRUE);
		m_unk0x120.SetShowDockingContextStickers(TRUE);
		commandBars->GetCommandBarsOptions()->ShowKeyboardCues(xtpKeyboardCuesShowWindowsDefault);
		commandBars->GetToolTipContext()->SetStyle(xtpToolTipOffice);

		XTPImageManager()->InternalAddRef();
		commandBars->SetImageManager(XTPImageManager());
		XTPImageManager()->SetIcons(0x91);

		m_unk0xec = true;
		FUN_00435230(true);
		FUN_004335e0(XTPImageManager());
		getMonitorInfo();

		if (AfxGetApp()->GetProfileInt("Settings", "fullscreen", 0) == 1) {
			FUN_00435e00(true);
		}

		PostMessage(WM_COMMAND, 0x8101);
	}

	return 0;
}

// clang-format off
BEGIN_MESSAGE_MAP(CMainFrame, CXTPMDIFrameWnd)
	ON_WM_ACTIVATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_ACTIVATEAPP()
	ON_WM_CREATE()
	ON_COMMAND(0x88b9, FUN_00433d50)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, FUN_004357f0)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_COMMAND(0x809f, FUN_00433d10)
	ON_UPDATE_COMMAND_UI(0x809f, FUN_00453eb0)
	ON_COMMAND(0x8042, FUN_00433ce0)
	ON_UPDATE_COMMAND_UI(0x8042, FUN_00453eb0)
	ON_COMMAND(0x8018, FUN_00432650)
	ON_UPDATE_COMMAND_UI(0x8018, FUN_00453eb0)
	ON_COMMAND(0x800f, FUN_00432680)
	ON_UPDATE_COMMAND_UI(0x800f, FUN_00453eb0)
	ON_XTP_CREATECOMMANDBAR()
	ON_XTP_CREATECONTROL()
	ON_XTP_EXECUTE(0x8d, FUN_00435570)
	ON_UPDATE_COMMAND_UI(0x8d, FUN_00434d90)
	ON_XTP_EXECUTE(0x26dd, FUN_00435460)
	ON_UPDATE_COMMAND_UI(0x26dd, FUN_00453eb0)
	ON_WM_DESTROY()
	ON_COMMAND(0x80bf, FUN_00432980)
	ON_UPDATE_COMMAND_UI(0x80bf, FUN_00453eb0)
	ON_UPDATE_COMMAND_UI(0xbb, FUN_00432990)
	ON_UPDATE_COMMAND_UI(0xbe, FUN_00432990)
	ON_UPDATE_COMMAND_UI(0xbd, FUN_004329b0)
	ON_UPDATE_COMMAND_UI(0xbc, FUN_00432a80)
	ON_COMMAND(0x80ff, FUN_00434ea0)
	ON_COMMAND(0x80dd, FUN_00432b50)
	ON_UPDATE_COMMAND_UI(0x80dd, FUN_00453eb0)
	ON_COMMAND(0x80f3, FUN_00432b60)
	ON_UPDATE_COMMAND_UI(0x80f3, FUN_00453eb0)
	ON_UPDATE_COMMAND_UI(0x80f5, FUN_00432b90)
	ON_COMMAND(0x80f5, FUN_004357d0)
	ON_COMMAND(0x80f4, FUN_00435fa0)
	ON_UPDATE_COMMAND_UI(0x80f4, FUN_00432bc0)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
// clang-format on

// FUNCTION: ROBLOX 0x00436350
CMainFrame::CMainFrame()
	: m_unk0xe0(false), m_unk0xe1(false), m_unk0xe4(NULL), m_unk0xe8(NULL), m_unk0xec(false), m_unk0xed(false)
{
	{
		DWORD value = 0;
		ATL::CRegKey key;

		if (key.Open(HKEY_CURRENT_USER, "Software\\ROBLOX Corporation\\Roblox", KEY_READ) == ERROR_SUCCESS) {
			key.QueryDWORDValue("OverrideLockViewGamelayout", value);
		}

		g_unk0x0095391d = value == 1;
		RBX::Accoutrement::writeUnlocked = g_unk0x0095391d;
	}

	std::string directory = RBX::FileSystem::getUserAppDataDirectory(true, NULL);

	if (directory.size() > 0) {
		m_unk0xf0.Format("%s\\IDESettings.xml", directory.c_str());
	}

	RBX::FillTool::color.addListener(this);

	XTPImageManager()->InternalAddRef();
	m_unk0x120.SetImageManager(XTPImageManager());
}

void CMainFrame::FUN_00453eb0(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

// FUNCTION: ROBLOX 0x00465830 FOLDED
void CMainFrame::OnPaint()
{
	Default();
}
