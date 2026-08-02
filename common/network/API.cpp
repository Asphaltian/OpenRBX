#include "network/Client.h"
#include "network/NetworkSettings.h"
#include "network/Server.h"
#include "util/Name.h"

template const RBX::Name& RBX::Name::doDeclare<RBX::Network::sClient>();
template const RBX::Name& RBX::Name::doDeclare<RBX::Network::sServer>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sNetworkSettings>();
template void RBX::Name::callDoDeclare<RBX::Network::sClient>();
template void RBX::Name::callDoDeclare<RBX::Network::sServer>();
template void RBX::Name::callDoDeclare<RBX::sNetworkSettings>();
