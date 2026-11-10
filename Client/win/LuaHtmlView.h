#ifndef LUAHTMLVIEW_H
#define LUAHTMLVIEW_H

#include "SecureHtmlView.h"
#include "decomp.h"

#include <atlbase.h>
#include <atlcom.h>
#include <boost/shared_ptr.hpp>
#include <list>

namespace RBX {
class DataModel;
}

class CLuaFunction;
class DHTMLWindow;

// VTABLE: ROBLOX 0x008040bc
// SIZE 0x118
class CLuaHtmlView : public CSecureHtmlView
{
	DECLARE_DYNCREATE(CLuaHtmlView)

	// SYNTHETIC: ROBLOX 0x0042d630
	// ?GetRuntimeClass@CLuaHtmlView@@UBEPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x0042f8f0
	// ?CreateObject@CLuaHtmlView@@SGPAVCObject@@XZ

public:
	class Binder;

protected:
	CLuaHtmlView();

public:
	virtual ~CLuaHtmlView();

	// SYNTHETIC: ROBLOX 0x0042f960
	// CLuaHtmlView::`scalar deleting destructor'

	// FUNCTION: ROBLOX 0x0042d650
	virtual void OnInitialUpdate() { CSecureHtmlView::OnInitialUpdate(); }

	virtual HRESULT OnGetHostInfo(DOCHOSTUIINFO* pInfo);
	virtual HRESULT OnGetExternal(LPDISPATCH* lppDispatch);
	virtual void OnDocumentComplete(LPCTSTR lpszURL);

	void FUN_0042d640();
	void FUN_0042e330(LPCTSTR html);
	void FUN_0042f1b0(boost::shared_ptr<DHTMLWindow> window);
	void FUN_0042f370();

protected:
	virtual BOOL OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar);

	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

	// SYNTHETIC: ROBLOX 0x0042e750
	// ?GetMessageMap@CLuaHtmlView@@MBEPBUAFX_MSGMAP@@XZ

private:
	friend class DHTMLWindow;

	CComPtr<IDispatch> m_unk0xf4;                     // 0xf4
	CComObjectNoLock<CLuaFunction>* m_unk0xf8;        // 0xf8
	boost::shared_ptr<RBX::DataModel> m_unk0xfc;      // 0xfc
	boost::shared_ptr<DHTMLWindow> m_unk0x104;        // 0x104
	std::list<boost::shared_ptr<Binder> > m_unk0x10c; // 0x10c
};

DECOMP_SIZE_ASSERT(CLuaHtmlView, 0x118)

#endif // LUAHTMLVIEW_H
