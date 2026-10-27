#include "network/API.h"

#include "network/Client.h"
#include "network/NetworkSettings.h"
#include "network/Player.h"
#include "network/Players.h"
#include "network/Server.h"
#include "util/Name.h"

namespace RBX {
namespace Network {

std::string API::version;

// FUNCTION: WEBSERVICE 0x1018c370
void API::init(const char* version)
{
	API::version = version;

	Client::classDescriptor();
	Server::classDescriptor();
	Player::classDescriptor();
	Players::classDescriptor();
	GlobalSettings::classDescriptor();
	NetworkSettings::classDescriptor();

	NetworkSettings::singleton();
}

} // namespace Network
} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::Network::sClient>();
template const RBX::Name& RBX::Name::doDeclare<RBX::Network::sServer>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sNetworkSettings>();
template void RBX::Name::callDoDeclare<RBX::Network::sClient>();
template void RBX::Name::callDoDeclare<RBX::Network::sServer>();
template void RBX::Name::callDoDeclare<RBX::sNetworkSettings>();

template const RBX::FactoryProduct<RBX::Network::Client, RBX::Network::Peer, RBX::Network::sClient>::Creator
	RBX::FactoryProduct<RBX::Network::Client, RBX::Network::Peer, RBX::Network::sClient>::creator;
template const RBX::FactoryProduct<RBX::Network::Server, RBX::Network::Peer, RBX::Network::sServer>::Creator
	RBX::FactoryProduct<RBX::Network::Server, RBX::Network::Peer, RBX::Network::sServer>::creator;
