#ifndef CONTENT_H
#define CONTENT_H

#include "Roblox_h.h"
#include "decomp.h"
#include "reflection/type.h"

#include <atlbase.h>
#include <atlcom.h>
#include <sstream>
#include <vector>

// SIZE 0x94
class __declspec(novtable) CContent : public CComObjectRootEx<CComSingleThreadModel>,
									  public CComCoClass<CContent, &CLSID_Content>,
									  public ISupportErrorInfoImpl<&IID_IContent>,
									  public IDispatchImpl<IContent, &IID_IContent, &LIBID_RobloxLib, 1, 0>
{
public:
	DECLARE_REGISTRY_RESOURCEID(116)

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

OBJECT_ENTRY_AUTO(__uuidof(Content), CContent)

void FUN_004132b0(const RBX::Reflection::Value& value, VARIANT* result);
SAFEARRAY* FUN_00413500(const std::vector<RBX::Reflection::Value>& values);

#endif // CONTENT_H
