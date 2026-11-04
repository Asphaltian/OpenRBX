#include "App.h"

#include "Roblox.h"
#include "util/ContentProvider.h"
#include "v8datamodel/DebugSettings.h"

// STUB: ROBLOX 0x004071d0
STDMETHODIMP CApp::LaunchIDE()
{
	STUB(0x4071d0);
	return S_OK;
}

// FUNCTION: ROBLOX 0x00407270
STDMETHODIMP CApp::get_HasErrorFiles(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	*pVal = theApp.m_unk0xa4.hasCrashLogs() ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

// FUNCTION: ROBLOX 0x00407940
STDMETHODIMP CApp::get_Version(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	CComBSTR version(static_cast<CRobloxApp*>(AfxGetApp())->m_unk0xf0.c_str());
	return version.CopyTo(pVal);
}

// STUB: ROBLOX 0x00407ae0
STDMETHODIMP CApp::SendErrorFiles(BSTR url)
{
	STUB(0x407ae0);
	return S_OK;
}

// FUNCTION: ROBLOX 0x00407b90
STDMETHODIMP CApp::ClearFileCache()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	RBX::ContentProvider::singleton().clearFileCache();
	return S_OK;
}

// STUB: ROBLOX 0x00407fd0
STDMETHODIMP CApp::get_ID(BSTR* pVal)
{
	STUB(0x407fd0);
	return S_OK;
}

// STUB: ROBLOX 0x004081f0
STDMETHODIMP CApp::get_IsUploadingErrorFiles(VARIANT_BOOL* pVal)
{
	STUB(0x4081f0);
	return S_OK;
}

// STUB: ROBLOX 0x00408920
STDMETHODIMP CApp::CreateGame(WINDOW_TYPE WindowType, IWorkspace** result)
{
	STUB(0x408920);
	return S_OK;
}

// FUNCTION: ROBLOX 0x00409060
STDMETHODIMP CApp::get_ErrorReporting(ERROR_REPORTING* pVal)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	*pVal = static_cast<ERROR_REPORTING>(RBX::DebugSettings::singleton().errorReporting);
	return S_OK;
}

// STUB: ROBLOX 0x00409ab0
STDMETHODIMP CApp::RobloxAuthenticate(BSTR url, BSTR ticket)
{
	STUB(0x409ab0);
	return S_OK;
}

// STUB: ROBLOX 0x00409d20
STDMETHODIMP CApp::DeleteErrorFiles()
{
	STUB(0x409d20);
	return S_OK;
}

OBJECT_ENTRY_AUTO(__uuidof(App), CApp)
