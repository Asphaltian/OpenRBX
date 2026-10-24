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
public:
	GameSettings();

	// SYNTHETIC: WEBSERVICE 0x100de300
	// RBX::GameSettings::`scalar deleting destructor'

	int chatHistory;        // 0x0f8
	int chatScrollLength;   // 0x0fc
	bool soundEnabled;      // 0x100
	bool softwareSound;     // 0x101
	bool animatedCharacter; // 0x102
};

DECOMP_SIZE_ASSERT(GameSettings, 0x104)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021c8d0
// RBX::`dynamic initializer for 'prop_AnimateCharacter''
// SYNTHETIC: WEBSERVICE 0x1021c900
// RBX::`dynamic initializer for 'prop_ChatHistory''
// SYNTHETIC: WEBSERVICE 0x1021c930
// RBX::`dynamic initializer for 'prop_ChatScrollLength''
// SYNTHETIC: WEBSERVICE 0x1021c960
// RBX::`dynamic initializer for 'prop_SoundEnabled''
// SYNTHETIC: WEBSERVICE 0x1021c990
// RBX::`dynamic initializer for 'prop_SoftwareSound''
// SYNTHETIC: WEBSERVICE 0x10223f60
// RBX::`dynamic atexit destructor for 'prop_SoundEnabled''
// SYNTHETIC: WEBSERVICE 0x10223f80
// RBX::`dynamic atexit destructor for 'prop_SoftwareSound''
// SYNTHETIC: WEBSERVICE 0x10223fa0
// RBX::`dynamic atexit destructor for 'prop_ChatScrollLength''
// SYNTHETIC: WEBSERVICE 0x10223fc0
// RBX::`dynamic atexit destructor for 'prop_ChatHistory''
// SYNTHETIC: WEBSERVICE 0x10223fe0
// RBX::`dynamic atexit destructor for 'prop_AnimateCharacter''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_GAMESETTINGS_H
