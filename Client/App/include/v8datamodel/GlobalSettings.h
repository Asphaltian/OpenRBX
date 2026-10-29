#ifndef V8DATAMODEL_GLOBALSETTINGS_H
#define V8DATAMODEL_GLOBALSETTINGS_H

#include "decomp.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace RBX {

extern const char sGlobalSettings[];

// clang-format off
// TEMPLATE: WEBSERVICE 0x10062fb0
// TEMPLATE: ROBLOX 0x00408c00
// RBX::GlobalSettingsItem<RBX::DebugSettings,&RBX::sDebugSettings>::singleton
// TEMPLATE: WEBSERVICE 0x10065760
// RBX::DescribedNonCreatable<RBX::Stats::Item,RBX::Instance,&RBX::Stats::sStatsItem>::DescribedNonCreatable<RBX::Stats::Item,RBX::Instance,&RBX::Stats::sStatsItem>
// FUNCTION: WEBSERVICE 0x10071970
// FUNCTION: ROBLOX 0x00448330
// RBX::NonFactoryProduct<RBX::Instance,0>::NonFactoryProduct<RBX::Instance,0>
// TEMPLATE: WEBSERVICE 0x1007e890
// TEMPLATE: ROBLOX 0x00450230
// RBX::GlobalSettingsItem<RBX::GameSettings,&RBX::sGameSettings>::singleton
// FUNCTION: WEBSERVICE 0x100b3640
// FUNCTION: ROBLOX 0x004484a0
// RBX::GlobalSettings::Item::Item
// FUNCTION: WEBSERVICE 0x100b36a0
// RBX::GlobalSettings::Item::askAddChild
// TEMPLATE: WEBSERVICE 0x100b3b40
// RBX::GlobalSettingsItem<RBX::DebugSettings,&RBX::sDebugSettings>::~GlobalSettingsItem<RBX::DebugSettings,&RBX::sDebugSettings>
// TEMPLATE: WEBSERVICE 0x100b4fd0
// TEMPLATE: ROBLOX 0x0057cf80
// RBX::GlobalSettingsItem<RBX::DebugSettings,&RBX::sDebugSettings>::GlobalSettingsItem<RBX::DebugSettings,&RBX::sDebugSettings>
// FUNCTION: WEBSERVICE 0x100b7150
// FUNCTION: ROBLOX 0x00582610
// RBX::NonFactoryProduct<RBX::ServiceProvider,&RBX::sGlobalSettings>::NonFactoryProduct<RBX::ServiceProvider,&RBX::sGlobalSettings>
// FUNCTION: WEBSERVICE 0x100b72c0
// FUNCTION: ROBLOX 0x00582750
// RBX::DescribedNonCreatable<RBX::GlobalSettings,RBX::ServiceProvider,&RBX::sGlobalSettings>::DescribedNonCreatable<RBX::GlobalSettings,RBX::ServiceProvider,&RBX::sGlobalSettings>
// TEMPLATE: WEBSERVICE 0x100de240
// RBX::GlobalSettingsItem<RBX::GameSettings,&RBX::sGameSettings>::~GlobalSettingsItem<RBX::GameSettings,&RBX::sGameSettings>
// TEMPLATE: WEBSERVICE 0x100de5a0
// TEMPLATE: ROBLOX 0x005c9880
// RBX::GlobalSettingsItem<RBX::GameSettings,&RBX::sGameSettings>::GlobalSettingsItem<RBX::GameSettings,&RBX::sGameSettings>
// TEMPLATE: WEBSERVICE 0x100e6ef0
// TEMPLATE: ROBLOX 0x00619db0
// RBX::DescribedNonCreatable<RBX::Feature,RBX::Instance,&RBX::sFeature>::DescribedNonCreatable<RBX::Feature,RBX::Instance,&RBX::sFeature>
// TEMPLATE: WEBSERVICE 0x1018b970
// TEMPLATE: ROBLOX 0x004b8330
// RBX::GlobalSettingsItem<RBX::NetworkSettings,&RBX::sNetworkSettings>::singleton
// TEMPLATE: WEBSERVICE 0x1019b4b0
// RBX::GlobalSettingsItem<RBX::NetworkSettings,&RBX::sNetworkSettings>::~GlobalSettingsItem<RBX::NetworkSettings,&RBX::sNetworkSettings>
// TEMPLATE: WEBSERVICE 0x1019b960
// TEMPLATE: ROBLOX 0x004c21e0
// RBX::GlobalSettingsItem<RBX::NetworkSettings,&RBX::sNetworkSettings>::GlobalSettingsItem<RBX::NetworkSettings,&RBX::sNetworkSettings>
// SYNTHETIC: WEBSERVICE 0x102226f0
// `RBX::Reflection::Described<RBX::GlobalSettings,&RBX::sGlobalSettings,RBX::NonFactoryProduct<RBX::ServiceProvider,&RBX::sGlobalSettings> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102239a0
// SYNTHETIC: ROBLOX 0x007f22b0
// `RBX::GlobalSettings::singleton'::`2'::`dynamic atexit destructor for 'sing''
// clang-format on
// SIZE 0x15c
class GlobalSettings : public DescribedNonCreatable<GlobalSettings, ServiceProvider, sGlobalSettings>
{
public:
	GlobalSettings();

	static boost::recursive_mutex mutex;

	static boost::shared_ptr<GlobalSettings> singleton();
	void loadState();
	void saveState();
	void eraseSettingsStore();

	// SIZE 0xf8
	class Item : public NonFactoryProduct<Instance, static_cast<const char*>(0)>
	{
	protected:
		virtual bool askAddChild(const Instance* instance) const { return dynamic_cast<const Item*>(instance) != NULL; }
	};
};

DECOMP_SIZE_ASSERT(GlobalSettings, 0x15c)
DECOMP_SIZE_ASSERT(GlobalSettings::Item, 0xf8)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x100b72a0
// ??_GServiceProvider@RBX@@UAEPAXI@Z
// clang-format on

template <class T, const char* sName>
class GlobalSettingsItem : public DescribedCreatable<T, GlobalSettings::Item, sName>, public Service
{
private:
	static GlobalSettingsItem* sing;

protected:
	GlobalSettingsItem()
	{
		setName(sName);
		sing = this;
	}

	virtual ~GlobalSettingsItem() { sing = NULL; }

public:
	static T& singleton()
	{
		boost::recursive_mutex::scoped_lock lock(GlobalSettings::mutex);
		if (sing == NULL) {
			boost::shared_ptr<T> s = Creatable<Instance>::create<T>();
			s->setParent(GlobalSettings::singleton().get());
		}
		return static_cast<T&>(*sing);
	}
};

template <class T, const char* sName>
GlobalSettingsItem<T, sName>* GlobalSettingsItem<T, sName>::sing = NULL;

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021bd00
// `dynamic initializer for 'RBX::GlobalSettings::mutex''
// SYNTHETIC: WEBSERVICE 0x10223990
// `dynamic atexit destructor for 'RBX::GlobalSettings::mutex''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_GLOBALSETTINGS_H
