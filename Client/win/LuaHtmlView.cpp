#include "decomp.h"
#include "util/StandardOut.h"

#include <boost/function.hpp>
#include <exception>
#include <oaidl.h>

// VTABLE: ROBLOX 0x00804010
// SIZE 0x30
class EventHandler : public IDispatch
{
public:
	EventHandler(boost::function<bool()> handler);

	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	STDMETHOD(GetTypeInfoCount)(UINT* pctinfo);
	STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo);
	STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId);
	STDMETHOD(Invoke)(
		DISPID dispIdMember,
		REFIID riid,
		LCID lcid,
		WORD wFlags,
		DISPPARAMS* pDispParams,
		VARIANT* pVarResult,
		EXCEPINFO* pExcepInfo,
		UINT* puArgErr
	);

private:
	boost::function<bool()> m_unk0x08; // 0x08
	ULONG m_unk0x28;                   // 0x28
};

DECOMP_SIZE_ASSERT(EventHandler, 0x30)

// FUNCTION: ROBLOX 0x0042e150
EventHandler::EventHandler(boost::function<bool()> handler) : m_unk0x08(handler), m_unk0x28(0)
{
}

// STUB: ROBLOX 0x0042e290
STDMETHODIMP EventHandler::QueryInterface(REFIID riid, void** ppvObject)
{
	*ppvObject = NULL;

	if (IsEqualGUID(riid, IID_IUnknown)) {
		*ppvObject = this;
		AddRef();
		return S_OK;
	}

	if (IsEqualGUID(riid, IID_IDispatch)) {
		*ppvObject = this;
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

// FUNCTION: ROBLOX 0x0042e2f0
ULONG STDMETHODCALLTYPE EventHandler::AddRef()
{
	return ++m_unk0x28;
}

// FUNCTION: ROBLOX 0x0042e300
STDMETHODIMP EventHandler::GetTypeInfoCount(UINT* pctinfo)
{
	*pctinfo = 0;
	return S_OK;
}

// FUNCTION: ROBLOX 0x0042e310
STDMETHODIMP EventHandler::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)
{
	return DISP_E_BADINDEX;
}

// FUNCTION: ROBLOX 0x0042e320
STDMETHODIMP EventHandler::GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)
{
	return DISP_E_UNKNOWNNAME;
}

// FUNCTION: ROBLOX 0x0042e760
STDMETHODIMP EventHandler::Invoke(
	DISPID dispIdMember,
	REFIID riid,
	LCID lcid,
	WORD wFlags,
	DISPPARAMS* pDispParams,
	VARIANT* pVarResult,
	EXCEPINFO* pExcepInfo,
	UINT* puArgErr
)
{
	V_VT(pVarResult) = VT_BOOL;

	try {
		V_BOOL(pVarResult) = m_unk0x08() ? 1 : 0;
		return S_FALSE;
	}
	catch (std::exception& e) {
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_WARNING, e);
		return E_FAIL;
	}
}

// FUNCTION: ROBLOX 0x0042ebb0
ULONG STDMETHODCALLTYPE EventHandler::Release()
{
	if (--m_unk0x28 == 0) {
		delete this;
		return 0;
	}

	return m_unk0x28;
}
