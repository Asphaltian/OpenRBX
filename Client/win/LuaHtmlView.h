#ifndef LUAHTMLVIEW_H
#define LUAHTMLVIEW_H

#include "SecureHtmlView.h"
#include "placeholder.h"

#include <atlbase.h>
#include <atlcom.h>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>
#include <list>
#include <mshtml.h>
#include <string>

namespace RBX {
class DataModel;
class Instance;

namespace Reflection {
class PropertyDescriptor;
}
} // namespace RBX

class CLuaFunction;
class DHTMLWindow;
class EventHandler;

class CLuaHtmlView : public CSecureHtmlView
{
	DECLARE_DYNCREATE(CLuaHtmlView)

public:
	class Binder : public boost::enable_shared_from_this<Binder>, public boost::noncopyable
	{
	public:
		class PropBinding
		{
		public:
			PropBinding(
				boost::shared_ptr<RBX::Instance> instance,
				const RBX::Reflection::PropertyDescriptor* descriptor,
				Binder* binder
			);

			void FUN_0042d7d0(const std::string& value);
			std::string FUN_0042d810();

		private:
			boost::shared_ptr<RBX::Instance> m_unk0x00;
			const RBX::Reflection::PropertyDescriptor* m_unk0x08;
			boost::signals::scoped_connection m_unk0x0c;
		};

		Binder(IHTMLInputElement* element);
		virtual ~Binder();

		void FUN_0042ebf0();
		bool FUN_0042f6e0();
		void FUN_00430750();
		void FUN_00430b30(
			boost::shared_ptr<RBX::Instance> instance, const RBX::Reflection::PropertyDescriptor* descriptor
		);

	private:
		friend class CLuaHtmlView;

		std::list<boost::shared_ptr<PropBinding> > m_unk0x0c;
		bool m_unk0x18;
		CComPtr<EventHandler> m_unk0x1c;
		CComPtr<IHTMLInputElement> m_unk0x20;
	};

protected:
	CLuaHtmlView();

public:
	virtual ~CLuaHtmlView();

	virtual void OnInitialUpdate() { CSecureHtmlView::OnInitialUpdate(); }

	virtual HRESULT OnGetHostInfo(DOCHOSTUIINFO* pInfo);
	virtual HRESULT OnGetExternal(LPDISPATCH* lppDispatch);
	virtual void OnDocumentComplete(LPCTSTR lpszURL);

	void FUN_0042d640();
	void FUN_0042e330(LPCTSTR html);
	void FUN_0042f1b0(boost::shared_ptr<DHTMLWindow> window);
	void FUN_0042f370();
	void FUN_00430c90(
		boost::shared_ptr<RBX::Instance> instance, const RBX::Reflection::PropertyDescriptor* descriptor, LPCTSTR name
	);

protected:
	virtual BOOL OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar);

	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

private:
	friend class DHTMLWindow;

	CComPtr<IDispatch> m_unk0xf4;
	CComObjectNoLock<CLuaFunction>* m_unk0xf8;
	boost::shared_ptr<RBX::DataModel> m_unk0xfc;
	boost::shared_ptr<DHTMLWindow> m_unk0x104;
	std::list<boost::shared_ptr<Binder> > m_unk0x10c;
};

#endif // LUAHTMLVIEW_H
