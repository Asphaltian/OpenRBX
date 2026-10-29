#include "network/Client.h"

#include "decomp.h"
#include "network/API.h"
#include "network/Player.h"
#include "network/Players.h"
#include "util/RunStateOwner.h"
#include "util/standardout.h"

#include <MessageIdentifiers.h>
#include <PacketLogger.h>
#include <RakPeer.h>
#include <stdexcept>

namespace RBX {
namespace Network {

char sClient[] = "NetworkClient";

static Reflection::BoundFuncDesc<Client, void(std::string, int, int, int), 4> f_connect(
	&Client::connect,
	"Connect",
	"server",
	"serverPort",
	"clientPort",
	"threadSleepTime",
	30,
	Reflection::FunctionDescriptor::NeedTrustedCaller
);

static Reflection::BoundFuncDesc<Client, void(int), 1> f_disconnect(
	&Client::disconnect, "Disconnect", "blockDuration", 3000, Reflection::FunctionDescriptor::NeedTrustedCaller
);

Reflection::SignalDesc<Client, void(std::string, boost::shared_ptr<Instance>)> Client::event_ConnectionAccepted(
	"ConnectionAccepted", "peer", "replicator"
);

Reflection::SignalDesc<Client, void(std::string)> Client::event_ConnectionRejected("ConnectionRejected", "peer");
Reflection::SignalDesc<Client, void(std::string, int)> Client::event_ConnectionFailed(
	"ConnectionFailed", "peer", "code"
);

// FUNCTION: WEBSERVICE 0x101969f0
void Client::disconnect(int blockDuration)
{
	removeAllChildren();

	rakPeer->CloseConnection(serverId, true);
	rakPeer->Shutdown(blockDuration);
}

// FUNCTION: WEBSERVICE 0x10196a30
void Client::onEvent(const ServiceProvider* source, Closing event)
{
	disconnect(3000);
}

} // namespace Network
} // namespace RBX

class Exposer : protected PacketLogger
{
public:
	// FUNCTION: WEBSERVICE 0x10196a80
	static const char* IDTOString(int id)
	{
		Exposer exposer;
		return exposer.PacketLogger::IDTOString(id);
	}
};

namespace RBX {
namespace Network {

// FUNCTION: WEBSERVICE 0x10196c20
Client::~Client()
{
	rakPeer->CloseConnection(serverId, true);
}

// FUNCTION: WEBSERVICE 0x10196db0
void Client::connect(std::string server, int serverPort, int clientPort, int threadSleepTime)
{
	SocketDescriptor d(clientPort, "");
	if (!rakPeer->Startup(1, threadSleepTime, &d, 1)) {
		throw std::runtime_error("Failed to start network client");
	}

	if (!rakPeer->Connect(server.c_str(), serverPort, API::version.c_str(), API::version.size())) {
		throw std::runtime_error("Failed to connect to server");
	}

	StandardOut::singleton()->print(MESSAGE_INFO, "Connecting to %s:%d", server.c_str(), serverPort);

	updateNetworkSimulator();
}

// FUNCTION: WEBSERVICE 0x10197b90
bool Client::clientIsPresent(const Instance* context, bool testInDatamodel)
{
	const ServiceProvider* serviceProvider = ServiceProvider::findServiceProvider(context);
	return (serviceProvider ? serviceProvider->find<Client>() : NULL) != NULL;
}

// FUNCTION: WEBSERVICE 0x10197bc0
void Client::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	stopListening<ServiceProvider, Closing>(oldProvider, this);

	if (oldProvider) {
		RunService* runService = oldProvider->find<RunService>();
		if (runService) {
			runService->runDisabled = false;
		}

		disconnect(3000);

		Players* players = oldProvider->find<Players>();
		players->setConnection(NULL);
	}

	Peer::onServiceProvider(oldProvider, newProvider);

	if (newProvider) {
		Players* players = newProvider->create<Players>();
		players->setConnection(rakPeer.get());

		RunService* runService = newProvider->find<RunService>();
		if (runService) {
			runService->runDisabled = true;
		}
	}

	startListening<ServiceProvider, Closing>(newProvider, this);
}

// FUNCTION: WEBSERVICE 0x101986d0
Client::Client()
{
	setName(sClient);
	updateLogger();
}

// STUB: WEBSERVICE 0x10198ba0
PluginReceiveResult Client::OnReceive(RakPeerInterface* peer, Packet* packet)
{
	PluginReceiveResult result = Peer::OnReceive(peer, packet);
	if (result != RR_CONTINUE_PROCESSING) {
		return result;
	}

	const SystemAddress& systemAddress = packet->systemAddress;

	switch (packet->data[0]) {
	case ID_CONNECTION_REQUEST_ACCEPTED:
		try {
			StandardOut::singleton()->print(MESSAGE_INFO, "Connection accepted from %s\n", systemAddress.ToString());

			serverId = systemAddress;

			boost::shared_ptr<Replicator> proxy = Creatable<Instance>::create<Replicator>(systemAddress, rakPeer.get());
			proxy->setParent(this);

			event_ConnectionAccepted.fire(this, systemAddress.ToString(), proxy);
		}
		catch (std::exception& e) {
			StandardOut::singleton()->print(
				MESSAGE_ERROR, "Client::OnReceive packet %d: %s", packet->data[0], e.what()
			);
		}
		break;

	case ID_CONNECTION_ATTEMPT_FAILED:
	case ID_NO_FREE_INCOMING_CONNECTIONS:
	case ID_CONNECTION_BANNED:
		try {
			StandardOut::singleton()->print(
				MESSAGE_ERROR,
				"Failed to connect to %s. %s\n",
				systemAddress.ToString(),
				Exposer::IDTOString(packet->data[0])
			);

			event_ConnectionFailed.fire(this, systemAddress.ToString(), packet->data[0]);
		}
		catch (std::exception& e) {
			StandardOut::singleton()->print(
				MESSAGE_ERROR, "Client::OnReceive packet %d: %s", packet->data[0], e.what()
			);
		}
		break;

	case ID_INVALID_PASSWORD:
		try {
			StandardOut::singleton()->print(MESSAGE_ERROR, "Invalid version from %s", systemAddress.ToString());

			event_ConnectionFailed.fire(this, systemAddress.ToString(), packet->data[0]);
			event_ConnectionRejected.fire(this, systemAddress.ToString());
		}
		catch (std::exception& e) {
			StandardOut::singleton()->print(
				MESSAGE_ERROR, "Client::OnReceive packet %d: %s", packet->data[0], e.what()
			);
		}
		break;

	case ID_DISCONNECTION_NOTIFICATION:
	case ID_CONNECTION_LOST:
		serverId = UNASSIGNED_SYSTEM_ADDRESS;
		break;
	}

	return RR_CONTINUE_PROCESSING;
}

} // namespace Network
} // namespace RBX
