#include "Roblox.h"

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
