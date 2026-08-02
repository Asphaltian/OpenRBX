#ifndef V8DATAMODEL_GAMESETTINGS_H
#define V8DATAMODEL_GAMESETTINGS_H

#include "decomp.h"
#include "v8datamodel/GlobalSettings.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sGameSettings[];
// SIZE 0x104
class GameSettings : public GlobalSettingsItem<GameSettings, sGameSettings>
{
private:
	undefined m_unk0x0f8[0x104 - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(GameSettings, 0x104)

} // namespace RBX

#endif // V8DATAMODEL_GAMESETTINGS_H
