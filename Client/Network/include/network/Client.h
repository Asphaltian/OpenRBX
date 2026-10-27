#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include "decomp.h"
#include "network/Replicator.h"
#include "reflection/signal.h"
#include "util/Events.h"
#include "v8tree/Service.h"

#include <RakNetTypes.h>
#include <string>

namespace RBX {

class Instance;

namespace Network {

extern char sClient[];

// SIZE 0x118
class Client : public DescribedCreatable<Client, Peer, sClient>,
			   public Service,
			   public Listener<ServiceProvider, Closing>
{
	SystemAddress serverId; // 0x110

public:
	Client();
	virtual ~Client();

	// SYNTHETIC: WEBSERVICE 0x10197060
	// RBX::Network::Client::`scalar deleting destructor'

	static Reflection::SignalDesc<Client, void(std::string, boost::shared_ptr<Instance>)> event_ConnectionAccepted;
	static Reflection::SignalDesc<Client, void(std::string, int)> event_ConnectionFailed;
	static Reflection::SignalDesc<Client, void(std::string)> event_ConnectionRejected;

	void connect(std::string server, int serverPort, int clientPort, int threadSleepTime);
	void disconnect(int blockDuration);

	virtual PluginReceiveResult OnReceive(RakPeerInterface* peer, Packet* packet);

	// FUNCTION: WEBSERVICE 0x10059d20 FOLDED
	virtual XmlElement* write() { return NULL; }

	static bool clientIsPresent(const Instance* context, bool testInDatamodel);

protected:
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
	virtual void onEvent(const ServiceProvider* source, Closing event);
};

DECOMP_SIZE_ASSERT(Client, 0x118)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021f750
// RBX::Network::`dynamic initializer for 'f_connect''
// SYNTHETIC: WEBSERVICE 0x1021f790
// RBX::Network::`dynamic initializer for 'f_disconnect''
// SYNTHETIC: WEBSERVICE 0x1021f7c0
// `dynamic initializer for 'RBX::Network::Client::event_ConnectionAccepted''
// SYNTHETIC: WEBSERVICE 0x1021f7f0
// `dynamic initializer for 'RBX::Network::Client::event_ConnectionRejected''
// SYNTHETIC: WEBSERVICE 0x1021f810
// `dynamic initializer for 'RBX::Network::Client::event_ConnectionFailed''
// SYNTHETIC: WEBSERVICE 0x102260b0
// RBX::Network::`dynamic atexit destructor for 'f_disconnect''
// SYNTHETIC: WEBSERVICE 0x102260c0
// `dynamic atexit destructor for 'RBX::Network::Client::event_ConnectionRejected''
// SYNTHETIC: WEBSERVICE 0x102260d0
// `dynamic atexit destructor for 'RBX::Network::Client::event_ConnectionFailed''
// SYNTHETIC: WEBSERVICE 0x102260e0
// RBX::Network::`dynamic atexit destructor for 'f_connect''
// SYNTHETIC: WEBSERVICE 0x102260f0
// `dynamic atexit destructor for 'RBX::Network::Client::event_ConnectionAccepted''
// clang-format on

} // namespace Network
} // namespace RBX

#endif // NETWORK_CLIENT_H
