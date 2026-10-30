#include "Roblox.h"

#include "BrowserView.h"
#include "ChildFrm.h"
#include "MainFrm.h"
#include "NullDoc.h"
#include "ObjectBrowser.h"
#include "RobloxDoc.h"
#include "RobloxReportView.h"
#include "RobloxView.h"
#include "ScriptDoc.h"
#include "VersionInfo.h"
#include "network/API.h"
#include "security/SecurityContext.h"
#include "util/StandardOut.h"
#include "v8datamodel/factoryregistration.h"

#include <afxpriv.h>
#include <atlcom.h>

class CRobloxModule : public ATL::CAtlMfcModule
{
public:
	DECLARE_REGISTRY_APPID_RESOURCEID(103, "{A40F941A-3670-40DA-85B9-39A7F53511B9}");
};

CRobloxModule _AtlModule;

// SIZE 0x90
class CRbxDocTemplate : public CMultiDocTemplate
{
public:
	CRbxDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
		: CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
	{
	}

	virtual CFrameWnd* CreateNewFrame(CDocument* pDoc, CFrameWnd* pOther); // vtable+0x78
};

DECOMP_SIZE_ASSERT(CRbxDocTemplate, 0x90)

// SIZE 0x98
class CRbxPlayDocTemplate : public CMultiDocTemplate
{
public:
	CRbxPlayDocTemplate(
		UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass
	)
		: CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass), m_unk0x90(NULL)
	{
	}

	virtual void RemoveDocument(CDocument* pDoc);                          // vtable+0x68
	virtual CFrameWnd* CreateNewFrame(CDocument* pDoc, CFrameWnd* pOther); // vtable+0x78

private:
	CDocument* m_unk0x90; // 0x90
	CString m_unk0x94;    // 0x94
};

DECOMP_SIZE_ASSERT(CRbxPlayDocTemplate, 0x98)

// SIZE 0x30
class CIDEDocManager : public CDocManager
{
public:
	CIDEDocManager(bool p_unk0x04);

	virtual BOOL DoPromptFileName(
		CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate
	);                        // vtable+0x34
	virtual void OnFileNew(); // vtable+0x3c

	CRbxDocTemplate* m_unk0x20;     // 0x20
	CRbxPlayDocTemplate* m_unk0x24; // 0x24
	CRbxDocTemplate* m_unk0x28;     // 0x28
	CRbxDocTemplate* m_unk0x2c;     // 0x2c
};

DECOMP_SIZE_ASSERT(CIDEDocManager, 0x30)

// clang-format off
BEGIN_MESSAGE_MAP(CRobloxApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_HELP, FUN_0044dc90)
	ON_UPDATE_COMMAND_UI(ID_HELP, FUN_00453eb0)
	ON_COMMAND(0x80de, FUN_0044e270)
	ON_UPDATE_COMMAND_UI(0x80de, FUN_00453eb0)
	ON_THREAD_MESSAGE(WM_USER + 2, FUN_0044d450)
	ON_COMMAND(0x80f1, FUN_0044e290)
	ON_UPDATE_COMMAND_UI(0x80f1, FUN_004510b0)
	ON_COMMAND(0x8101, FUN_00451100)
END_MESSAGE_MAP()
// clang-format on

// STUB: ROBLOX 0x0044d450
void CRobloxApp::FUN_0044d450(WPARAM p_unk0x04, LPARAM p_unk0x08)
{
	STUB(0x44d450);
}

// FUNCTION: ROBLOX 0x0044d4a0
static void FUN_0044d4a0()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = AfxGetInstanceHandle();
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = "Menu";
	wc.lpszClassName = "ROBLOX-ATOM";
	RegisterClass(&wc);

	CFrameWnd* frame = static_cast<CFrameWnd*>(RUNTIME_CLASS(CFrameWnd)->CreateObject());
	frame->Create(wc.lpszClassName, "ROBLOX");
}

// FUNCTION: ROBLOX 0x0044d5f0
void CRbxPlayDocTemplate::RemoveDocument(CDocument* pDoc)
{
	CMultiDocTemplate::RemoveDocument(pDoc);

	if (pDoc == m_unk0x90) {
		m_unk0x90 = NULL;
	}
}

// FUNCTION: ROBLOX 0x0044d7d0
CIDEDocManager::CIDEDocManager(bool p_unk0x04)
{
	m_unk0x24 = new CRbxPlayDocTemplate(
		129,
		RUNTIME_CLASS(CRobloxDoc),
		RUNTIME_CLASS(CRbxChildFrame),
		p_unk0x04 ? RUNTIME_CLASS(CRobloxReportDocView) : RUNTIME_CLASS(CRobloxView)
	);
	AddDocTemplate(m_unk0x24);

	m_unk0x20 = new CRbxDocTemplate(
		185, RUNTIME_CLASS(CBrowserDoc), RUNTIME_CLASS(CChildFrame), RUNTIME_CLASS(CIDEBrowserView)
	);
	AddDocTemplate(m_unk0x20);

	m_unk0x28 =
		new CRbxDocTemplate(207, RUNTIME_CLASS(CScriptDoc), RUNTIME_CLASS(CMDIChildWnd), RUNTIME_CLASS(CScriptEditor));
	AddDocTemplate(m_unk0x28);

	m_unk0x2c =
		new CRbxDocTemplate(138, RUNTIME_CLASS(CNullDoc), RUNTIME_CLASS(CMDIChildWnd), RUNTIME_CLASS(CObjectBrowser));
	AddDocTemplate(m_unk0x2c);
}

// FUNCTION: ROBLOX 0x0044d9b0
BOOL CIDEDocManager::DoPromptFileName(
	CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate
)
{
	if (pTemplate == NULL) {
		pTemplate = m_unk0x24;
	}

	return CDocManager::DoPromptFileName(fileName, nIDSTitle, lFlags, bOpenFileDialog, pTemplate);
}

// STUB: ROBLOX 0x0044dac0
BOOL CRobloxApp::FUN_0044dac0(bool p_unk0x04)
{
	if (m_pMainWnd != NULL) {
		if (!m_pMainWnd->IsWindowVisible()) {
			static_cast<CMainFrame*>(m_pMainWnd)->FUN_00432550(m_nCmdShow);
		}

		return m_pMainWnd->m_hWnd != NULL;
	}

	CMainFrame* frame = new CMainFrame;

	if (frame == NULL || !frame->LoadFrame(128)) {
		return FALSE;
	}

	if (p_unk0x04) {
		frame->FUN_00434b80();
	}
	else {
		frame->FUN_00435230(false);
	}

	frame->DragAcceptFiles();
	frame->FUN_00432550(m_nCmdShow);
	m_pMainWnd = frame;

	if (m_atomApp == 0 && m_atomSystemTopic == 0) {
		EnableShellOpen();
	}

	return TRUE;
}

// STUB: ROBLOX 0x0044dc00
void CRobloxApp::OnAppAbout()
{
	STUB(0x44dc00);
}

// STUB: ROBLOX 0x0044dc90
void CRobloxApp::FUN_0044dc90()
{
	STUB(0x44dc90);
}

// FUNCTION: ROBLOX 0x0044e100
CFrameWnd* CRbxDocTemplate::CreateNewFrame(CDocument* pDoc, CFrameWnd* pOther)
{
	theApp.FUN_0044dac0(true);
	return CMultiDocTemplate::CreateNewFrame(pDoc, pOther);
}

// FUNCTION: ROBLOX 0x0044e100 FOLDED
CFrameWnd* CRbxPlayDocTemplate::CreateNewFrame(CDocument* pDoc, CFrameWnd* pOther)
{
	theApp.FUN_0044dac0(true);
	return CMultiDocTemplate::CreateNewFrame(pDoc, pOther);
}

// FUNCTION: ROBLOX 0x0044e250
void CRobloxApp::FUN_0044e250()
{
	static_cast<CIDEDocManager*>(m_pDocManager)->m_unk0x20->OpenDocumentFile(NULL);
}

// STUB: ROBLOX 0x0044e270
void CRobloxApp::FUN_0044e270()
{
	STUB(0x44e270);
}

// STUB: ROBLOX 0x0044e290
void CRobloxApp::FUN_0044e290()
{
	STUB(0x44e290);
}

// STUB: ROBLOX 0x0044ed40
void CIDEDocManager::OnFileNew()
{
	STUB(0x44ed40);
	m_unk0x24->OpenDocumentFile(NULL);
}

// FUNCTION: ROBLOX 0x0044eee0
int CRobloxApp::ExitInstance()
{
	RBX::StandardOut::singleton()->print(RBX::MESSAGE_INFO, "ROBLOX ExitInstance");
	_AtlModule.RevokeClassObjects();
	return CWinApp::ExitInstance();
}

// FUNCTION: ROBLOX 0x004508c0
CRobloxApp::CRobloxApp() : m_unk0xa4("Roblox")
{
}

// FUNCTION: ROBLOX 0x00450940
CRobloxApp::~CRobloxApp()
{
}

// STUB: ROBLOX 0x004510b0
void CRobloxApp::FUN_004510b0(CCmdUI* p_unk0x04)
{
	STUB(0x4510b0);
	p_unk0x04->Enable(TRUE);
}

// STUB: ROBLOX 0x00451100
void CRobloxApp::FUN_00451100()
{
	STUB(0x451100);
}

// STUB: ROBLOX 0x004521d0
BOOL CRobloxApp::InitInstance()
{
	RBX::StandardOut::singleton()->print(RBX::MESSAGE_INFO, "ROBLOX InitInstance");

	m_unk0x10c.reset(new ErrorUploader());

	InitCommonControls();

	if (!CWinApp::InitInstance()) {
		return FALSE;
	}

	{
		CVersionInfo versionInfo;
		CString fileName;
		AfxGetModuleShortFileName(m_hInstance, fileName);
		versionInfo.Load(fileName);
		m_unk0xf0 = versionInfo.GetFileVersionAsString();
	}

	if (!AfxOleInit()) {
		AfxMessageBox(100);
		return FALSE;
	}

	FUN_0044d4a0();

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister) {
		_AtlModule.UpdateRegistryAppId(FALSE);
		_AtlModule.UnregisterServer(TRUE);
		return FALSE;
	}

	if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppRegister) {
		_AtlModule.UpdateRegistryAppId(TRUE);
		_AtlModule.RegisterServer(TRUE);
		return FALSE;
	}

	static RBX::FactoryRegistrator registrator;
	RBX::Network::API::init(m_unk0xf0.c_str());
	AfxEnableControlContainer();

	m_pDocManager = new CIDEDocManager(false);

	if (!FUN_0044dac0(false)) {
		return FALSE;
	}

	return ProcessShellCommand(cmdInfo);
}

// FUNCTION: ROBLOX 0x00452960
int CRobloxApp::Run()
{
	RBX::Security::Impersonator impersonate(RBX::Security::LocalGUI);
	return CWinApp::Run();
}

// FUNCTION: ROBLOX 0x00453eb0 FOLDED
void CRobloxApp::FUN_00453eb0(CCmdUI* p_unk0x04)
{
	p_unk0x04->Enable(TRUE);
}

CRobloxApp theApp;
