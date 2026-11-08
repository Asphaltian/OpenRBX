#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "IObjectSafetyRobloxImpl.h"
#include "Roblox_h.h"
#include "decomp.h"
#include "util/Events.h"
#include "util/RunStateOwner.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Selection.h"
#include "v8datamodel/Workspace.h"

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <boost/shared_ptr.hpp>
#include <iosfwd>
#include <memory>

// SIZE 0x34
class __declspec(novtable) CWorkspace : public IDispatchImpl<IWorkspace, &IID_IWorkspace, &LIBID_RobloxLib, 1, 2>,
										public CComObjectRootEx<CComSingleThreadModel>,
										public CComCoClass<CWorkspace, &CLSID_Workspace>,
										public ISupportErrorInfoImpl<&IID_IWorkspace>,
										public IObjectSafetyRobloxImpl<CWorkspace, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
										public IObjectWithSiteImpl<CWorkspace>,
										public RBX::Listener<RBX::Selection, RBX::SelectionChanged>,
										public RBX::Listener<RBX::Workspace, RBX::DrawChanged>
{
public:
	CWorkspace();
	~CWorkspace();

	DECLARE_REGISTRY_RESOURCEID(106)

	// clang-format off
	BEGIN_COM_MAP(CWorkspace)
		COM_INTERFACE_ENTRY(IWorkspace)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()
	// clang-format on

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	void FinalRelease();

	STDMETHOD(SaveToFile)(BSTR pathName);
	STDMETHOD(SaveSelectionToFile)(BSTR pathName);
	STDMETHOD(Insert)(BSTR url);
	STDMETHOD(Write)(IContent** result);
	STDMETHOD(WriteSelection)(IContent** result);
	STDMETHOD(ExecScript)(BSTR script, VARIANT arg1, VARIANT arg2, VARIANT arg3, VARIANT arg4, SAFEARRAY** result);
	STDMETHOD(ExecUrlScript)(BSTR url, VARIANT arg1, VARIANT arg2, VARIANT arg3, VARIANT arg4, SAFEARRAY** result);
	STDMETHOD(Close)();

	HRESULT FinalConstruct();

	// FUNCTION: ROBLOX 0x0046edc0
	boost::shared_ptr<RBX::DataModel> FUN_0046edc0() const { return m_unk0x24; }

	// FUNCTION: ROBLOX 0x0046edf0
	RBX::Workspace* FUN_0046edf0() const { return m_unk0x24 ? m_unk0x24->getWorkspace() : NULL; }

	void FUN_0046ef00(std::istream& stream);

protected:
	virtual void onEvent(const RBX::Selection* source, RBX::SelectionChanged event);
	virtual void onEvent(const RBX::Workspace* source, RBX::DrawChanged event);

private:
	std::auto_ptr<XmlElement> FUN_0046f040();
	void FUN_0046f420();
	HRESULT FUN_004700b0(const XmlElement* root, std::ostream& stream);

	boost::shared_ptr<RBX::DataModel> m_unk0x24;  // 0x24
	boost::shared_ptr<RBX::RunService> m_unk0x2c; // 0x2c
};

DECOMP_SIZE_ASSERT(CWorkspace, 0x34)

#endif // WORKSPACE_H
