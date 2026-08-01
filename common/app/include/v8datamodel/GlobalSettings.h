#ifndef V8DATAMODEL_GLOBALSETTINGS_H
#define V8DATAMODEL_GLOBALSETTINGS_H

#include "decomp.h"
#include "v8tree/Instance.h"

namespace RBX {

class DebugSettings;

extern DebugSettings* sDebugSettings;

template <class T, T*& instance>
class GlobalSettingsItem : public Instance
{
};

} // namespace RBX

#endif // V8DATAMODEL_GLOBALSETTINGS_H
