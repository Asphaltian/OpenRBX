#include "App.h"

#include "Roblox.h"
#include "util/ContentProvider.h"
#include "v8datamodel/DebugSettings.h"

#include <boost/scoped_ptr.hpp>
#include <iphlpapi.h>

// FUNCTION: ROBLOX 0x004071d0
STDMETHODIMP CApp::LaunchIDE()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return static_cast<CRobloxApp*>(AfxGetApp())->FUN_0044dac0(false) ? S_OK : E_FAIL;
}

// FUNCTION: ROBLOX 0x00407270
STDMETHODIMP CApp::get_HasErrorFiles(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	*pVal = theApp.m_unk0xa4.hasCrashLogs() ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

// FUNCTION: ROBLOX 0x00407300
bool CApp::FUN_00407300()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return theApp.m_unk0xa4.hasCrashLogs();
}

// FUNCTION: ROBLOX 0x00407940
STDMETHODIMP CApp::get_Version(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	CComBSTR version(static_cast<CRobloxApp*>(AfxGetApp())->m_unk0xf0.c_str());
	return version.CopyTo(pVal);
}

// FUNCTION: ROBLOX 0x00407a40
void CApp::FUN_00407a40(const char* url)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	theApp.m_unk0x10c->Upload(url, false);
}

// FUNCTION: ROBLOX 0x00407ae0
STDMETHODIMP CApp::SendErrorFiles(BSTR url)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	FUN_00407a40(CString(url));
	return S_OK;
}

// FUNCTION: ROBLOX 0x00407b90
STDMETHODIMP CApp::ClearFileCache()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	RBX::ContentProvider::singleton().clearFileCache();
	return S_OK;
}

// FUNCTION: ROBLOX 0x00407fd0
STDMETHODIMP CApp::get_ID(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	IP_ADAPTER_INFO adapterInfo[100];
	DWORD bufferLength = sizeof(adapterInfo);
	DWORD result = GetAdaptersInfo(adapterInfo, &bufferLength);
	if (result != ERROR_SUCCESS) {
		return E_FAIL;
	}

	CComBSTR id;
	if (bufferLength > 0) {
		boost::scoped_ptr<RBX::MD5Hasher> hasher(RBX::MD5Hasher::create());
		for (unsigned int i = 0; i < bufferLength / sizeof(IP_ADAPTER_INFO); i++) {
			hasher->addData(adapterInfo[i].AdapterName);
		}
		id = CString(hasher->c_str());
	}

	return id.CopyTo(pVal);
}

// FUNCTION: ROBLOX 0x004081f0
STDMETHODIMP CApp::get_IsUploadingErrorFiles(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	*pVal = theApp.m_unk0x10c->IsUploading() ? VARIANT_TRUE : VARIANT_FALSE;
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

// FUNCTION: ROBLOX 0x00409d20
STDMETHODIMP CApp::DeleteErrorFiles()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	theApp.m_unk0xa4.archiveCrashLogs();
	return S_OK;
}

OBJECT_ENTRY_AUTO(__uuidof(App), CApp)
