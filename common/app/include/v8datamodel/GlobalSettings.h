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
// FUNCTION: WEBSERVICE 0x100b7150
// RBX::NonFactoryProduct<RBX::ServiceProvider,&RBX::sGlobalSettings>::NonFactoryProduct<RBX::ServiceProvider,&RBX::sGlobalSettings>
// FUNCTION: WEBSERVICE 0x100b72c0
// RBX::DescribedNonCreatable<RBX::GlobalSettings,RBX::ServiceProvider,&RBX::sGlobalSettings>::DescribedNonCreatable<RBX::GlobalSettings,RBX::ServiceProvider,&RBX::sGlobalSettings>
// SYNTHETIC: WEBSERVICE 0x102226f0
// `RBX::Reflection::Described<RBX::GlobalSettings,&RBX::sGlobalSettings,RBX::NonFactoryProduct<RBX::ServiceProvider,&RBX::sGlobalSettings> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: WEBSERVICE 0x102239a0
// `RBX::GlobalSettings::singleton'::`2'::`dynamic atexit destructor for 'sing''
// clang-format on
// SIZE 0x15c
class GlobalSettings : public DescribedNonCreatable<GlobalSettings, ServiceProvider, sGlobalSettings>
{
public:
	GlobalSettings();

	static boost::recursive_mutex mutex;

	static boost::shared_ptr<GlobalSettings> singleton();

	// SIZE 0xf8
	class Item : public NonFactoryProduct<Instance, static_cast<const char*>(0)>
	{
	};
};

DECOMP_SIZE_ASSERT(GlobalSettings, 0x15c)
DECOMP_SIZE_ASSERT(GlobalSettings::Item, 0xf8)

template <class T, const char* sName>
class GlobalSettingsItem : public DescribedCreatable<T, GlobalSettings::Item, sName>, public Service
{
};

} // namespace RBX

#endif // V8DATAMODEL_GLOBALSETTINGS_H
