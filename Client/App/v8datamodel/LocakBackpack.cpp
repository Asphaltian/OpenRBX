#include "network/Player.h"
#include "v8datamodel/LocalBackpack.h"

namespace RBX {

const char sLocalBackpack[] = "LocalBackpack";

const char sLocalBackpackItem[] = "LocalBackpackItem";

} // namespace RBX

template boost::shared_ptr<RBX::Network::Player> RBX::shared_from(RBX::Network::Player* r);
