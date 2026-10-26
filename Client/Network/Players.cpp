#include "network/Players.h"

#include "network/Client.h"
#include "network/Server.h"
#include "reflection/property.h"
#include "v8datamodel/ModelInstance.h"

#include <PluginInterface.h>
#include <RakPeerInterface.h>
#include <boost/bind.hpp>

template <class T>
class PluginInterfaceAdapter : public PluginInterface
{
	T* c; // 0x04

public:
	PluginInterfaceAdapter(T* c) : c(c) {}

	virtual PluginReceiveResult OnReceive(RakPeerInterface* peer, Packet* packet);
};

namespace RBX {
namespace Network {

char sPlayers[] = "Players";

static Reflection::PropDescriptor<Players, int> propPlayerMaxCount(
	"MaxPlayers",
	"Data",
	&RBX::Network::Players::getMaxPlayers,
	&RBX::Network::Players::setMaxPlayers
);

// SIZE 0x08
class Players::Plugin : public PluginInterfaceAdapter<Players>
{
public:
	Plugin(Players* players) : PluginInterfaceAdapter<Players>(players) {}
};

// FUNCTION: WEBSERVICE 0x10190c50
bool Players::clientIsPresent(const Instance* context, bool testInDatamodel)
{
	return Client::clientIsPresent(context, testInDatamodel);
}

// FUNCTION: WEBSERVICE 0x10190c60
bool Players::frontendProcessing(const Instance* context, bool testInDatamodel)
{
	const ServiceProvider* serviceProvider = ServiceProvider::findServiceProvider(context);

	return serviceProvider != NULL && !Server::serverIsPresent(context, testInDatamodel);
}

// FUNCTION: WEBSERVICE 0x10190c90
bool Players::backendProcessing(const Instance* context, bool testInDatamodel)
{
	const ServiceProvider* serviceProvider = ServiceProvider::findServiceProvider(context);

	return serviceProvider != NULL && !Client::clientIsPresent(context, testInDatamodel);
}

// FUNCTION: WEBSERVICE 0x10190cc0
bool Players::askAddChild(const Instance* instance) const
{
	return dynamic_cast<const Player*>(instance) != NULL;
}

// FUNCTION: WEBSERVICE 0x10190e80
void Players::setConnection(RakPeerInterface* peer)
{
	if (this->peer) {
		this->peer->DetachPlugin(plugin.get());
	}

	this->peer = peer;

	if (peer) {
		peer->AttachPlugin(plugin.get());
	}
}

// STUB: WEBSERVICE 0x101913e0
void Players::onEvent(const Player* source, CharacterAdded event)
{
	STUB(0x101913e0);
}

// STUB: WEBSERVICE 0x101921d0
void Players::onChildChanged(Instance* instance, const PropertyChanged& event)
{
	STUB(0x101921d0);
}

// FUNCTION: WEBSERVICE 0x10192bb0
Players::~Players()
{
	setConnection(NULL);
}

// FUNCTION: WEBSERVICE 0x101935a0
Player* Players::findLocalPlayer(const Instance* context)
{
	const Players* players = ServiceProvider::find<Players>(context);

	return players ? players->getLocalPlayer() : NULL;
}

// FUNCTION: WEBSERVICE 0x101935c0
RBX::ModelInstance* Players::findLocalCharacter(const Instance* context)
{
	if (Player* player = findLocalPlayer(context)) {
		return player->getCharacter();
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x101943e0
Players::Players() : players(std::vector<boost::shared_ptr<Instance> >()), peer(NULL), maxPlayers(12)
{
	setName("Players");
	plugin.reset(new Plugin(this));
}

// STUB: WEBSERVICE 0x101945e0
void AbuseReporter::add(AbuseReport& r, const std::list<ChatMessage>& chatHistory)
{
	STUB(0x101945e0);
}

// STUB: WEBSERVICE 0x10194680
worker_thread::work_result AbuseReporter::processRequests(boost::shared_ptr<data> _data, std::string abuseUrl)
{
	STUB(0x10194680);
	return worker_thread::done;
}

// FUNCTION: WEBSERVICE 0x101956e0
void Players::setMaxPlayers(int value)
{
	if (value != maxPlayers) {
		maxPlayers = value;
		raisePropertyChanged(propPlayerMaxCount);
	}
}

// STUB: WEBSERVICE 0x10195a30
bool Players::OnReceive(RakPeerInterface* peer, Packet* packet)
{
	STUB(0x10195a30);
	return false;
}

} // namespace Network
} // namespace RBX

// STUB: WEBSERVICE 0x101961a0
template <class T>
PluginReceiveResult PluginInterfaceAdapter<T>::OnReceive(RakPeerInterface* peer, Packet* packet)
{
	return c->OnReceive(peer, packet) ? RR_CONTINUE_PROCESSING : RR_STOP_PROCESSING_AND_DEALLOCATE;
}

namespace RBX {
namespace Network {

// FUNCTION: WEBSERVICE 0x10196310
AbuseReporter::AbuseReporter(std::string abuseUrl) : _data(new data())
{
	requestProcessor.reset(
		new worker_thread(boost::bind(&AbuseReporter::processRequests, _data, abuseUrl), "rbx_abusereporter")
	);
}

} // namespace Network
} // namespace RBX
