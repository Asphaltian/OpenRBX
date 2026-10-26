#include "network/Player.h"
#include "v8datamodel/LocalBackpack.h"

namespace RBX {

const char sLocalBackpack[] = "LocalBackpack";

const char sLocalBackpackItem[] = "LocalBackpackItem";

} // namespace RBX

template boost::shared_ptr<RBX::Network::Player> RBX::shared_from(RBX::Network::Player* r);
template void RBX::Notifier<RBX::Network::Player, RBX::Network::CharacterAdded>::addListener(
	RBX::Listener<RBX::Network::Player, RBX::Network::CharacterAdded>* listener
) const;
template void RBX::Notifier<RBX::Network::Player, RBX::Network::CharacterAdded>::removeListener(
	RBX::Listener<RBX::Network::Player, RBX::Network::CharacterAdded>* listener
) const;
template void RBX::Notifier<RBX::Network::Player, RBX::Network::CharacterRemoving>::addListener(
	RBX::Listener<RBX::Network::Player, RBX::Network::CharacterRemoving>* listener
) const;
template void RBX::Notifier<RBX::Network::Player, RBX::Network::CharacterRemoving>::removeListener(
	RBX::Listener<RBX::Network::Player, RBX::Network::CharacterRemoving>* listener
) const;
