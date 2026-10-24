#include "v8datamodel/GameSettings.h"

#include "reflection/property.h"

namespace RBX {

const char sGameSettings[] = "GameSettings";

Reflection::BoundProp<bool, 1> prop_AnimateCharacter("AnimatedCharacter", "Control", &GameSettings::animatedCharacter);
Reflection::BoundProp<int, 1> prop_ChatHistory("ChatHistory", "Online", &GameSettings::chatHistory);
Reflection::BoundProp<int, 1> prop_ChatScrollLength("ChatScrollLength", "Online", &GameSettings::chatScrollLength);
Reflection::BoundProp<bool, 1> prop_SoundEnabled("SoundEnabled", "Sound", &GameSettings::soundEnabled);
Reflection::BoundProp<bool, 1> prop_SoftwareSound("SoftwareSound", "Sound", &GameSettings::softwareSound);

// FUNCTION: WEBSERVICE 0x100de660
GameSettings::GameSettings()
	: chatHistory(50), chatScrollLength(5), soundEnabled(true), softwareSound(false), animatedCharacter(true)
{
	setName("Game Options");
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::GameSettings, RBX::GlobalSettings::Item, RBX::sGameSettings>;
template class RBX::Reflection::Described<
	RBX::GameSettings,
	RBX::sGameSettings,
	RBX::FactoryProduct<RBX::GameSettings, RBX::GlobalSettings::Item, RBX::sGameSettings> >;
template class RBX::DescribedCreatable<RBX::GameSettings, RBX::GlobalSettings::Item, RBX::sGameSettings>;
