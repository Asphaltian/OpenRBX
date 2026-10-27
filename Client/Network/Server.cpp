#include "network/Server.h"

#include "PacketIds.h"
#include "decomp.h"
#include "network/API.h"
#include "network/NetworkSettings.h"
#include "network/Player.h"
#include "network/Players.h"
#include "util/StandardOut.h"

#include <BitStream.h>
#include <G3D/format.h>
#include <MessageIdentifiers.h>
#include <RakPeer.h>
#include <algorithm>
#include <stdexcept>

namespace RBX {
namespace Network {

char sServer[] = "NetworkServer";

static Reflection::BoundFuncDesc<Server, void(int, int), 2> server_startFunction(
	&Server::start,
	"Start",
	"port",
	"threadSleepTime",
	30,
	Reflection::FunctionDescriptor::NeedTrustedCaller
);
static Reflection::BoundFuncDesc<Server, void(int), 1> f_disconnect(
	&Server::stop,
	"Stop",
	"blockDuration",
	1000,
	Reflection::FunctionDescriptor::NeedTrustedCaller
);
static Reflection::BoundFuncDesc<Server, int(), 0> f_GetClientCount(
	&Server::getClientCount,
	"GetClientCount",
	Reflection::FunctionDescriptor::NeedTrustedCaller
);
Reflection::SignalDesc<Server, void(std::string, boost::shared_ptr<Instance>)> Server::event_IncommingConnection(
	"IncommingConnection",
	"peer",
	"replicator"
);

// FUNCTION: WEBSERVICE 0x10199280
void Server::ClientProxy::onSentMarker(long id)
{
	sendPhysicsEnabled = true;
}

// FUNCTION: WEBSERVICE 0x10199490
static bool isReplicator(boost::shared_ptr<Instance> instance)
{
	return dynamic_cast<Replicator*>(instance.get()) != NULL;
}

// FUNCTION: WEBSERVICE 0x10199530
void Server::stop(int blockDuration)
{
	if (rakPeer->IsActive()) {
		rakPeer->Shutdown(blockDuration);
	}

	removeAllChildren();
}

// FUNCTION: WEBSERVICE 0x10199570
Server::ClientProxy::~ClientProxy()
{
}

// FUNCTION: WEBSERVICE 0x10199900
Server::~Server()
{
}

// FUNCTION: WEBSERVICE 0x10199ac0
void Server::start(int port, int threadSleepTime)
{
	SocketDescriptor d(port, "");
	if (!rakPeer->Startup(32, threadSleepTime, &d, 1)) {
		throw std::runtime_error("Failed to start network server");
	}

	outgoingPort = port;
	StandardOut::singleton()->print(MESSAGE_INFO, "Starting network server on port %d", port);

	const unsigned int numAddresses = rakPeer->GetNumberOfAddresses();
	StandardOut::singleton()->print(MESSAGE_INFO, "IP addresses:");
	for (unsigned int i = 0; i < numAddresses; ++i) {
		StandardOut::singleton()->print(MESSAGE_INFO, "%s", rakPeer->GetLocalIP(i));
	}

	updateNetworkSimulator();
}

// FUNCTION: WEBSERVICE 0x10199d10
Server::ClientProxy::ClientProxy(SystemAddress systemAddress, Server* server)
	: Replicator(systemAddress, server->peerInterface()), server(server)
{
}

// FUNCTION: WEBSERVICE 0x10199ed0
void Server::ClientProxy::sendTop()
{
	RakNet::BitStream bitStream;
	bitStream << (unsigned char) ID_SET_GLOBALS;

	bitStream << Guid::getLocalScope().toString();

	TopReplConts::iterator end = topReplicationContainers.end();
	for (TopReplConts::iterator iter = topReplicationContainers.begin(); iter != end; ++iter) {
		serializeId(bitStream, *iter);
	}

	writeProperties(topReplicationContainers[0], bitStream);

	peer->Send(&bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, remotePlayerId, false);
}

// FUNCTION: WEBSERVICE 0x1019a140
int Server::getClientCount()
{
	if (getChildren()) {
		return std::count_if(getChildren()->begin(), getChildren()->end(), isReplicator);
	}

	return 0;
}

// STUB: WEBSERVICE 0x1019a330
PluginReceiveResult Server::ClientProxy::OnReceive(RakPeerInterface* peer, Packet* packet)
{
	const SystemAddress& systemAddress = packet->systemAddress;
	if (systemAddress != remotePlayerId) {
		return Replicator::OnReceive(peer, packet);
	}

	switch (packet->data[0]) {
	case ID_REQUEST_CHARACTER:
		try {
			RakNet::BitStream inBitstream(packet->data, packet->length, false);
			inBitstream.IgnoreBits(8);

			Instance* instance;
			Guid::Data id;
			if (deserializeInstanceRef(inBitstream, instance, id)) {
				if (NetworkSettings::singleton().printInstances) {
					StandardOut::singleton()->print(
						MESSAGE_INFO,
						"Received remotePlayer %s from %s",
						this,
						id.readableString(4).c_str(),
						remotePlayerId.ToString()
					);
				}

				remotePlayer = shared_from_dynamic_cast<Player>(instance);

				if (!remotePlayer) {
					throw std::runtime_error("remotePlayer is NULL");
				}

				remotePlayer->loadCharacter();
			}
			else {
				throw std::runtime_error(
					G3D::format(
						"Couldn't resolve remotePlayer %s from %s",
						id.readableString(4).c_str(),
						remotePlayerId.ToString()
					)
				);
			}
		}
		catch (std::exception& e) {
			StandardOut::singleton()
				->print(MESSAGE_ERROR, "Replicator::ClientProxy::OnReceive packet %d: %s", packet->data[0], e.what());
		}
		return RR_STOP_PROCESSING_AND_DEALLOCATE;

	default:
		return Replicator::OnReceive(peer, packet);
	}
}

// FUNCTION: WEBSERVICE 0x1019a7f0
bool Server::serverIsPresent(const Instance* context, bool testInDatamodel)
{
	const ServiceProvider* serviceProvider = ServiceProvider::findServiceProvider(context);
	return (serviceProvider ? serviceProvider->find<Server>() : NULL) != NULL;
}

// FUNCTION: WEBSERVICE 0x1019a820
void Server::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	if (oldProvider) {
		players->setConnection(NULL);
		stop(1000);
		players.reset();
	}

	Peer::onServiceProvider(oldProvider, newProvider);

	if (newProvider) {
		players = shared_from(newProvider->create<Players>());
		players->setConnection(peerInterface());
	}
}

// FUNCTION: WEBSERVICE 0x1019b080
Server::Server() : outgoingPort(0)
{
	setName(sServer);

	rakPeer->SetMaximumIncomingConnections(32);
	rakPeer->SetIncomingPassword(API::version.c_str(), API::version.size());

	updateLogger();
}

// FUNCTION: WEBSERVICE 0x1019b1b0
PluginReceiveResult Server::OnReceive(RakPeerInterface* peer, Packet* packet)
{
	PluginReceiveResult result = Peer::OnReceive(peer, packet);
	if (result != RR_CONTINUE_PROCESSING) {
		return result;
	}

	switch (packet->data[0]) {
	case ID_NEW_INCOMING_CONNECTION:
		try {
			StandardOut::singleton()->print(MESSAGE_INFO, "New connection from %s\n", packet->systemAddress.ToString());

			boost::shared_ptr<ClientProxy> proxy =
				Creatable<Instance>::create<ClientProxy>(packet->systemAddress, this);
			proxy->setParent(this);
			proxy->sendTop();

			event_IncommingConnection.fire(this, packet->systemAddress.ToString(), proxy);
		}
		catch (std::exception& e) {
			StandardOut::singleton()
				->print(MESSAGE_ERROR, "Server::OnReceive packet %d: %s", packet->data[0], e.what());
		}
		break;
	}

	return RR_CONTINUE_PROCESSING;
}

} // namespace Network
} // namespace RBX
