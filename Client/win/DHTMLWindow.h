#ifndef DHTMLWINDOW_H
#define DHTMLWINDOW_H

#include "decomp.h"
#include "reflection/signal.h"
#include "v8tree/Instance.h"

#include <boost/thread/recursive_mutex.hpp>
#include <string>

class CLuaHtmlView;
class DHTMLWindow;

extern char sDHTMLWindowService[];
extern char sDHTMLWindow[];

// SIZE 0xf8
class DHTMLWindowService : public RBX::DescribedCreatable<DHTMLWindowService, RBX::Instance, sDHTMLWindowService>
{
public:
	DHTMLWindowService();

	virtual bool askAddChild(const RBX::Instance* instance) const;

	// SYNTHETIC: ROBLOX 0x0041a7e0
	// DHTMLWindowService::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(DHTMLWindowService, 0xf8)

// SIZE 0x108
class DHTMLWindow : public RBX::DescribedNonCreatable<DHTMLWindow, RBX::Instance, sDHTMLWindow>
{
public:
	DHTMLWindow();

	// FUNCTION: ROBLOX 0x004148f0
	virtual ~DHTMLWindow() {}

	// FUNCTION: ROBLOX 0x004149a0
	virtual bool askSetParent(const RBX::Instance* instance) const
	{
		return dynamic_cast<const DHTMLWindowService*>(instance) != NULL;
	}

	// SYNTHETIC: ROBLOX 0x004153d0
	// DHTMLWindow::`scalar deleting destructor'

	static RBX::Reflection::SignalDesc<DHTMLWindow, void(std::string)> g_unk0x00953120;

private:
	friend class CLuaHtmlView;

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
// TEMPLATE: ROBLOX 0x0041c2c0
// RBX::Creatable<RBX::Instance>::create<DHTMLWindowService>
// TEMPLATE: ROBLOX 0x0041c620
// RBX::FactoryProduct<DHTMLWindowService,RBX::Instance,&sDHTMLWindowService>::Creator::create
// SYNTHETIC: ROBLOX 0x007ef590
// `RBX::Reflection::Described<DHTMLWindow,&sDHTMLWindow,RBX::NonFactoryProduct<RBX::Instance,&sDHTMLWindow> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: ROBLOX 0x007ef5a0
// `RBX::Reflection::Described<DHTMLWindowService,&sDHTMLWindowService,RBX::FactoryProduct<DHTMLWindowService,RBX::Instance,&sDHTMLWindowService> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// clang-format on

void FUN_0041ac10();

#endif // DHTMLWINDOW_H
