#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

#include "network/Replicator.h"
#include "placeholder.h"
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

class Server : public DescribedCreatable<Server, Peer, sServer>, public Service
{
	int outgoingPort;
	boost::shared_ptr<Players> players;

	static int lastId;

	class ClientProxy : public Replicator
	{
		boost::shared_ptr<Player> remotePlayer;
		Server* server;

	public:
		ClientProxy(SystemAddress systemAddress, Server* server);
		virtual ~ClientProxy();

		void sendTop();
		virtual PluginReceiveResult OnReceive(RakPeerInterface* peer, Packet* packet);

	protected:
		virtual Player* findTargetPlayer() { return remotePlayer.get(); }

		virtual void onSentMarker(long id);

	private:
		virtual bool remoteDeleteOnDisconnect(const Instance* instance) const
		{
			TopReplConts::const_iterator begin = topReplicationContainers.begin();
			TopReplConts::const_iterator end = topReplicationContainers.end();
			return std::find(begin, end, instance) == end;
		}

		virtual bool canSendItems() { return true; }
	};

public:
	Server();
	virtual ~Server();

	static Reflection::SignalDesc<Server, void(std::string, boost::shared_ptr<Instance>)> event_IncommingConnection;

	static bool serverIsPresent(const Instance* context, bool testInDatamodel);

	void start(int port, int threadSleepTime);
	void stop(int blockDuration);
	int getClientCount();

	virtual XmlElement* write() { return NULL; }

	virtual PluginReceiveResult OnReceive(RakPeerInterface* peer, Packet* packet);

protected:
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

	virtual bool askAddChild(const Instance* instance) const
	{
		return dynamic_cast<const ClientProxy*>(instance) != NULL;
	}
};

} // namespace Network
} // namespace RBX

#endif // NETWORK_SERVER_H
