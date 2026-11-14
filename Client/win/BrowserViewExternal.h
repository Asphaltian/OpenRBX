#ifndef BROWSERVIEWEXTERNAL_H
#define BROWSERVIEWEXTERNAL_H

#include "Roblox_h.h"
#include "_IBrowserViewExternalEventsCP.h"
#include "placeholder.h"

#include <atlbase.h>
#include <atlcom.h>

class __declspec(novtable) CBrowserViewExternal
	: public CComObjectRootEx<CComSingleThreadModel>,
	  public CComCoClass<CBrowserViewExternal, &CLSID_BrowserViewExternal>,
	  public ISupportErrorInfoImpl<&IID_IBrowserViewExternal>,
	  public IConnectionPointContainerImpl<CBrowserViewExternal>,
	  public CProxy_IBrowserViewExternalEvents<CBrowserViewExternal>,
	  public IDispatchImpl<IBrowserViewExternal, &IID_IBrowserViewExternal, &LIBID_RobloxLib, 1, 0>
{
public:
	CBrowserViewExternal() : m_unk0x1c(VARIANT_FALSE) {}

	DECLARE_REGISTRY_RESOURCEID(119)

	// clang-format off
	BEGIN_COM_MAP(CBrowserViewExternal)
		COM_INTERFACE_ENTRY(IBrowserViewExternal)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CBrowserViewExternal)
		CONNECTION_POINT_ENTRY(__uuidof(_IBrowserViewExternalEvents))
	END_CONNECTION_POINT_MAP()
	// clang-format on

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	STDMETHOD(get_IsRobloxIDE)(VARIANT_BOOL* pVal);
	STDMETHOD(GetApp)(IDispatch** result);

	VARIANT_BOOL m_unk0x1c;
};

#endif // BROWSERVIEWEXTERNAL_H
