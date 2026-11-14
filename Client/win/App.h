#ifndef APP_H
#define APP_H

#include "IObjectSafetyRobloxImpl.h"
#include "Roblox_h.h"
#include "placeholder.h"

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

class __declspec(novtable) CApp : public CComObjectRootEx<CComSingleThreadModel>,
								  public CComCoClass<CApp, &CLSID_App>,
								  public ISupportErrorInfoImpl<&IID_IApp>,
								  public IObjectSafetyRobloxImpl<CApp, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
								  public IObjectWithSiteImpl<CApp>,
								  public IDispatchImpl<IApp, &IID_IApp, &LIBID_RobloxLib, 1, 2>
{
public:
	DECLARE_REGISTRY_RESOURCEID(110)

	// clang-format off
	BEGIN_COM_MAP(CApp)
		COM_INTERFACE_ENTRY(IApp)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IObjectWithSite)
		COM_INTERFACE_ENTRY(IObjectSafety)
	END_COM_MAP()
	// clang-format on

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	STDMETHOD(CreateGame)(WINDOW_TYPE WindowType, IWorkspace** result);
	STDMETHOD(LaunchIDE)();
	STDMETHOD(get_Version)(BSTR* pVal);
	STDMETHOD(get_ErrorReporting)(ERROR_REPORTING* pVal);
	STDMETHOD(get_HasErrorFiles)(VARIANT_BOOL* pVal);
	STDMETHOD(SendErrorFiles)(BSTR url);
	STDMETHOD(get_IsUploadingErrorFiles)(VARIANT_BOOL* pVal);
	STDMETHOD(DeleteErrorFiles)();
	STDMETHOD(ClearFileCache)();
	STDMETHOD(get_ID)(BSTR* pVal);
	STDMETHOD(RobloxAuthenticate)(BSTR url, BSTR ticket);

	static bool FUN_00407300();
	static void FUN_00407a40(const char* url);
};

#endif // APP_H
