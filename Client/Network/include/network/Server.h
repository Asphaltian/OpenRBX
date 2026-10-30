#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

#include "decomp.h"
#include "network/Replicator.h"
#include "reflection/signal.h"
#include "v8tree/Service.h"

#include <RakNetTypes.h>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <string>

namespace RBX {

class Instance;

namespace Network {

class Player;
class Players;

extern char sServer[];

// SIZE 0x118
class Server : public DescribedCreatable<Server, Peer, sServer>, public Service
{
	int outgoingPort;                   // 0x10c
	boost::shared_ptr<Players> players; // 0x110

	static int lastId;

	// SIZE 0x2c40
	class ClientProxy : public Replicator
	{
		boost::shared_ptr<Player> remotePlayer; // 0x2c30
		Server* server;                         // 0x2c38

	public:
		ClientProxy(SystemAddress systemAddress, Server* server);
		virtual ~ClientProxy();

		// SYNTHETIC: WEBSERVICE 0x10199db0
		// SYNTHETIC: ROBLOX 0x004bbfa0
		// RBX::Network::Server::ClientProxy::`scalar deleting destructor'

		void sendTop();
		virtual PluginReceiveResult OnReceive(RakPeerInterface* peer, Packet* packet);

	protected:
		// FUNCTION: WEBSERVICE 0x101996e0
		// FUNCTION: ROBLOX 0x004bb930
		virtual Player* findTargetPlayer() { return remotePlayer.get(); }

		virtual void onSentMarker(long id);

	private:
		// FUNCTION: WEBSERVICE 0x1019a0d0
		// FUNCTION: ROBLOX 0x004bc2c0
		virtual bool remoteDeleteOnDisconnect(const Instance* instance) const
		{
			TopReplConts::const_iterator begin = topReplicationContainers.begin();
			TopReplConts::const_iterator end = topReplicationContainers.end();
			return std::find(begin, end, instance) == end;
		}

		// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
		// FUNCTION: ROBLOX 0x005df250 FOLDED
		virtual bool canSendItems() { return true; }
	};

public:
	Server();
	virtual ~Server();

	// SYNTHETIC: WEBSERVICE 0x10199eb0
	// SYNTHETIC: ROBLOX 0x004bc0a0
	// RBX::Network::Server::`scalar deleting destructor'

	static Reflection::SignalDesc<Server, void(std::string, boost::shared_ptr<Instance>)> event_IncommingConnection;

	static bool serverIsPresent(const Instance* context, bool testInDatamodel);

	void start(int port, int threadSleepTime);
	void stop(int blockDuration);
	int getClientCount();

	// FUNCTION: WEBSERVICE 0x10059d20 FOLDED
	// FUNCTION: ROBLOX 0x00407390 FOLDED
	virtual XmlElement* write() { return NULL; }

	virtual PluginReceiveResult OnReceive(RakPeerInterface* peer, Packet* packet);

protected:
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

	// FUNCTION: WEBSERVICE 0x101999f0
	// FUNCTION: ROBLOX 0x004bbbe0
	virtual bool askAddChild(const Instance* instance) const
	{
		return dynamic_cast<const ClientProxy*>(instance) != NULL;
	}
};

DECOMP_SIZE_ASSERT(Server, 0x118)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021f840
// SYNTHETIC: ROBLOX 0x007e62d0
// RBX::Network::`dynamic initializer for 'server_startFunction''
// SYNTHETIC: WEBSERVICE 0x1021f880
// SYNTHETIC: ROBLOX 0x007e6310
// RBX::Network::`dynamic initializer for 'f_disconnect''
// SYNTHETIC: WEBSERVICE 0x1021f8b0
// RBX::Network::`dynamic initializer for 'f_GetClientCount''
// SYNTHETIC: WEBSERVICE 0x1021f8e0
// `dynamic initializer for 'RBX::Network::Server::event_IncommingConnection''
// SYNTHETIC: WEBSERVICE 0x10226100
// SYNTHETIC: ROBLOX 0x007f0f10
// RBX::Network::`dynamic atexit destructor for 'f_disconnect''
// SYNTHETIC: WEBSERVICE 0x10226110
// RBX::Network::`dynamic atexit destructor for 'f_GetClientCount''
// SYNTHETIC: WEBSERVICE 0x10226120
// SYNTHETIC: ROBLOX 0x007f0f30
// RBX::Network::`dynamic atexit destructor for 'server_startFunction''
// SYNTHETIC: WEBSERVICE 0x10226130
// `dynamic atexit destructor for 'RBX::Network::Server::event_IncommingConnection''
// clang-format on

} // namespace Network
} // namespace RBX

#endif // NETWORK_SERVER_H
