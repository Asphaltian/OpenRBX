#include "DHTMLWindow.h"

#include "Marshaller.h"
#include "reflection/reflection.h"
#include "util/StandardOut.h"

#include <boost/bind.hpp>
#include <exception>

char sDHTMLWindowService[] = "HtmlService";
char sDHTMLWindow[] = "HtmlWindow";

template const RBX::FactoryProduct<DHTMLWindowService, RBX::Instance, sDHTMLWindowService>::Creator
	RBX::FactoryProduct<DHTMLWindowService, RBX::Instance, sDHTMLWindowService>::creator;

// GLOBAL: ROBLOX 0x00952f1c
Marshaller* g_unk0x00952f1c;

static RBX::Reflection::BoundFuncDesc<DHTMLWindow, void(std::string), 1> g_unk0x00952f60(
	&DHTMLWindow::FUN_0041d1f0, "Navigate", "url", "about:blank", RBX::Reflection::FunctionDescriptor::AnyCaller
);

static RBX::Reflection::BoundFuncDesc<DHTMLWindow, void(int, int), 2> g_unk0x00953008(
	&DHTMLWindow::FUN_0041d0a0, "Resize", "x", "y", RBX::Reflection::FunctionDescriptor::AnyCaller
);

static RBX::Reflection::BoundFuncDesc<DHTMLWindow, void(), 0> g_unk0x00953048(
	&DHTMLWindow::FUN_0041c1d0, "Show", RBX::Reflection::FunctionDescriptor::AnyCaller
);

static RBX::Reflection::BoundFuncDesc<DHTMLWindow, void(), 0> g_unk0x00953078(
	&DHTMLWindow::FUN_00413f40, "Close", RBX::Reflection::FunctionDescriptor::AnyCaller
);

static RBX::Reflection::BoundFuncDesc<DHTMLWindow, void(), 0> g_unk0x009530f0(
	&DHTMLWindow::FUN_0041dc70, "Unbind", RBX::Reflection::FunctionDescriptor::AnyCaller
);

RBX::Reflection::SignalDesc<DHTMLWindow, void(std::string)> DHTMLWindow::g_unk0x00953120("DocumentComplete", "url");

// GLOBAL: ROBLOX 0x00953144
std::map<DWORD, Marshaller*> Marshaller::g_unk0x00953144;

// GLOBAL: ROBLOX 0x00953150
boost::recursive_mutex Marshaller::g_unk0x00953150;

// STUB: ROBLOX 0x00418660
LRESULT Marshaller::FUN_00418660(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	try {
		boost::function<void()> function;
		{
			boost::mutex::scoped_lock lock(m_unk0x40);
			function = m_unk0x2c.front();
			m_unk0x2c.pop_front();
		}
		function();
	}
	catch (std::exception& e) {
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_ERROR, e);
	}

	return 0;
}

// FUNCTION: ROBLOX 0x00419ee0
Marshaller::Marshaller(DWORD threadId) : m_unk0x24(0)
{
	m_unk0x28 = threadId;
	Create(NULL, NULL, NULL, WS_POPUP);
}

// FUNCTION: ROBLOX 0x0041a4a0
Marshaller* Marshaller::FUN_0041a4a0()
{
	boost::recursive_mutex::scoped_lock lock(g_unk0x00953150);
	DWORD threadId = GetCurrentThreadId();
	std::map<DWORD, Marshaller*>::iterator it = g_unk0x00953144.find(threadId);
	if (it != g_unk0x00953144.end()) {
		++it->second->m_unk0x24;
		return it->second;
	}

	Marshaller* marshaller = new Marshaller(threadId);
	g_unk0x00953144[threadId] = marshaller;
	++marshaller->m_unk0x24;
	return marshaller;
}

// FUNCTION: ROBLOX 0x0041ac10
void FUN_0041ac10()
{
	g_unk0x00952f1c = Marshaller::FUN_0041a4a0();
}

// FUNCTION: ROBLOX 0x0041bec0
DHTMLWindowService::DHTMLWindowService()
{
	setName("WindowService");
}

// FUNCTION: ROBLOX 0x0041bfc0
DHTMLWindow::DHTMLWindow() : m_unk0x104(NULL)
{
	setName("Window");
}

// FUNCTION: ROBLOX 0x0041c1d0
void DHTMLWindow::FUN_0041c1d0()
{
	g_unk0x00952f1c->FUN_00418fd0(boost::bind(&DHTMLWindow::FUN_00415550, RBX::shared_from(this)));
}

// FUNCTION: ROBLOX 0x0041d0a0
void DHTMLWindow::FUN_0041d0a0(int x, int y)
{
	g_unk0x00952f1c->FUN_00418fd0(boost::bind(&DHTMLWindow::FUN_00415670, RBX::shared_from(this), x, y));
}

// FUNCTION: ROBLOX 0x0041d1f0
void DHTMLWindow::FUN_0041d1f0(std::string url)
{
	g_unk0x00952f1c->FUN_00418fd0(boost::bind(&DHTMLWindow::FUN_004157b0, RBX::shared_from(this), url));
}

// FUNCTION: ROBLOX 0x0041dc70
void DHTMLWindow::FUN_0041dc70()
{
	g_unk0x00952f1c->FUN_00418fd0(
		boost::bind(&DHTMLWindow::FUN_004152c0, RBX::shared_from(this), &CLuaHtmlView::FUN_0042f370)
	);
}
