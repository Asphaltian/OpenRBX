#ifndef DHTMLWINDOW_H
#define DHTMLWINDOW_H

#include "LuaFunction.h"
#include "LuaHtmlView.h"
#include "placeholder.h"
#include "reflection/signal.h"
#include "v8tree/Instance.h"

#include <boost/function.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <string>

class DHTMLWindow;

extern char sDHTMLWindowService[];
extern char sDHTMLWindow[];

class DHTMLWindowService : public RBX::DescribedCreatable<DHTMLWindowService, RBX::Instance, sDHTMLWindowService>
{
public:
	DHTMLWindowService();

	virtual bool askAddChild(const RBX::Instance* instance) const;

	boost::shared_ptr<RBX::Instance> FUN_0041cf60(int width, int height);
};

class DHTMLWindow : public RBX::DescribedNonCreatable<DHTMLWindow, RBX::Instance, sDHTMLWindow>
{
public:
	DHTMLWindow();

	void FUN_00413f40() { setParent(NULL); }

	virtual ~DHTMLWindow() {}

	virtual bool askSetParent(const RBX::Instance* instance) const
	{
		return dynamic_cast<const DHTMLWindowService*>(instance) != NULL;
	}

	void FUN_0041c1d0();
	void FUN_0041d0a0(int x, int y);
	void FUN_0041d110(RBX::Lua::FunctionRef function);
	void FUN_0041d1f0(std::string url);
	void FUN_0041d2d0(std::string html);
	void FUN_0041d880(boost::shared_ptr<RBX::Instance> instance, std::string property, std::string name);
	void FUN_0041dc70();

	static RBX::Reflection::SignalDesc<DHTMLWindow, void(std::string)> g_unk0x00953120;

protected:
	virtual void onServiceProvider(const RBX::ServiceProvider* oldProvider, const RBX::ServiceProvider* newProvider);

private:
	friend class CLuaHtmlView;
	friend class DHTMLWindowService;

	void FUN_004152c0(boost::function<void(CLuaHtmlView*)> function)
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0xf8);
		if (m_unk0x104) {
			function(m_unk0x104);
		}
	}

	void FUN_004153f0(int width, int height)
	{
		CFrameWnd* frame = new CFrameWnd;
		frame->Create(NULL, NULL, WS_OVERLAPPEDWINDOW, CRect(0, 0, width, height), AfxGetMainWnd());
		frame->ModifyStyleEx(WS_EX_CLIENTEDGE, WS_EX_TOOLWINDOW);
		CLuaHtmlView* view = dynamic_cast<CLuaHtmlView*>(XTFuncCreateView(frame, RUNTIME_CLASS(CLuaHtmlView)));
		{
			boost::recursive_mutex::scoped_lock lock(m_unk0xf8);
			m_unk0x104 = view;
		}
		frame->InitialUpdateFrame(NULL, FALSE);
	}

	void FUN_00415550()
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0xf8);
		if (m_unk0x104) {
			m_unk0x104->GetParentFrame()->CenterWindow(NULL);
			m_unk0x104->GetParentFrame()->ShowWindow(SW_SHOW);
		}
	}

	void FUN_004155f0()
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0xf8);
		if (m_unk0x104) {
			m_unk0x104->FUN_0042d640();
		}
	}

	void FUN_00415670(int x, int y)
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0xf8);
		if (m_unk0x104) {
			m_unk0x104->GetParentFrame()->SetWindowPos(NULL, 0, 0, x, y, SWP_NOMOVE | SWP_NOZORDER);
		}
	}

	void FUN_00415710(RBX::Lua::FunctionRef function)
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0xf8);
		if (m_unk0x104) {
			m_unk0x104->m_unk0xf8->m_unk0x14 = function;
		}
	}

	void FUN_004157b0(std::string url)
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0xf8);
		if (m_unk0x104) {
			m_unk0x104->Navigate(url.c_str());
		}
	}

	void FUN_00415860(std::string html)
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0xf8);
		if (m_unk0x104) {
			m_unk0x104->FUN_0042e330(html.c_str());
		}
	}

	void FUN_00415910(
		boost::shared_ptr<RBX::Instance> instance,
		const RBX::Reflection::PropertyDescriptor* descriptor,
		std::string name
	)
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0xf8);
		if (m_unk0x104) {
			m_unk0x104->FUN_00430c90(instance, descriptor, name.c_str());
		}
	}

	void FUN_00417550()
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0xf8);
		if (m_unk0x104) {
			m_unk0x104->FUN_0042f1b0(RBX::shared_from(this));
		}
	}

	boost::recursive_mutex m_unk0xf8;
	CLuaHtmlView* m_unk0x104;
};

inline bool DHTMLWindowService::askAddChild(const RBX::Instance* instance) const
{
	return dynamic_cast<const DHTMLWindow*>(instance) != NULL;
}

void FUN_0041ac10();

#endif // DHTMLWINDOW_H
