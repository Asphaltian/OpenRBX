#ifndef V8DATAMODEL_GLOBALSETTINGS_H
#define V8DATAMODEL_GLOBALSETTINGS_H

#include "decomp.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sGlobalSettings[];

class GlobalSettings
{
public:
	// SIZE 0xf8
	class Item : public NonFactoryProduct<Instance, sGlobalSettings>
	{
	};
};

DECOMP_SIZE_ASSERT(GlobalSettings::Item, 0xf8)

template <class T, const char* sName>
class GlobalSettingsItem : public DescribedCreatable<T, GlobalSettings::Item, sName>
{
};

} // namespace RBX

#endif // V8DATAMODEL_GLOBALSETTINGS_H
