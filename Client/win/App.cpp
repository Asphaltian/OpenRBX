#include "App.h"

#include "LogManager.h"
#include "Roblox.h"
#include "RobloxDoc.h"
#include "Unknown00409ef0.h"
#include "util/ContentProvider.h"
#include "util/standardout.h"
#include "v8datamodel/DebugSettings.h"

#include <atlutil.h>
#include <boost/scoped_ptr.hpp>
#include <iphlpapi.h>

STDMETHODIMP CApp::LaunchIDE()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return static_cast<CRobloxApp*>(AfxGetApp())->FUN_0044dac0(false) ? S_OK : E_FAIL;
}

STDMETHODIMP CApp::get_HasErrorFiles(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	*pVal = theApp.m_unk0xa4.hasCrashLogs() ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

bool CApp::FUN_00407300()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return theApp.m_unk0xa4.hasCrashLogs();
}

STDMETHODIMP CApp::get_Version(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	CComBSTR version(static_cast<CRobloxApp*>(AfxGetApp())->m_unk0xf0.c_str());
	return version.CopyTo(pVal);
}

void CApp::FUN_00407a40(const char* url)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	theApp.m_unk0x10c->Upload(url, false);
}

STDMETHODIMP CApp::SendErrorFiles(BSTR url)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	FUN_00407a40(CString(url));
	return S_OK;
}

STDMETHODIMP CApp::ClearFileCache()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	RBX::ContentProvider::singleton().clearFileCache();
	return S_OK;
}

HRESULT FUN_00407ca0(CComObject<CWorkspace>** p_unk0x04, void* p_unk0x08)
{
	HRESULT result = E_OUTOFMEMORY;
	CComObject<CWorkspace>* object = NULL;

	ATLTRY(object = new CComObject<CWorkspace>(p_unk0x08))

	if (object != NULL) {
		object->SetVoid(p_unk0x08);
		object->InternalFinalConstructAddRef();
		result = object->FinalConstruct();
		object->InternalFinalConstructRelease();

		if (result != S_OK) {
			delete object;
			object = NULL;
		}
	}

	*p_unk0x04 = object;
	return result;
}

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

STDMETHODIMP CApp::get_IsUploadingErrorFiles(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	*pVal = theApp.m_unk0x10c->IsUploading() ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CApp::CreateGame(WINDOW_TYPE WindowType, IWorkspace** result)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	CRobloxDoc* doc = NULL;
	try {
		HRESULT hr = FUN_004085e0();
		if (hr != S_OK) {
			return hr;
		}

		CRobloxApp* app = static_cast<CRobloxApp*>(AfxGetApp());
		CComObject<CWorkspace>* workspace;
		switch (WindowType) {
		case NoWindowType:
			hr = FUN_00407ca0(&workspace, NULL);
			if (hr != S_OK) {
				return LogManager::ReportCOMError(CLSID_App, "Couldn't create workspace", hr);
			}
			break;
		case PlayWindowType: {
			doc = static_cast<CRobloxDoc*>(app->FUN_0044fbc0());
			workspace = doc->m_unk0x78;

			POSITION pos = doc->GetFirstViewPosition();
			if (pos) {
				CView* view = doc->GetNextView(pos);
				if (view) {
					view->GetParentFrame()->ShowWindow(SW_SHOW);
					view->SetForegroundWindow();
				}
			}
			break;
		}
		default:
			return LogManager::ReportCOMError(CLSID_App, "requested WindowType is invalid", ERROR_INVALID_PARAMETER);
		}

		CComPtr<IWorkspace> ptr(workspace);
		hr = ptr.CopyTo(result);
		if (FAILED(hr)) {
			return LogManager::ReportCOMError(CLSID_App, "iworkspace.CopyTo failed", hr);
		}

		return S_OK;
	}
	catch (std::exception& e) {
		if (doc) {
			doc->OnCloseDocument();
		}
		return LogManager::ReportExceptionAsCOMError(CLSID_App, e);
	}
}

STDMETHODIMP CApp::get_ErrorReporting(ERROR_REPORTING* pVal)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	*pVal = static_cast<ERROR_REPORTING>(RBX::DebugSettings::singleton().errorReporting);
	return S_OK;
}

STDMETHODIMP CApp::RobloxAuthenticate(BSTR url, BSTR ticket)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	RBX::StandardOut::singleton()->print(
		RBX::MESSAGE_INFO,
		"CApp::RobloxAuthenticate %s %s",
		static_cast<LPCSTR>(CString(url)),
		static_cast<LPCSTR>(CString(ticket))
	);

	try {
		CUrl crackedUrl;
		crackedUrl.CrackUrl(FUN_00407190());

		Unknown00409ef0 authenticator(crackedUrl.GetHostName());
		authenticator.FUN_00409f70(CString(url), CString(ticket));
	}
	catch (std::exception& e) {
		return LogManager::ReportExceptionAsCOMError(CLSID_App, e);
	}

	return S_OK;
}

STDMETHODIMP CApp::DeleteErrorFiles()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	theApp.m_unk0xa4.archiveCrashLogs();
	return S_OK;
}

OBJECT_ENTRY_AUTO(__uuidof(App), CApp)

template const RBX::FactoryProduct<RBX::DebugSettings, RBX::GlobalSettings::Item, RBX::sDebugSettings>::Creator
	RBX::FactoryProduct<RBX::DebugSettings, RBX::GlobalSettings::Item, RBX::sDebugSettings>::creator;
