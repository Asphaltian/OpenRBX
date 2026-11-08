#ifndef CONTENT_H
#define CONTENT_H

#include "Roblox_h.h"
#include "decomp.h"

#include <atlbase.h>
#include <atlcom.h>
#include <sstream>

// SIZE 0x94
class __declspec(novtable) CContent : public CComObjectRootEx<CComSingleThreadModel>,
									  public CComCoClass<CContent, &CLSID_Content>,
									  public ISupportErrorInfoImpl<&IID_IContent>,
									  public IDispatchImpl<IContent, &IID_IContent, &LIBID_RobloxLib, 1, 0>
{
public:
	// clang-format off
	BEGIN_COM_MAP(CContent)
		COM_INTERFACE_ENTRY(IContent)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()
	// clang-format on

	STDMETHOD(Upload)(BSTR url);

	std::stringstream m_unk0x0c; // 0x0c
};

DECOMP_SIZE_ASSERT(CContent, 0x94)

#endif // CONTENT_H
