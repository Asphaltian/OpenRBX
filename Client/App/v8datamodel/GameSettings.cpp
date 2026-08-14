#include "v8datamodel/GameSettings.h"

namespace RBX {

const char sGameSettings[] = "GameSettings";

} // namespace RBX

template class RBX::FactoryProduct<RBX::GameSettings, RBX::GlobalSettings::Item, RBX::sGameSettings>;
template class RBX::Reflection::Described<
	RBX::GameSettings,
	RBX::sGameSettings,
	RBX::FactoryProduct<RBX::GameSettings, RBX::GlobalSettings::Item, RBX::sGameSettings> >;
template class RBX::DescribedCreatable<RBX::GameSettings, RBX::GlobalSettings::Item, RBX::sGameSettings>;
