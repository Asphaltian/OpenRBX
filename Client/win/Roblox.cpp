#include "Roblox.h"

#include "AboutRobloxDialog.h"
#include "App.h"
#include "BrowserView.h"
#include "ChildFrm.h"
#include "DHTMLWindow.h"
#include "InstanceExplorer.h"
#include "LogManager.h"
#include "MainFrm.h"
#include "NullDoc.h"
#include "ObjectBrowser.h"
#include "ReflectionMetadata.h"
#include "RenderSettings.h"
#include "RobloxDoc.h"
#include "RobloxReportView.h"
#include "RobloxView.h"
#include "ScriptDoc.h"
#include "VersionInfo.h"
#include "network/API.h"
#include "script/Script.h"
#include "security/SecurityContext.h"
#include "util/ContentProvider.h"
#include "util/Http.h"
#include "util/Profiling.h"
#include "util/StandardOut.h"
#include "v8datamodel/DebugSettings.h"
#include "v8datamodel/GameSettings.h"
#include "v8datamodel/GlobalSettings.h"
#include "v8datamodel/factoryregistration.h"

#include <afxpriv.h>
#include <algorithm>
#include <atlcom.h>
#include <boost/bind.hpp>
#include <exdispid.h>
#include <functional>
#include <shlwapi.h>
#include <sstream>
#include <stdexcept>

class CRobloxModule : public ATL::CAtlMfcModule
{
public:
	DECLARE_REGISTRY_APPID_RESOURCEID(103, "{A40F941A-3670-40DA-85B9-39A7F53511B9}");
};

CRobloxModule _AtlModule;

// GLOBAL: ROBLOX 0x00954290
CString g_unk0x00954290;

// GLOBAL: ROBLOX 0x00954294
static boost::scoped_ptr<std::runtime_error> g_unk0x00954294;

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

	CDocument* FUN_0044d620(LPCTSTR lpszPathName, BOOL bMakeVisible);
	CDocument* FUN_0044f970();

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

// SIZE 0x24
class CBrowserDocManager : public CDocManager
{
public:
	CBrowserDocManager();

	CSingleDocTemplate* m_unk0x20; // 0x20
};

DECOMP_SIZE_ASSERT(CBrowserDocManager, 0x24)

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

// FUNCTION: ROBLOX 0x0044d450
void CRobloxApp::FUN_0044d450(WPARAM p_unk0x04, LPARAM p_unk0x08)
{
	if (AfxGetMainWnd()) {
		AfxGetMainWnd()->FlashWindow(FALSE);
		AfxMessageBox(196);
	}

	AfxPostQuitMessage(0);
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

// FUNCTION: ROBLOX 0x0044d550
CBrowserDocManager::CBrowserDocManager()
{
	m_unk0x20 = new CSingleDocTemplate(
		185, RUNTIME_CLASS(CBrowserDoc), RUNTIME_CLASS(CBrowserFrameWnd), RUNTIME_CLASS(CPlayBrowserView)
	);
	AddDocTemplate(m_unk0x20);
}

// FUNCTION: ROBLOX 0x0044d5f0
void CRbxPlayDocTemplate::RemoveDocument(CDocument* pDoc)
{
	CMultiDocTemplate::RemoveDocument(pDoc);

	if (pDoc == m_unk0x90) {
		m_unk0x90 = NULL;
	}
}

// FUNCTION: ROBLOX 0x0044d620
CDocument* CRbxPlayDocTemplate::FUN_0044d620(LPCTSTR lpszPathName, BOOL bMakeVisible)
{
	m_unk0x94 = "";

	CDocument* pDocument = CreateNewDocument();
	if (pDocument == NULL) {
		m_unk0x94 = "CDocTemplate::CreateNewDocument returned NULL";
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return NULL;
	}

	BOOL bAutoDelete = pDocument->m_bAutoDelete;
	pDocument->m_bAutoDelete = FALSE;
	CFrameWnd* pFrame = CreateNewFrame(pDocument, NULL);
	pDocument->m_bAutoDelete = bAutoDelete;
	if (pFrame == NULL) {
		m_unk0x94 = "CreateNewFrame returned NULL";
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		delete pDocument;
		return NULL;
	}

	if (lpszPathName == NULL) {
		SetDefaultTitle(pDocument);

		if (!bMakeVisible) {
			pDocument->m_bEmbedded = TRUE;
		}

		if (!pDocument->OnNewDocument()) {
			m_unk0x94 = "CDocument::OnNewDocument returned FALSE";
			pFrame->DestroyWindow();
			return NULL;
		}

		m_nUntitledCount++;
	}
	else {
		CWaitCursor wait;
		if (!pDocument->OnOpenDocument(lpszPathName)) {
			m_unk0x94 = "CDocument::OnNewDocument returned FALSE (v2)";
			pFrame->DestroyWindow();
			return NULL;
		}
		pDocument->SetPathName(lpszPathName);
	}

	InitialUpdateFrame(pFrame, pDocument, bMakeVisible);
	return pDocument;
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

// FUNCTION: ROBLOX 0x0044d9d0
static void FUN_0044d9d0(RBX::Reflection::ConstProperty property)
{
	if (property.getDescriptor().hasStringValue()) {
		RBX::Log::current()->writeEntry(
			RBX::Log::Information,
			G3D::format(
				"\t\t%s.%s: %s",
				property.getDescriptor().category.c_str(),
				property.getName().c_str(),
				property.getStringValue().c_str()
			)
				.c_str()
		);
	}
}

// FUNCTION: ROBLOX 0x0044dac0
BOOL CRobloxApp::FUN_0044dac0(bool p_unk0x04)
{
	if (m_pMainWnd != NULL) {
		if (!m_pMainWnd->IsWindowVisible()) {
			static_cast<CMainFrame*>(m_pMainWnd)->FUN_00432550(m_nCmdShow);
		}

		return m_pMainWnd->m_hWnd != NULL;
	}

	CMainFrame* frame = new CMainFrame;

	if (frame == NULL) {
		return FALSE;
	}

	if (!frame->LoadFrame(128)) {
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

// FUNCTION: ROBLOX 0x0044dc00
void CRobloxApp::OnAppAbout()
{
	CAboutRobloxDialog aboutDlg;
	aboutDlg.DoModal();
}

// FUNCTION: ROBLOX 0x0044dc90
void CRobloxApp::FUN_0044dc90()
{
	CString parameters;
	parameters.Format("url.dll,FileProtocolHandler %s", "http://wiki.roblox.com");
	ShellExecute(AfxGetMainWnd()->m_hWnd, "open", "rundll32.exe", parameters, NULL, SW_SHOWDEFAULT);
}

// FUNCTION: ROBLOX 0x0044e100 FOLDED
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

// FUNCTION: ROBLOX 0x0044e180
BOOL CRobloxApp::FUN_0044e180(std::string script)
{
	CRbxPlayDocTemplate* docTemplate = static_cast<CIDEDocManager*>(m_pDocManager)->m_unk0x24;
	POSITION pos = docTemplate->GetFirstDocPosition();
	while (pos) {
		CRobloxDoc* doc = static_cast<CRobloxDoc*>(docTemplate->GetNextDoc(pos));
		if (!doc->FUN_00459950(script)) {
			return FALSE;
		}
	}
	return TRUE;
}

// FUNCTION: ROBLOX 0x0044e250
void CRobloxApp::FUN_0044e250()
{
	static_cast<CIDEDocManager*>(m_pDocManager)->m_unk0x20->OpenDocumentFile(NULL);
}

// FUNCTION: ROBLOX 0x0044e270
void CRobloxApp::FUN_0044e270()
{
	static_cast<CIDEDocManager*>(m_pDocManager)->m_unk0x2c->OpenDocumentFile(NULL);
}

// FUNCTION: ROBLOX 0x0044e290
void CRobloxApp::FUN_0044e290()
{
	static_cast<CRobloxDoc*>(static_cast<CIDEDocManager*>(m_pDocManager)->m_unk0x24->OpenDocumentFile(NULL))
		->FUN_00459c80();
}

// FUNCTION: ROBLOX 0x0044ea70
BOOL CRobloxApp::FUN_0044ea70(std::string script)
{
	return !FUN_0044e180(script);
}

// FUNCTION: ROBLOX 0x0044eaf0
void CScriptDoc::FUN_0044eaf0(boost::shared_ptr<RBX::Script> script)
{
	m_unk0x54 = script;
	SetTitle(script->getName().c_str());
	UpdateAllViews(NULL, 1);
}

// FUNCTION: ROBLOX 0x0044ebb0
void FUN_0044ebb0(const std::exception& e)
{
	g_unk0x00954294.reset(new std::runtime_error(e.what()));
}

// FUNCTION: ROBLOX 0x0044ec70
void FUN_0044ec70()
{
	if (g_unk0x00954294) {
		std::runtime_error error(g_unk0x00954294->what());
		g_unk0x00954294.reset();
		throw error;
	}
}

// FUNCTION: ROBLOX 0x0044ed40
void CIDEDocManager::OnFileNew()
{
	g_unk0x00954294.reset();
	CDocument* document = m_unk0x24->OpenDocumentFile(NULL);
	try {
		FUN_0044ec70();
	}
	catch (std::exception& e) {
		FUN_0042b3c0(e);
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		if (document) {
			document->OnCloseDocument();
		}
	}
}

// FUNCTION: ROBLOX 0x0044edf0
static void FUN_0044edf0(
	const char* category, std::stringstream* stream, const RBX::Reflection::ConstProperty& property
)
{
	if (property.getDescriptor().category == category) {
		*stream << property.getName().c_str();
		*stream << ':';
		*stream << property.getStringValue();
		*stream << ';';
	}
}

// FUNCTION: ROBLOX 0x0044eee0
int CRobloxApp::ExitInstance()
{
	RBX::StandardOut::singleton()->print(RBX::MESSAGE_INFO, "ROBLOX ExitInstance");
	_AtlModule.RevokeClassObjects();
	return CWinApp::ExitInstance();
}

// FUNCTION: ROBLOX 0x0044ef90
void CRobloxApp::FUN_0044ef90(boost::shared_ptr<RBX::Script> script)
{
	CScriptDoc* doc =
		static_cast<CScriptDoc*>(static_cast<CIDEDocManager*>(m_pDocManager)->m_unk0x28->OpenDocumentFile(NULL));
	doc->FUN_0044eaf0(script);
}

// FUNCTION: ROBLOX 0x0044f970
CDocument* CRbxPlayDocTemplate::FUN_0044f970()
{
	if (!m_unk0x90) {
		g_unk0x00954294.reset();
		CDocument* document = FUN_0044d620(NULL, TRUE);
		try {
			FUN_0044ec70();
		}
		catch (std::exception&) {
			AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
			if (document) {
				document->OnCloseDocument();
			}
			throw;
		}

		if (!document) {
			if (!m_unk0x94.IsEmpty()) {
				throw std::runtime_error(static_cast<LPCTSTR>(m_unk0x94));
			}
			throw std::runtime_error("OpenDocumentFile returned NULL");
		}

		m_unk0x90 = document;
		return document;
	}
	else {
		if (!m_unk0x90->SaveModified()) {
			throw std::runtime_error("User cancelled");
		}

		SetDefaultTitle(m_unk0x90);
		if (!m_unk0x90->OnNewDocument()) {
			throw std::runtime_error("CDocument::OnNewDocument returned FALSE");
		}

		if (ActiveDocView::g_unk0x00952d8c == m_unk0x90) {
			ActiveDocView::FUN_004054f0(true, ActiveDocView::g_unk0x00952d8c);
		}

		POSITION pos = m_unk0x90->GetFirstViewPosition();
		while (pos) {
			CView* view = m_unk0x90->GetNextView(pos);
			g_unk0x00954294.reset();
			view->SendMessage(WM_INITIALUPDATE);
		}

		return m_unk0x90;
	}
}

// FUNCTION: ROBLOX 0x0044fbc0
CDocument* CRobloxApp::FUN_0044fbc0()
{
	CIDEDocManager* manager = static_cast<CIDEDocManager*>(m_pDocManager);
	return manager->m_unk0x24->FUN_0044f970();
}

// FUNCTION: ROBLOX 0x0044ff90
static void FUN_0044ff90(boost::shared_ptr<RBX::Instance> instance)
{
	RBX::Log::current()->writeEntry(RBX::Log::Information, G3D::format("\t%s", instance->getName().c_str()).c_str());
	std::for_each(instance->properties_begin(), instance->properties_end(), FUN_0044d9d0);
}

// FUNCTION: ROBLOX 0x004500e0
static void FUN_004500e0()
{
	RBX::Log::current()->writeEntry(
		RBX::Log::Information, G3D::format("GlobalSettings ---------------------------------------").c_str()
	);
	RBX::GlobalSettings::singleton()->visitChildren(FUN_0044ff90);
	RBX::Log::current()->writeEntry(
		RBX::Log::Information, G3D::format("End GlobalSettings -----------------------------------").c_str()
	);
}

// FUNCTION: ROBLOX 0x004506a0
CRobloxCommandLineInfo::CRobloxCommandLineInfo()
	: m_unk0x24(false), m_unk0x25(false), m_unk0x26(false), m_unk0x27(false)
{
	if (__argc > 0) {
		std::string commandLine;
		for (int i = 0; i < __argc; i++) {
			if (i != 0) {
				commandLine += " ";
			}
			commandLine += __argv[i];
		}
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_INFO, commandLine.c_str());
	}
}

// FUNCTION: ROBLOX 0x00450800
void FUN_00450800(CString p_unk0x04)
{
	g_unk0x00954290 = p_unk0x04;
}

// FUNCTION: ROBLOX 0x004508c0
CRobloxApp::CRobloxApp() : m_unk0xa4("Roblox")
{
}

// FUNCTION: ROBLOX 0x00450940
CRobloxApp::~CRobloxApp()
{
}

// FUNCTION: ROBLOX 0x004509e0
void CRobloxApp::FUN_004509e0()
{
	SetRegistryKey("ROBLOX Corporation");
	LoadStdProfileSettings(6);

	CXTPPropExchangeXMLNode px(TRUE, NULL, "Settings");
	px.SetCompactMode(FALSE);

	CString moduleFileName;
	AfxGetModuleShortFileName(AfxGetApp()->m_hInstance, moduleFileName);
	CPath path(moduleFileName);
	path.RemoveFileSpec();
	px.LoadFromFile(path.m_strPath + "\\AppSettings.xml");

	{
		BOOL silentCrashReport = TRUE;
		if (!PX_Bool(&px, "SilentCrashReport", silentCrashReport) || silentCrashReport) {
			silentCrashReport = FALSE;
			CRegKey key;
			if (key.Open(HKEY_CURRENT_USER, "Software\\ROBLOX Corporation\\Roblox", KEY_READ) == ERROR_SUCCESS) {
				DWORD value = 0;
				key.QueryDWORDValue("SilentCrashReport", value);
				silentCrashReport = value;
				g_unk0x00954290.ReleaseBuffer();
			}
		}
		RobloxCrashReporter::silent = silentCrashReport != FALSE;
	}

	PX_String(&px, "BaseUrl", g_unk0x00954290);
	if (g_unk0x00954290.IsEmpty()) {
		CRegKey key;
		if (key.Open(HKEY_LOCAL_MACHINE, "Software\\Roblox", KEY_READ) == ERROR_SUCCESS) {
			ULONG length = 1024;
			key.QueryStringValue("BaseUrl", g_unk0x00954290.GetBuffer(1024), &length);
			g_unk0x00954290.ReleaseBuffer();
		}
	}

	CString contentFolder;
	PX_String(&px, "ContentFolder", contentFolder);
	if (!contentFolder.IsEmpty()) {
		CPath assetFolder;
		assetFolder.Combine(path, contentFolder);
		RBX::ContentProvider::singleton().setAssetFolder(assetFolder);
	}

	CRenderSettings::singleton();
	RBX::DebugSettings::singleton();
	RBX::GameSettings::singleton();
	RBX::GlobalSettings::singleton()->loadState();
	FUN_004500e0();
}

// FUNCTION: ROBLOX 0x00450e30
static void FUN_00450e30()
{
	std::string url = FUN_00407190() + "/Game/MachineConfiguration.ashx";
	std::string response;

	try {
		std::stringstream stream;

		RBX::DebugSettings& debugSettings = RBX::DebugSettings::singleton();
		std::for_each(
			debugSettings.properties_begin(),
			debugSettings.properties_end(),
			boost::bind(FUN_0044edf0, "Profile", &stream, _1)
		);

		CRenderSettings& renderSettings = CRenderSettings::singleton();
		std::for_each(
			renderSettings.properties_begin(),
			renderSettings.properties_end(),
			boost::bind(FUN_0044edf0, "General", &stream, _1)
		);

		stream.flush();
		RBX::Http(url).post(stream, false, response);
	}
	catch (std::exception&) {
	}
}

// FUNCTION: ROBLOX 0x004510b0
void CRobloxApp::FUN_004510b0(CCmdUI* p_unk0x04)
{
	switch (RBX::DebugSettings::singleton().getErrorReporting()) {
	case RBX::DebugSettings::Prompt:
		if (CApp::FUN_00407300()) {
			p_unk0x04->Enable(TRUE);
		}
		break;
	case RBX::DebugSettings::Report:
		p_unk0x04->Enable(TRUE);
		break;
	}
}

// FUNCTION: ROBLOX 0x00451100
void CRobloxApp::FUN_00451100()
{
	if (CApp::FUN_00407300()) {
		switch (RBX::DebugSettings::singleton().getErrorReporting()) {
		case RBX::DebugSettings::Prompt: {
			CRobloxDHtmlDialog dialog(124, FUN_00407190() + "/Error/ReportDialog.aspx", NULL);
			dialog.DoModal();
			break;
		}
		case RBX::DebugSettings::Report:
			CApp::FUN_00407a40(FUN_00407190() + "/Error/Dmp.ashx");
			break;
		}
	}
}

// FUNCTION: ROBLOX 0x00451550
BOOL CRobloxApp::FUN_00451550(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end)
{
	return std::find_if(begin, end, std::bind1st(std::mem_fun(&CRobloxApp::FUN_0044ea70), this)) == end;
}

// FUNCTION: ROBLOX 0x00451710
void CRobloxCommandLineInfo::ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast)
{
	if (m_unk0x24) {
		FUN_00450800(pszParam);
		m_unk0x24 = false;
		ParseLast(bLast);
	}
	else if (m_unk0x25) {
		m_unk0x28.push_back(pszParam);
		m_unk0x25 = false;
		ParseLast(bLast);
	}
	else {
		if (bFlag) {
			if (CString("No3D").CompareNoCase(pszParam) == 0) {
				m_unk0x27 = true;
				ParseLast(bLast);
				return;
			}
			if (CString("Browser").CompareNoCase(pszParam) == 0) {
				m_unk0x26 = true;
				ParseLast(bLast);
				return;
			}
			if (CString("BaseUrl").CompareNoCase(pszParam) == 0) {
				m_unk0x24 = true;
				ParseLast(bLast);
				return;
			}
			if (CString("Script").CompareNoCase(pszParam) == 0) {
				m_unk0x25 = true;
				ParseLast(bLast);
				return;
			}
		}
		CCommandLineInfo::ParseParam(pszParam, bFlag, bLast);
	}
}

// STUB: ROBLOX 0x004521d0
BOOL CRobloxApp::InitInstance()
{
	try {
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

		{
			DWORD crashReport = 1;
			CRegKey key;
			if (key.Open(HKEY_CURRENT_USER, "Software\\ROBLOX Corporation\\Roblox", KEY_READ) == ERROR_SUCCESS) {
				key.QueryDWORDValue("CrashReport", crashReport);
				g_unk0x00954290.ReleaseBuffer();
			}
			if (crashReport) {
				m_unk0xa4.WriteCrashDump();
			}
		}

		if (!AfxOleInit()) {
			AfxMessageBox(100);
			return FALSE;
		}

		FUN_0044d4a0();

		CRobloxCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);

		if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister) {
			_AtlModule.UpdateRegistryAppId(FALSE);
			_AtlModule.UnregisterServer(TRUE);
		}
		else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppRegister) {
			_AtlModule.UpdateRegistryAppId(TRUE);
			_AtlModule.RegisterServer(TRUE);
		}
		else {
			RBX::Profiling::init(true);
			RBX::Reflection::Metadata::FUN_00447680();

			static RBX::FactoryRegistrator registrator;
			RBX::Network::API::init(m_unk0xf0.c_str());
			AfxEnableControlContainer();

			FUN_004509e0();
			FUN_00450e30();

			if (CRenderSettings::singleton().FUN_0044d430() == CRenderSettings::c_graphicsModeUnk0x5) {
				cmdInfo.m_unk0x27 = true;
			}

			FUN_0041ac10();

			if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated) {
				m_pDocManager = new CIDEDocManager(cmdInfo.m_unk0x27);
				if (FAILED(AtlComModuleRegisterClassObjects(&_AtlComModule, CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE))) {
					return FALSE;
				}
				return TRUE;
			}

			if (cmdInfo.m_unk0x26) {
				m_pDocManager = new CBrowserDocManager();
			}
			else {
				m_pDocManager = new CIDEDocManager(cmdInfo.m_unk0x27);
				if (!FUN_0044dac0(false)) {
					return FALSE;
				}
				if (FAILED(AtlComModuleRegisterClassObjects(&_AtlComModule, CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE))) {
					return FALSE;
				}

				if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew) {
					if (cmdInfo.m_unk0x28.size() > 0) {
						ProcessShellCommand(cmdInfo);
						return FUN_00451550(cmdInfo.m_unk0x28.begin(), cmdInfo.m_unk0x28.end());
					}
					if (!cmdInfo.m_unk0x27) {
						FUN_0044e250();
					}
					return TRUE;
				}

				if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen) {
					return ProcessShellCommand(cmdInfo) &&
						   FUN_00451550(cmdInfo.m_unk0x28.begin(), cmdInfo.m_unk0x28.end());
				}
			}

			return ProcessShellCommand(cmdInfo);
		}
	}
	catch (std::exception& e) {
		FUN_0042b3c0(e);
		AfxMessageBox(e.what(), MB_ICONERROR);
	}

	return FALSE;
}

// FUNCTION: ROBLOX 0x00452960
int CRobloxApp::Run()
{
	RBX::Security::Impersonator impersonate(RBX::Security::LocalGUI);
	return CWinApp::Run();
}

IMPLEMENT_XTP_CONTROL(CRobloxControlColorSelector, CXTPControl)

// GLOBAL: ROBLOX 0x00954298
static int g_unk0x00954298;

// GLOBAL: ROBLOX 0x0095429c
static std::vector<CRobloxControlColorSelector::CColorInfo> g_unk0x0095429c;

static COLORREF toColorRef(RBX::BrickColor color)
{
	return RGB(color.color3uint8().r, color.color3uint8().g, color.color3uint8().b);
}

// FUNCTION: ROBLOX 0x00452a00
BOOL CRobloxControlColorSelector::OnSetSelected(int bSelected)
{
	if (!bSelected) {
		m_unk0x16c = -1;
	}

	return CXTPControl::OnSetSelected(bSelected);
}

// FUNCTION: ROBLOX 0x00452a20
COLORREF CRobloxControlColorSelector::GetColor() const
{
	return m_unk0x178;
}

// FUNCTION: ROBLOX 0x00452a30
void CRobloxControlColorSelector::SetColor(COLORREF clr)
{
	m_unk0x178 = toColorRef(RBX::BrickColor::closest(G3D::Color3uint8(GetRValue(clr), GetGValue(clr), GetBValue(clr))));
	RedrawParent();
}

// FUNCTION: ROBLOX 0x00452b80
CSize CRobloxControlColorSelector::GetSize(CDC* pDC)
{
	return CSize(260, 132);
}

CRect CRobloxControlColorSelector::GetRect(int nIndex) const
{
	int x = nIndex % 8, y = nIndex / 8;

	return CRect(CPoint(2 + m_rcControl.left + x * 32, 2 + m_rcControl.top + y * 32), CSize(32, 32));
}

// FUNCTION: ROBLOX 0x00452ba0
int CRobloxControlColorSelector::HitTest(CPoint point) const
{
	int nCount = g_unk0x0095429c.size();

	if (!m_rcControl.PtInRect(point)) {
		return -1;
	}

	for (int i = 0; i < nCount; i++) {
		if (GetRect(i).PtInRect(point)) {
			return i;
		}
	}

	return -1;
}

// FUNCTION: ROBLOX 0x00452c80
void CRobloxControlColorSelector::Draw(CDC* pDC)
{
	int nCount = g_unk0x0095429c.size();

	for (int i = 0; i < nCount; i++) {
		CRect rcButton = GetRect(i);

		if (i == m_unk0x16c || g_unk0x0095429c[i].clrButton == m_unk0x178) {
			BOOL bPressed = GetPressed() && i == m_unk0x16c;
			GetPaintManager()->DrawRectangle(
				pDC,
				rcButton,
				i == m_unk0x16c,
				bPressed,
				GetEnabled(),
				g_unk0x0095429c[i].clrButton == m_unk0x178,
				FALSE,
				xtpBarTypeNormal,
				xtpBarPopup
			);
		}

		rcButton.DeflateRect(3, 3);
		if (GetEnabled()) {
			pDC->FillSolidRect(rcButton, g_unk0x0095429c[i].clrButton);
		}

		pDC->Draw3dRect(rcButton, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DSHADOW));
	}
}

// FUNCTION: ROBLOX 0x00452ef0
void CRobloxControlColorSelector::OnMouseMove(CPoint point)
{
	int nHit = HitTest(point);
	if (nHit != -1) {
		m_unk0x16c = nHit;
		RedrawParent();
	}
}

// FUNCTION: ROBLOX 0x00452f20
void CRobloxControlColorSelector::OnExecute()
{
	if (m_unk0x170 != -1) {
		m_unk0x178 = g_unk0x0095429c[m_unk0x170].clrButton;
		m_unk0x174 = true;
	}

	CXTPControl::OnExecute();
}

// FUNCTION: ROBLOX 0x00452f80
void CRobloxControlColorSelector::OnClick(BOOL bKeyboard, CPoint pt)
{
	if (!GetEnabled()) {
		return;
	}

	int nHit = HitTest(pt);
	if (nHit == -1) {
		return;
	}

	m_unk0x170 = nHit;

	if (!bKeyboard) {
		ClickToolBarButton();
	}
	else {
		OnExecute();
	}

	m_unk0x170 = -1;
}

void CRobloxControlColorSelector::OnLButtonUp(CPoint point)
{
	if (GetEnabled() && m_pParent->GetPosition() == xtpBarPopup) {
		OnClick(TRUE, point);
	}
}

// FUNCTION: ROBLOX 0x00453780
static void FUN_00453780(RBX::BrickColor color)
{
	CRobloxControlColorSelector::CColorInfo info(toColorRef(color), g_unk0x00954298++);
	info.strTip = color.name().c_str();
	g_unk0x0095429c.push_back(info);
}

// FUNCTION: ROBLOX 0x00453860
CRobloxControlColorSelector::CRobloxControlColorSelector() : m_unk0x174(false)
{
	if (g_unk0x0095429c.empty()) {
		std::for_each(RBX::BrickColor::colorPalette().begin(), RBX::BrickColor::colorPalette().end(), FUN_00453780);
	}

	m_unk0x16c = -1;
	m_unk0x178 = (COLORREF) -1;
	SetFlags(xtpFlagNoMovable);
	m_unk0x170 = -1;
}

IMPLEMENT_DYNAMIC(CRobloxDHtmlDialog, CDHtmlDialog)

BEGIN_EVENTSINK_MAP(CRobloxDHtmlDialog, CDHtmlDialog)
ON_EVENT(
	CRobloxDHtmlDialog,
	AFX_IDC_BROWSER,
	DISPID_BEFORENAVIGATE2,
	OnBeforeNavigate2,
	VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL
)
ON_EVENT(CRobloxDHtmlDialog, AFX_IDC_BROWSER, DISPID_WINDOWCLOSING, OnWindowClosing, VTS_BOOL VTS_PBOOL)
END_EVENTSINK_MAP()

// FUNCTION: ROBLOX 0x00453aa0
CRobloxDHtmlDialog::CRobloxDHtmlDialog(UINT nIDTemplate, const char* url, CWnd* pParentWnd)
	: CDHtmlDialog(nIDTemplate, 114, pParentWnd), m_unk0x114(url)
{
	m_bUseHtmlTitle = TRUE;
	SetHostFlags(m_dwHostFlags | 0x1844013);
}

// FUNCTION: ROBLOX 0x00453b40
CRobloxDHtmlDialog::CRobloxDHtmlDialog(UINT nIDTemplate, UINT nHtmlResID, CWnd* pParentWnd)
	: CDHtmlDialog(nIDTemplate, nHtmlResID, pParentWnd)
{
	m_bUseHtmlTitle = TRUE;
	SetHostFlags(m_dwHostFlags | 0x1844013);
}

// FUNCTION: ROBLOX 0x00453be0
BOOL CRobloxDHtmlDialog::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	if (!m_unk0x114.empty()) {
		Navigate(m_unk0x114.c_str());
		return FALSE;
	}

	return TRUE;
}

// FUNCTION: ROBLOX 0x00453c20
void CRobloxDHtmlDialog::OnBeforeNavigate2(
	LPDISPATCH pDisp,
	VARIANT* url,
	VARIANT* flags,
	VARIANT* targetFrameName,
	VARIANT* postData,
	VARIANT* headers,
	BOOL* cancel
)
{
	CString strUrl(V_BSTR(url));
	if (FUN_0041e610(strUrl)) {
		*cancel = TRUE;
	}
	else {
		OnBeforeNavigate(pDisp, strUrl);
	}
}

// FUNCTION: ROBLOX 0x00453ce0
void CRobloxDHtmlDialog::OnWindowClosing(BOOL isChildWindow, BOOL* cancel)
{
	*cancel = -1;

	CString result;
	DDX_DHtml_ElementText(_T("DialogResult"), DISPID_A_VALUE, result, TRUE);

	if (result == "") {
		OnCancel();
	}
	else {
		EndDialog(atoi(result));
	}
}

// FUNCTION: ROBLOX 0x00453eb0 FOLDED
void CRobloxApp::FUN_00453eb0(CCmdUI* p_unk0x04)
{
	p_unk0x04->Enable(TRUE);
}

CRobloxApp theApp;
