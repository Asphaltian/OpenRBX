#ifndef V8DATAMODEL_GAMESETTINGS_H
#define V8DATAMODEL_GAMESETTINGS_H

#include "placeholder.h"
#include "v8datamodel/GlobalSettings.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sGameSettings[];
class GameSettings : public GlobalSettingsItem<GameSettings, sGameSettings>
{
public:
	GameSettings();

	int chatHistory;
	int chatScrollLength;
	bool soundEnabled;
	bool softwareSound;
	bool animatedCharacter;
};

} // namespace RBX

#endif // V8DATAMODEL_GAMESETTINGS_H
