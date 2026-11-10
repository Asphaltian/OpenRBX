#include "LuaHtmlView.h"

#include "DHTMLWindow.h"
#include "LuaFunction.h"
#include "Marshaller.h"
#include "decomp.h"
#include "reflection/property.h"
#include "util/StandardOut.h"
#include "v8datamodel/DataModel.h"
#include "v8tree/Instance.h"

#include <G3D/format.h>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/ref.hpp>
#include <dispex.h>
#include <exception>
#include <mshtmdid.h>
#include <mshtmhst.h>
#include <mshtml.h>
#include <oaidl.h>
#include <stdexcept>
#include <string>

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
	friend class CLuaHtmlView::Binder;

	boost::function<bool()> m_unk0x08; // 0x08
	ULONG m_unk0x28;                   // 0x28
};

DECOMP_SIZE_ASSERT(EventHandler, 0x30)

IMPLEMENT_DYNCREATE(CLuaHtmlView, CSecureHtmlView)

// clang-format off
BEGIN_MESSAGE_MAP(CLuaHtmlView, CSecureHtmlView)
	ON_WM_DESTROY()
END_MESSAGE_MAP()
// clang-format on

// FUNCTION: ROBLOX 0x0042d640
void CLuaHtmlView::FUN_0042d640()
{
	GetParentFrame()->DestroyWindow();
}

// FUNCTION: ROBLOX 0x0042d660
BOOL CLuaHtmlView::OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar)
{
	USES_CONVERSION;

	if (dispid != DISPID_AMBIENT_DLCONTROL) {
		return CSecureHtmlView::OnAmbientProperty(pSite, dispid, pvar);
	}

	if (!CSecureHtmlView::OnAmbientProperty(pSite, DISPID_AMBIENT_DLCONTROL, pvar)) {
		return FALSE;
	}

	V_I4(pvar) |= DLCTL_NO_SCRIPTS;
	V_VT(pvar) = VT_I4;
	return TRUE;
}

// FUNCTION: ROBLOX 0x0042d6c0
HRESULT CLuaHtmlView::OnGetHostInfo(DOCHOSTUIINFO* pInfo)
{
	HRESULT hr = CSecureHtmlView::OnGetHostInfo(pInfo);
	if (hr == S_OK) {
		pInfo->dwFlags |= DOCHOSTUIFLAG_DIALOG | DOCHOSTUIFLAG_DISABLE_HELP_MENU | DOCHOSTUIFLAG_NO3DBORDER |
						  DOCHOSTUIFLAG_DISABLE_SCRIPT_INACTIVE | DOCHOSTUIFLAG_ENABLE_FORMS_AUTOCOMPLETE |
						  DOCHOSTUIFLAG_THEME | DOCHOSTUIFLAG_LOCAL_MACHINE_ACCESS_CHECK |
						  DOCHOSTUIFLAG_DISABLE_UNTRUSTEDPROTOCOL;
	}

	return hr;
}

// FUNCTION: ROBLOX 0x0042d7d0
void CLuaHtmlView::Binder::PropBinding::FUN_0042d7d0(const std::string& value)
{
	m_unk0x08->setStringValue(m_unk0x00.get(), value);
}

// FUNCTION: ROBLOX 0x0042d810
std::string CLuaHtmlView::Binder::PropBinding::FUN_0042d810()
{
	return m_unk0x08->getStringValue(m_unk0x00.get());
}

// FUNCTION: ROBLOX 0x0042d850
HRESULT CLuaHtmlView::OnGetExternal(LPDISPATCH* lppDispatch)
{
	return m_unk0xf4.CopyTo(lppDispatch);
}

// FUNCTION: ROBLOX 0x0042dbb0
void CLuaHtmlView::OnDestroy()
{
	CSecureHtmlView::OnDestroy();

	if (m_unk0x104) {
		RBX::DataModel::Lock lock(m_unk0xfc);
		m_unk0x104->m_unk0x104 = NULL;
		m_unk0x104->setParent(NULL);
	}
}

// FUNCTION: ROBLOX 0x0042dd20
bool FUN_0042dd20(IUnknown* p_unk0x04, IUnknown* p_unk0x08)
{
	CComPtr<IUnknown> unknown1;
	CComPtr<IUnknown> unknown2;
	CComPtr<IObjectIdentity> identity;

	if (p_unk0x04 == p_unk0x08) {
		return true;
	}

	if (p_unk0x04 != NULL && p_unk0x08 != NULL) {
		p_unk0x04->QueryInterface(IID_IUnknown, (void**) &unknown1);
		p_unk0x08->QueryInterface(IID_IUnknown, (void**) &unknown2);

		if (unknown1 == unknown2) {
			return true;
		}

		if (unknown1 != NULL && unknown2 != NULL) {
			HRESULT hr = unknown1->QueryInterface(IID_IObjectIdentity, (void**) &identity);
			if (SUCCEEDED(hr) && identity != NULL) {
				hr = identity->IsEqualObject(unknown2);
				if (hr == S_OK) {
					return true;
				}
			}
		}
	}

	return false;
}

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

// FUNCTION: ROBLOX 0x0042e330
void CLuaHtmlView::FUN_0042e330(LPCTSTR html)
{
	CComQIPtr<IHTMLDocument2> document = GetHtmlDocument();
	if (!document) {
		throw std::runtime_error("The HtmlDocument is not loaded");
	}

	CComPtr<IHTMLElement> body;
	document->get_body(&body);
	if (!body) {
		throw std::runtime_error("The HtmlDocument does not contain a body");
	}

	body->put_innerHTML(CComBSTR(html));
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

// FUNCTION: ROBLOX 0x0042ebf0
void CLuaHtmlView::Binder::FUN_0042ebf0()
{
	m_unk0x18 = false;

	std::string value;
	std::list<boost::shared_ptr<PropBinding> >::const_iterator iter = m_unk0x0c.begin();
	std::list<boost::shared_ptr<PropBinding> >::const_iterator end = m_unk0x0c.end();
	if (iter != end) {
		value = (*iter)->FUN_0042d810();
		++iter;
	}

	while (iter != end) {
		if (value != (*iter)->FUN_0042d810()) {
			value.clear();
			break;
		}

		++iter;
	}

	m_unk0x20->put_value(CComBSTR(value.c_str()));
}

// FUNCTION: ROBLOX 0x0042ef30
HRESULT FUN_0042ef30(CComObjectNoLock<CLuaFunction>** p_unk0x04, void* p_unk0x08)
{
	HRESULT result = E_OUTOFMEMORY;
	CComObjectNoLock<CLuaFunction>* object = NULL;

	ATLTRY(object = new CComObjectNoLock<CLuaFunction>(p_unk0x08))

	if (object != NULL) {
		object->SetVoid(p_unk0x08);
		object->InternalFinalConstructAddRef();
		result = object->FinalConstruct();
		object->InternalFinalConstructRelease();
	}

	*p_unk0x04 = object;
	return result;
}

// FUNCTION: ROBLOX 0x0042f1b0
void CLuaHtmlView::FUN_0042f1b0(boost::shared_ptr<DHTMLWindow> window)
{
	m_unk0x104 = window;
	m_unk0xfc = RBX::shared_from_dynamic_cast<RBX::DataModel, RBX::Instance>(window->getRootAncestor());

	if (FUN_0042ef30(&m_unk0xf8, NULL)) {
		throw std::runtime_error("CLuaHtmlView failed to create CLuaFunction");
	}

	m_unk0xf8->m_unk0x0c = m_unk0xfc;
	m_unk0xf4 = m_unk0xf8;
}

// FUNCTION: ROBLOX 0x0042f370
void CLuaHtmlView::FUN_0042f370()
{
	RBX::DataModel::Lock lock(m_unk0xfc);
	m_unk0x10c.clear();
}

// FUNCTION: ROBLOX 0x0042f530
CLuaHtmlView::CLuaHtmlView() : m_unk0xf8(NULL)
{
	m_unk0xf0 = 1;
}

// FUNCTION: ROBLOX 0x0042f5d0
CLuaHtmlView::~CLuaHtmlView()
{
}

// FUNCTION: ROBLOX 0x0042f6e0
bool CLuaHtmlView::Binder::FUN_0042f6e0()
{
	CComBSTR value;
	m_unk0x20->get_value(&value);

	std::string text = (LPCTSTR) CString(value);
	std::for_each(m_unk0x0c.begin(), m_unk0x0c.end(), boost::bind(&PropBinding::FUN_0042d7d0, _1, boost::cref(text)));
	return true;
}

// FUNCTION: ROBLOX 0x005df250 FOLDED
bool FUN_005df250()
{
	return true;
}

// FUNCTION: ROBLOX 0x0042fe90
CLuaHtmlView::Binder::Binder(IHTMLInputElement* element) : m_unk0x18(false), m_unk0x20(element)
{
	m_unk0x1c = new EventHandler(boost::bind(&Binder::FUN_0042f6e0, this));

	CComQIPtr<IHTMLElement2> element2(element);
	VARIANT_BOOL result;
	if (element2->attachEvent(CComBSTR("onchange"), m_unk0x1c, &result)) {
		throw std::runtime_error("attachEvent failed");
	}

	if (!result) {
		throw std::runtime_error("attachEvent failed");
	}
}

// FUNCTION: ROBLOX 0x00430200
CLuaHtmlView::Binder::~Binder()
{
	m_unk0x1c->m_unk0x08 = &FUN_005df250;

	CComQIPtr<IHTMLElement2> element2(m_unk0x20);
	element2->detachEvent(CComBSTR("onchange"), m_unk0x1c);
}

// FUNCTION: ROBLOX 0x00430750
void CLuaHtmlView::Binder::FUN_00430750()
{
	if (!m_unk0x18) {
		g_unk0x00952f1c->FUN_00418fd0(boost::bind(&Binder::FUN_0042ebf0, shared_from_this()));
		m_unk0x18 = true;
	}
}

// FUNCTION: ROBLOX 0x00430940
void FUN_00430940(
	const RBX::Reflection::PropertyDescriptor* p_unk0x04,
	const RBX::Reflection::PropertyDescriptor* p_unk0x08,
	CLuaHtmlView::Binder* binder
)
{
	if (p_unk0x04 == p_unk0x08) {
		binder->FUN_00430750();
	}
}

// FUNCTION: ROBLOX 0x004309f0
CLuaHtmlView::Binder::PropBinding::PropBinding(
	boost::shared_ptr<RBX::Instance> instance, const RBX::Reflection::PropertyDescriptor* descriptor, Binder* binder
)
	: m_unk0x00(instance), m_unk0x08(descriptor)
{
	m_unk0x0c = RBX::Instance::event_propertyChanged.connect(
		instance.get(), boost::bind(&FUN_00430940, descriptor, _1, binder), 10
	);
}

// FUNCTION: ROBLOX 0x00430b30
void CLuaHtmlView::Binder::FUN_00430b30(
	boost::shared_ptr<RBX::Instance> instance, const RBX::Reflection::PropertyDescriptor* descriptor
)
{
	m_unk0x0c.push_back(boost::shared_ptr<PropBinding>(new PropBinding(instance, descriptor, this)));
	FUN_0042ebf0();
}

// STUB: ROBLOX 0x00430c90
void CLuaHtmlView::FUN_00430c90(
	boost::shared_ptr<RBX::Instance> instance, const RBX::Reflection::PropertyDescriptor* descriptor, LPCTSTR name
)
{
	CComQIPtr<IHTMLDocument2> document = GetHtmlDocument();
	if (!document) {
		throw std::runtime_error("The HtmlDocument is not loaded");
	}

	CComPtr<IHTMLElementCollection> all;
	document->get_all(&all);

	CComPtr<IDispatch> dispatch;
	all->item(CComVariant(name), CComVariant(), &dispatch);
	if (!dispatch) {
		throw std::runtime_error(G3D::format("%s not found", name));
	}

	CComQIPtr<IHTMLInputElement> input(dispatch);
	if (!input) {
		throw std::runtime_error(G3D::format("%s is not an input element", name));
	}

	Binder* binder = NULL;
	std::list<boost::shared_ptr<Binder> >::iterator end = m_unk0x10c.end();
	for (std::list<boost::shared_ptr<Binder> >::iterator iter = m_unk0x10c.begin(); iter != end; ++iter) {
		if (FUN_0042dd20((*iter)->m_unk0x20, input)) {
			binder = (*iter).get();
			break;
		}
	}

	if (binder == NULL) {
		RBX::DataModel::Lock lock(m_unk0xfc);
		binder = new Binder(input);
		m_unk0x10c.push_back(boost::shared_ptr<Binder>(binder));
	}

	binder->FUN_00430b30(instance, descriptor);
}

// FUNCTION: ROBLOX 0x00431070
void CLuaHtmlView::OnDocumentComplete(LPCTSTR lpszURL)
{
	CSecureHtmlView::OnDocumentComplete(lpszURL);

	if (m_unk0x104) {
		RBX::DataModel::Lock lock(m_unk0xfc);
		DHTMLWindow::g_unk0x00953120.fire(m_unk0x104.get(), lpszURL);
	}
}
