#include "MainFrm.h"

#include "Roblox.h"
#include "util/FileSystem.h"
#include "util/StandardOut.h"
#include "util/scopedassign.h"
#include "v8datamodel/Accoutrement.h"
#include "v8datamodel/ToolsPart.h"

#include <G3D/Color3uint8.h>

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

// STUB: ROBLOX 0x00432760
CWrapperView::CWrapperView()
{
	STUB(0x432760);
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

// STUB: ROBLOX 0x004327f0
BOOL CWrapperView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	STUB(0x4327f0);
	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// STUB: ROBLOX 0x00432840
BOOL CWrapperView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	STUB(0x432840);
	return CView::OnWndMsg(message, wParam, lParam, pResult);
}

// STUB: ROBLOX 0x00432910
CWrapperView::~CWrapperView()
{
	STUB(0x432910);
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

// STUB: ROBLOX 0x004329b0
void CMainFrame::FUN_004329b0(CCmdUI* pCmdUI)
{
	STUB(0x4329b0);
	pCmdUI->SetText("");
}

// STUB: ROBLOX 0x00432a80
void CMainFrame::FUN_00432a80(CCmdUI* pCmdUI)
{
	STUB(0x432a80);
	pCmdUI->SetText("");
}

// STUB: ROBLOX 0x00432b50
void CMainFrame::FUN_00432b50()
{
	STUB(0x432b50);
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

// STUB: ROBLOX 0x004335e0
static void FUN_004335e0(CXTPImageManager* imageManager)
{
	STUB(0x4335e0);
}

// STUB: ROBLOX 0x00433b20
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

// STUB: ROBLOX 0x00433fc0
int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	STUB(0x433fc0);
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

// STUB: ROBLOX 0x00434b80
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

// STUB: ROBLOX 0x00434ce0
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

// STUB: ROBLOX 0x00434d90
void CMainFrame::FUN_00434d90(CCmdUI* pCmdUI)
{
	STUB(0x434d90);
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

// STUB: ROBLOX 0x00435230
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
	STUB(0x4357f0);
	return FALSE;
}

// STUB: ROBLOX 0x00435b60
void CMainFrame::FUN_00435b60()
{
	STUB(0x435b60);
}

// STUB: ROBLOX 0x00435e00
void CMainFrame::FUN_00435e00(bool p_unk0x04)
{
	STUB(0x435e00);
}

// STUB: ROBLOX 0x00435eb0
void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CXTPMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	if (m_unk0xe0 && !m_unk0xe1 && nState != WA_INACTIVE) {
		FUN_00435b60();
		ShowWindow(SW_RESTORE);
	}
}

// STUB: ROBLOX 0x00435f00
void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	STUB(0x435f00);
	CXTPMDIFrameWnd::OnGetMinMaxInfo(lpMMI);
}

// STUB: ROBLOX 0x00435fa0
void CMainFrame::FUN_00435fa0()
{
	FUN_00435e00(!m_unk0xe0);
}

// STUB: ROBLOX 0x00435fc0
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

// STUB: ROBLOX 0x00436350
CMainFrame::CMainFrame()
{
	m_unk0xe0 = false;
	m_unk0xe1 = false;
	m_unk0xe4 = NULL;
	m_unk0xe8 = NULL;
	m_unk0xec = false;
	m_unk0xed = false;

	DWORD value = 0;
	ATL::CRegKey key;

	if (key.Open(HKEY_CURRENT_USER, "Software\\ROBLOX Corporation\\Roblox", KEY_READ) == ERROR_SUCCESS) {
		key.QueryDWORDValue("OverrideLockViewGamelayout", value);
	}

	g_unk0x0095391d = value == 1;
	RBX::Accoutrement::writeUnlocked = g_unk0x0095391d;

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

void CMainFrame::OnPaint()
{
	CPaintDC dc(this);
}
