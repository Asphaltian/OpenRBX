#include "DHTMLWindow.h"

#include "Marshaller.h"
#include "reflection/member.h"
#include "reflection/reflection.h"
#include "util/Name.h"
#include "util/StandardOut.h"

#include <G3D/format.h>
#include <boost/bind.hpp>
#include <exception>
#include <stdexcept>

char sDHTMLWindowService[] = "HtmlService";
char sDHTMLWindow[] = "HtmlWindow";

template const RBX::FactoryProduct<DHTMLWindowService, RBX::Instance, sDHTMLWindowService>::Creator
	RBX::FactoryProduct<DHTMLWindowService, RBX::Instance, sDHTMLWindowService>::creator;

Marshaller* g_unk0x00952f1c;

static RBX::Reflection::BoundFuncDesc<DHTMLWindowService, boost::shared_ptr<RBX::Instance>(int, int), 2>
	g_unk0x00952f20(
		&DHTMLWindowService::FUN_0041cf60,
		"NewWindow",
		"width",
		500,
		"height",
		300,
		RBX::Reflection::FunctionDescriptor::AnyCaller
	);

static RBX::Reflection::BoundFuncDesc<DHTMLWindow, void(std::string), 1> g_unk0x00952f60(
	&DHTMLWindow::FUN_0041d1f0, "Navigate", "url", "about:blank", RBX::Reflection::FunctionDescriptor::AnyCaller
);

static RBX::Reflection::BoundFuncDesc<DHTMLWindow, void(std::string), 1> g_unk0x00952f98(
	&DHTMLWindow::FUN_0041d2d0, "SetBody", "html", RBX::Reflection::FunctionDescriptor::AnyCaller
);

static RBX::Reflection::BoundFuncDesc<DHTMLWindow, void(RBX::Lua::FunctionRef), 1> g_unk0x00952fd0(
	&DHTMLWindow::FUN_0041d110, "SetCallback", "function", RBX::Reflection::FunctionDescriptor::AnyCaller
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

static RBX::Reflection::BoundFuncDesc<DHTMLWindow, void(boost::shared_ptr<RBX::Instance>, std::string, std::string), 3>
	g_unk0x009530a8(
		&DHTMLWindow::FUN_0041d880,
		"Bind",
		"object",
		"property",
		"htmlElementId",
		RBX::Reflection::FunctionDescriptor::AnyCaller
	);

static RBX::Reflection::BoundFuncDesc<DHTMLWindow, void(), 0> g_unk0x009530f0(
	&DHTMLWindow::FUN_0041dc70, "Unbind", RBX::Reflection::FunctionDescriptor::AnyCaller
);

RBX::Reflection::SignalDesc<DHTMLWindow, void(std::string)> DHTMLWindow::g_unk0x00953120("DocumentComplete", "url");

std::map<DWORD, Marshaller*> Marshaller::g_unk0x00953144;

boost::recursive_mutex Marshaller::g_unk0x00953150;

LRESULT Marshaller::FUN_00418660(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	try {
		boost::function0<void> function;
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

Marshaller::Marshaller(DWORD threadId) : m_unk0x24(0)
{
	m_unk0x28 = threadId;
	Create(NULL, NULL, NULL, WS_POPUP);
}

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

void FUN_0041ac10()
{
	g_unk0x00952f1c = Marshaller::FUN_0041a4a0();
}

DHTMLWindowService::DHTMLWindowService()
{
	setName("WindowService");
}

DHTMLWindow::DHTMLWindow() : m_unk0x104(NULL)
{
	setName("Window");
}

void DHTMLWindow::onServiceProvider(const RBX::ServiceProvider* oldProvider, const RBX::ServiceProvider* newProvider)
{
	RBX::Instance::onServiceProvider(oldProvider, newProvider);

	if (oldProvider) {
		g_unk0x00952f1c->FUN_00418fd0(boost::bind(&DHTMLWindow::FUN_004155f0, RBX::shared_from(this)));
	}

	boost::recursive_mutex::scoped_lock lock(m_unk0xf8);
	if (newProvider) {
		g_unk0x00952f1c->FUN_00418fd0(boost::bind(&DHTMLWindow::FUN_00417550, RBX::shared_from(this)));
	}
}

void DHTMLWindow::FUN_0041c1d0()
{
	g_unk0x00952f1c->FUN_00418fd0(boost::bind(&DHTMLWindow::FUN_00415550, RBX::shared_from(this)));
}

boost::shared_ptr<RBX::Instance> DHTMLWindowService::FUN_0041cf60(int width, int height)
{
	boost::shared_ptr<DHTMLWindow> window = RBX::Creatable<RBX::Instance>::create<DHTMLWindow>();
	g_unk0x00952f1c->FUN_00418fd0(boost::bind(&DHTMLWindow::FUN_004153f0, window, width, height));
	window->setParent(this);
	return window;
}

void DHTMLWindow::FUN_0041d0a0(int x, int y)
{
	g_unk0x00952f1c->FUN_00418fd0(boost::bind(&DHTMLWindow::FUN_00415670, RBX::shared_from(this), x, y));
}

void DHTMLWindow::FUN_0041d110(RBX::Lua::FunctionRef function)
{
	g_unk0x00952f1c->FUN_00418fd0(boost::bind(&DHTMLWindow::FUN_00415710, RBX::shared_from(this), function));
}

void DHTMLWindow::FUN_0041d1f0(std::string url)
{
	g_unk0x00952f1c->FUN_00418fd0(boost::bind(&DHTMLWindow::FUN_004157b0, RBX::shared_from(this), url));
}

void DHTMLWindow::FUN_0041d2d0(std::string html)
{
	g_unk0x00952f1c->FUN_00418fd0(boost::bind(&DHTMLWindow::FUN_00415860, RBX::shared_from(this), html));
}

void DHTMLWindow::FUN_0041d880(boost::shared_ptr<RBX::Instance> instance, std::string property, std::string name)
{
	if (!instance) {
		throw std::runtime_error("The object to bind to must not be nil");
	}

	RBX::Reflection::MemberDescriptorContainer<RBX::Reflection::PropertyDescriptor>::Iterator iter =
		instance->findProperty(RBX::Name::lookup(property));
	if (iter == instance->properties_end()) {
		throw std::runtime_error(G3D::format("Instance does not have property %s", property));
	}

	const RBX::Reflection::PropertyDescriptor* descriptor = &(*iter).getDescriptor();
	g_unk0x00952f1c->FUN_00418fd0(
		boost::bind(&DHTMLWindow::FUN_00415910, RBX::shared_from(this), instance, descriptor, name)
	);
}

void DHTMLWindow::FUN_0041dc70()
{
	g_unk0x00952f1c->FUN_00418fd0(
		boost::bind(&DHTMLWindow::FUN_004152c0, RBX::shared_from(this), &CLuaHtmlView::FUN_0042f370)
	);
}
