#ifndef DHTMLWINDOW_H
#define DHTMLWINDOW_H

#include "LuaFunction.h"
#include "LuaHtmlView.h"
#include "decomp.h"
#include "reflection/signal.h"
#include "v8tree/Instance.h"

#include <boost/function.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <string>

class DHTMLWindow;

extern char sDHTMLWindowService[];
extern char sDHTMLWindow[];

// SIZE 0xf8
class DHTMLWindowService : public RBX::DescribedCreatable<DHTMLWindowService, RBX::Instance, sDHTMLWindowService>
{
public:
	DHTMLWindowService();

	virtual bool askAddChild(const RBX::Instance* instance) const;

	boost::shared_ptr<RBX::Instance> FUN_0041cf60(int width, int height);

	// SYNTHETIC: ROBLOX 0x0041a7e0
	// DHTMLWindowService::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(DHTMLWindowService, 0xf8)

// SIZE 0x108
class DHTMLWindow : public RBX::DescribedNonCreatable<DHTMLWindow, RBX::Instance, sDHTMLWindow>
{
public:
	DHTMLWindow();

	// FUNCTION: ROBLOX 0x00413f40
	void FUN_00413f40() { setParent(NULL); }

	// FUNCTION: ROBLOX 0x004148f0
	virtual ~DHTMLWindow() {}

	// FUNCTION: ROBLOX 0x004149a0
	virtual bool askSetParent(const RBX::Instance* instance) const
	{
		return dynamic_cast<const DHTMLWindowService*>(instance) != NULL;
	}

	// SYNTHETIC: ROBLOX 0x004153d0
	// DHTMLWindow::`scalar deleting destructor'

	void FUN_0041c1d0();
	void FUN_0041d0a0(int x, int y);
	void FUN_0041d110(RBX::Lua::FunctionRef function);
	void FUN_0041d1f0(std::string url);
	void FUN_0041d2d0(std::string html);
	void FUN_0041dc70();

	static RBX::Reflection::SignalDesc<DHTMLWindow, void(std::string)> g_unk0x00953120;

protected:
	virtual void onServiceProvider(const RBX::ServiceProvider* oldProvider, const RBX::ServiceProvider* newProvider);

private:
	friend class CLuaHtmlView;
	friend class DHTMLWindowService;

	// FUNCTION: ROBLOX 0x004152c0
	void FUN_004152c0(boost::function<void(CLuaHtmlView*)> function)
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0xf8);
		if (m_unk0x104) {
			function(m_unk0x104);
		}
	}

	// FUNCTION: ROBLOX 0x004153f0
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

	// FUNCTION: ROBLOX 0x00415550
	void FUN_00415550()
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0xf8);
		if (m_unk0x104) {
			m_unk0x104->GetParentFrame()->CenterWindow(NULL);
			m_unk0x104->GetParentFrame()->ShowWindow(SW_SHOW);
		}
	}

	// FUNCTION: ROBLOX 0x004155f0
	void FUN_004155f0()
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0xf8);
		if (m_unk0x104) {
			m_unk0x104->FUN_0042d640();
		}
	}

	// FUNCTION: ROBLOX 0x00415670
	void FUN_00415670(int x, int y)
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0xf8);
		if (m_unk0x104) {
			m_unk0x104->GetParentFrame()->SetWindowPos(NULL, 0, 0, x, y, SWP_NOMOVE | SWP_NOZORDER);
		}
	}

	// FUNCTION: ROBLOX 0x00415710
	void FUN_00415710(RBX::Lua::FunctionRef function)
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0xf8);
		if (m_unk0x104) {
			m_unk0x104->m_unk0xf8->m_unk0x14 = function;
		}
	}

	// FUNCTION: ROBLOX 0x004157b0
	void FUN_004157b0(std::string url)
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0xf8);
		if (m_unk0x104) {
			m_unk0x104->Navigate(url.c_str());
		}
	}

	// FUNCTION: ROBLOX 0x00415860
	void FUN_00415860(std::string html)
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0xf8);
		if (m_unk0x104) {
			m_unk0x104->FUN_0042e330(html.c_str());
		}
	}

	// FUNCTION: ROBLOX 0x00417550
	void FUN_00417550()
	{
		boost::recursive_mutex::scoped_lock lock(m_unk0xf8);
		if (m_unk0x104) {
			m_unk0x104->FUN_0042f1b0(RBX::shared_from(this));
		}
	}

	boost::recursive_mutex m_unk0xf8; // 0xf8
	CLuaHtmlView* m_unk0x104;         // 0x104
};

DECOMP_SIZE_ASSERT(DHTMLWindow, 0x108)

// FUNCTION: ROBLOX 0x00413f10
inline bool DHTMLWindowService::askAddChild(const RBX::Instance* instance) const
{
	return dynamic_cast<const DHTMLWindow*>(instance) != NULL;
}

// clang-format off
// TEMPLATE: ROBLOX 0x00414040
// RBX::NonFactoryProduct<RBX::Instance,&sDHTMLWindow>::NonFactoryProduct<RBX::Instance,&sDHTMLWindow>
// TEMPLATE: ROBLOX 0x00414140
// RBX::Name::doDeclare<&sDHTMLWindowService>
// TEMPLATE: ROBLOX 0x004141c0
// RBX::Name::doDeclare<&sDHTMLWindow>
// TEMPLATE: ROBLOX 0x004147c0
// RBX::Name::callDoDeclare<&sDHTMLWindowService>
// TEMPLATE: ROBLOX 0x004147d0
// RBX::Name::callDoDeclare<&sDHTMLWindow>
// TEMPLATE: ROBLOX 0x00416550
// RBX::NonFactoryProduct<RBX::Instance,&sDHTMLWindow>::getClassName
// TEMPLATE: ROBLOX 0x0041a030
// RBX::Reflection::Described<DHTMLWindowService,&sDHTMLWindowService,RBX::FactoryProduct<DHTMLWindowService,RBX::Instance,&sDHTMLWindowService> >::classDescriptor
// TEMPLATE: ROBLOX 0x0041a5e0
// RBX::Reflection::Described<DHTMLWindow,&sDHTMLWindow,RBX::NonFactoryProduct<RBX::Instance,&sDHTMLWindow> >::classDescriptor
// TEMPLATE: ROBLOX 0x0041a660
// RBX::Reflection::Described<DHTMLWindow,&sDHTMLWindow,RBX::NonFactoryProduct<RBX::Instance,&sDHTMLWindow> >::Described<DHTMLWindow,&sDHTMLWindow,RBX::NonFactoryProduct<RBX::Instance,&sDHTMLWindow> >
// TEMPLATE: ROBLOX 0x0041a730
// RBX::FactoryProduct<DHTMLWindowService,RBX::Instance,&sDHTMLWindowService>::~FactoryProduct<DHTMLWindowService,RBX::Instance,&sDHTMLWindowService>
// TEMPLATE: ROBLOX 0x0041a780
// RBX::FactoryProduct<DHTMLWindowService,RBX::Instance,&sDHTMLWindowService>::FactoryProduct<DHTMLWindowService,RBX::Instance,&sDHTMLWindowService>
// TEMPLATE: ROBLOX 0x0041a980
// RBX::FactoryProduct<DHTMLWindowService,RBX::Instance,&sDHTMLWindowService>::getClassName
// TEMPLATE: ROBLOX 0x0041a9a0
// RBX::Reflection::Described<DHTMLWindowService,&sDHTMLWindowService,RBX::FactoryProduct<DHTMLWindowService,RBX::Instance,&sDHTMLWindowService> >::Described<DHTMLWindowService,&sDHTMLWindowService,RBX::FactoryProduct<DHTMLWindowService,RBX::Instance,&sDHTMLWindowService> >
// TEMPLATE: ROBLOX 0x0041ac20
// RBX::DescribedCreatable<DHTMLWindowService,RBX::Instance,&sDHTMLWindowService>::DescribedCreatable<DHTMLWindowService,RBX::Instance,&sDHTMLWindowService>
// TEMPLATE: ROBLOX 0x0041aca0
// RBX::DescribedNonCreatable<DHTMLWindow,RBX::Instance,&sDHTMLWindow>::DescribedNonCreatable<DHTMLWindow,RBX::Instance,&sDHTMLWindow>
// TEMPLATE: ROBLOX 0x0041c230
// RBX::Creatable<RBX::Instance>::create<DHTMLWindow>
// TEMPLATE: ROBLOX 0x0041c2c0
// RBX::Creatable<RBX::Instance>::create<DHTMLWindowService>
// TEMPLATE: ROBLOX 0x0041c620
// RBX::FactoryProduct<DHTMLWindowService,RBX::Instance,&sDHTMLWindowService>::Creator::create
// SYNTHETIC: ROBLOX 0x007ef590
// `RBX::Reflection::Described<DHTMLWindow,&sDHTMLWindow,RBX::NonFactoryProduct<RBX::Instance,&sDHTMLWindow> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: ROBLOX 0x007ef5a0
// `RBX::Reflection::Described<DHTMLWindowService,&sDHTMLWindowService,RBX::FactoryProduct<DHTMLWindowService,RBX::Instance,&sDHTMLWindowService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// TEMPLATE: ROBLOX 0x0041b7c0
// RBX::Reflection::BoundFuncDesc<DHTMLWindowService,boost::shared_ptr<RBX::Instance> __cdecl(int,int),2>::`scalar deleting destructor'
// TEMPLATE: ROBLOX 0x004b3a30
// RBX::Reflection::BoundFuncDesc<DHTMLWindow,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >),1>::~BoundFuncDesc<DHTMLWindow,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >),1>
// TEMPLATE: ROBLOX 0x004b4700
// RBX::Reflection::BoundFuncDesc<DHTMLWindow,void __cdecl(RBX::Lua::FunctionRef),1>::`scalar deleting destructor'
// TEMPLATE: ROBLOX 0x004bb580
// RBX::Reflection::BoundFuncDesc<DHTMLWindow,void __cdecl(int,int),2>::declareSignature
// SYNTHETIC: ROBLOX 0x007e4180
// `dynamic initializer for 'g_unk0x00952f20''
// SYNTHETIC: ROBLOX 0x007e41c0
// `dynamic initializer for 'g_unk0x00952f60''
// SYNTHETIC: ROBLOX 0x007e4210
// `dynamic initializer for 'g_unk0x00952f98''
// SYNTHETIC: ROBLOX 0x007e4240
// `dynamic initializer for 'g_unk0x00952fd0''
// SYNTHETIC: ROBLOX 0x007e4270
// `dynamic initializer for 'g_unk0x00953008''
// SYNTHETIC: ROBLOX 0x007e42a0
// `dynamic initializer for 'g_unk0x00953048''
// SYNTHETIC: ROBLOX 0x007e42d0
// `dynamic initializer for 'g_unk0x00953078''
// SYNTHETIC: ROBLOX 0x007e4340
// `dynamic initializer for 'g_unk0x009530f0''
// SYNTHETIC: ROBLOX 0x007e4370
// `dynamic initializer for 'DHTMLWindow::g_unk0x00953120''
// SYNTHETIC: ROBLOX 0x007ef5d0
// `dynamic atexit destructor for 'g_unk0x00953078''
// SYNTHETIC: ROBLOX 0x007ef5e0
// `dynamic atexit destructor for 'g_unk0x009530f0''
// SYNTHETIC: ROBLOX 0x007ef600
// `dynamic atexit destructor for 'g_unk0x00952fd0''
// SYNTHETIC: ROBLOX 0x007ef610
// `dynamic atexit destructor for 'g_unk0x00953008''
// SYNTHETIC: ROBLOX 0x007ef620
// `dynamic atexit destructor for 'g_unk0x00953048''
// SYNTHETIC: ROBLOX 0x007ef640
// `dynamic atexit destructor for 'g_unk0x00952f60''
// SYNTHETIC: ROBLOX 0x007ef5f0
// `dynamic atexit destructor for 'g_unk0x00952f98''
// SYNTHETIC: ROBLOX 0x007ef650
// `dynamic atexit destructor for 'g_unk0x00952f20''
// SYNTHETIC: ROBLOX 0x007ef660
// `dynamic atexit destructor for 'DHTMLWindow::g_unk0x00953120''
// clang-format on

void FUN_0041ac10();

#endif // DHTMLWINDOW_H
