#ifndef LUAFUNCTION_H
#define LUAFUNCTION_H

#include "Roblox_h.h"
#include "decomp.h"
#include "reflection/type.h"
#include "script/ThreadRef.h"

#include <G3D/format.h>
#include <atlbase.h>
#include <atlcom.h>
#include <boost/shared_ptr.hpp>
#include <stdexcept>
#include <string>
#include <vector>

namespace RBX {
class DataModel;
}

// SIZE 0x10
class Unknown00431c00 : public std::vector<RBX::Reflection::Value>
{
public:
	// FUNCTION: ROBLOX 0x00431c00
	void FUN_00431c00(CComVariant value)
	{
		switch (value.vt) {
		case VT_R4:
			push_back(value.fltVal);
			break;
		case VT_R8:
			push_back((float) value.dblVal);
			break;
		case VT_BSTR:
			push_back(std::string(CString(value.bstrVal)));
			break;
		case VT_BOOL:
			push_back(value.boolVal == VARIANT_TRUE);
			break;
		case VT_I2:
			push_back((int) value.iVal);
			break;
		case VT_I4:
			push_back((int) value.lVal);
			break;
		case VT_NULL:
			push_back(RBX::Reflection::Value());
			break;
		case VT_EMPTY:
		case VT_ERROR:
			break;
		default: {
			std::string message = G3D::format("Invalid argument %d VT_TYPE=%d", size() + 1, value.vt);
			throw std::runtime_error(message);
		}
		}
	}
};

DECOMP_SIZE_ASSERT(Unknown00431c00, 0x10)

// SIZE 0x38
class __declspec(novtable) CLuaFunction : public CComObjectRootEx<CComSingleThreadModel>,
										  public CComCoClass<CLuaFunction, &CLSID_LuaFunction>,
										  public ISupportErrorInfoImpl<&IID_ILuaFunction>,
										  public IDispatchImpl<ILuaFunction, &IID_ILuaFunction, &LIBID_RobloxLib, 1, 0>
{
public:
	// clang-format off
	BEGIN_COM_MAP(CLuaFunction)
		COM_INTERFACE_ENTRY(ILuaFunction)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()
	// clang-format on

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	STDMETHOD(Call)(VARIANT arg1, VARIANT arg2, VARIANT arg3, VARIANT arg4, VARIANT* result);
	STDMETHOD(CallEx)(SAFEARRAY* args, SAFEARRAY** result);

	boost::shared_ptr<RBX::DataModel> m_unk0x0c; // 0x0c
	RBX::Lua::FunctionRef m_unk0x14;             // 0x14
};

DECOMP_SIZE_ASSERT(CLuaFunction, 0x38)

#endif // LUAFUNCTION_H
