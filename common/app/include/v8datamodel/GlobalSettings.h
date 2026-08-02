#ifndef V8DATAMODEL_GLOBALSETTINGS_H
#define V8DATAMODEL_GLOBALSETTINGS_H

#include "decomp.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sGlobalSettings[];

template <class T, const char* name>
class GlobalSettingsItem : public Instance
{
};

} // namespace RBX

#endif // V8DATAMODEL_GLOBALSETTINGS_H
