#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include "network/Replicator.h"
#include "reflection/signal.h"
#include "placeholder.h"
#include "util/Events.h"
#include "v8tree/Service.h"

#include <RakNetTypes.h>
#include <string>

namespace RBX {

class Instance;

namespace Network {

extern char sClient[];

class Client : public DescribedCreatable<Client, Peer, sClient>,
			   public Service,
			   public Listener<ServiceProvider, Closing>
{
	SystemAddress serverId;

public:
	Client();
	virtual ~Client();

	static Reflection::SignalDesc<Client, void(std::string, boost::shared_ptr<Instance>)> event_ConnectionAccepted;
	static Reflection::SignalDesc<Client, void(std::string, int)> event_ConnectionFailed;
	static Reflection::SignalDesc<Client, void(std::string)> event_ConnectionRejected;

	void connect(std::string server, int serverPort, int clientPort, int threadSleepTime);
	void disconnect(int blockDuration);

	virtual PluginReceiveResult OnReceive(RakPeerInterface* peer, Packet* packet);

	virtual XmlElement* write() { return NULL; }

	static bool clientIsPresent(const Instance* context, bool testInDatamodel);

protected:
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
	virtual void onEvent(const ServiceProvider* source, Closing event);
};

} // namespace Network
} // namespace RBX

#endif // NETWORK_CLIENT_H
