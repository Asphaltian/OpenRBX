#ifndef APP_H
#define APP_H

#include "IObjectSafetyRobloxImpl.h"
#include "Roblox_h.h"
#include "decomp.h"

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

// SIZE 0x1c
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

DECOMP_SIZE_ASSERT(CApp, 0x1c)

// clang-format off
// STUB: ROBLOX 0x00408780
// IObjectSafetyRobloxImpl<CApp,1>::GetInterfaceSafetyOptions
// STUB: ROBLOX 0x00408810
// IObjectSafetyRobloxImpl<CApp,1>::SetInterfaceSafetyOptions
// STUB: ROBLOX 0x004085e0
// IObjectSafetyRobloxImpl<CApp,1>::FUN_004085e0
// clang-format on

#endif // APP_H
