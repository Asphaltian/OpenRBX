#ifndef V8DATAMODEL_GLOBALSETTINGS_H
#define V8DATAMODEL_GLOBALSETTINGS_H

#include "decomp.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

namespace RBX {

extern const char sGlobalSettings[];

// SIZE 0x15c
class GlobalSettings : public DescribedNonCreatable<GlobalSettings, ServiceProvider, sGlobalSettings>
{
public:
	// SIZE 0xf8
	class Item : public NonFactoryProduct<Instance, 0>
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
