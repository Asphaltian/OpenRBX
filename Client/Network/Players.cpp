#include "network/Players.h"

#include "Streaming.h"
#include "network/Client.h"
#include "network/Server.h"
#include "reflection/property.h"
#include "util/StandardOut.h"
#include "v8datamodel/GameSettings.h"
#include "v8datamodel/ModelInstance.h"
#include "v8datamodel/Teams.h"

#include <PluginInterface.h>
#include <RakPeerInterface.h>
#include <algorithm>
#include <boost/bind.hpp>
#include <stdexcept>

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

static Reflection::PropDescriptor<Players, int> propPlayerCount(
	"NumPlayers",
	"Data",
	&RBX::Network::Players::numPlayers,
	0,
	Reflection::PropertyDescriptor::UI
);
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

Reflection::RefPropDescriptor<Players, Player> Players::propLocalPlayer(
	"LocalPlayer",
	"Data",
	&RBX::Network::Players::getLocalPlayer,
	0
);

static Reflection::BoundFuncDesc<Players, boost::shared_ptr<Instance>(int), 1> func_GetPlayerByID(
	&RBX::Network::Players::getPlayerByID,
	"GetPlayerByID",
	"userID",
	Reflection::FunctionDescriptor::NeedTrustedCaller
);
static Reflection::BoundFuncDesc<Players, void(std::string), 1> funcChat(
	&RBX::Network::Players::chat,
	"Chat",
	"message",
	Reflection::FunctionDescriptor::NeedTrustedCaller
);
static Reflection::BoundFuncDesc<Players, void(boost::shared_ptr<Instance>, std::string), 2> funcReportAbuse(
	&RBX::Network::Players::reportAbuse,
	"ReportAbuse",
	"player",
	"message",
	Reflection::FunctionDescriptor::NeedTrustedCaller
);
static Reflection::BoundFuncDesc<Players, boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > >(), 0>
	func_players(&RBX::Network::Players::getPlayers, "players", Reflection::FunctionDescriptor::AnyCaller);
static Reflection::BoundFuncDesc<Players, boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > >(), 0>
	func_GetPlayers(&RBX::Network::Players::getPlayers, "GetPlayers", Reflection::FunctionDescriptor::AnyCaller);
static Reflection::BoundFuncDesc<Players, boost::shared_ptr<Instance>(int), 1> func_createLocalPlayer(
	&RBX::Network::Players::createLocalPlayer,
	"CreateLocalPlayer",
	"userId",
	Reflection::FunctionDescriptor::NeedTrustedCaller
);
static Reflection::BoundFuncDesc<Players, void(std::string), 1> func_setAbuseReportUrl(
	&RBX::Network::Players::setAbuseReportUrl,
	"SetAbuseReportUrl",
	"url",
	Reflection::FunctionDescriptor::NeedTrustedCaller
);
static Reflection::BoundFuncDesc<Players, boost::shared_ptr<Instance>(boost::shared_ptr<Instance>), 1>
	func_playerFromCharacterOld(
		&RBX::Network::Players::playerFromCharacter,
		"playerFromCharacter",
		"character",
		Reflection::FunctionDescriptor::AnyCaller
	);
static Reflection::BoundFuncDesc<Players, boost::shared_ptr<Instance>(boost::shared_ptr<Instance>), 1>
	func_playerFromCharacter(
		&RBX::Network::Players::playerFromCharacter,
		"GetPlayerFromCharacter",
		"character",
		Reflection::FunctionDescriptor::AnyCaller
	);
static Reflection::SignalDesc<Players, void(boost::shared_ptr<Instance>)> event_PlayerAdded("PlayerAdded", "player");
static Reflection::SignalDesc<Players, void(boost::shared_ptr<Instance>)> event_PlayerRemoving(
	"PlayerRemoving",
	"player"
);

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

// FUNCTION: WEBSERVICE 0x101913e0
void Players::onEvent(const Player* source, CharacterAdded event)
{
	const_cast<Player*>(source)->rebuildBackpack();
}

// FUNCTION: WEBSERVICE 0x10191c10
boost::shared_ptr<Instance> Players::getPlayerByID(int userID)
{
	std::vector<boost::shared_ptr<Instance> >::const_iterator iter = players->begin();
	std::vector<boost::shared_ptr<Instance> >::const_iterator end = players->end();

	for (; iter != end; ++iter) {
		if (static_cast<Player*>(iter->get())->getUserID() == userID) {
			return *iter;
		}
	}

	return boost::shared_ptr<Instance>();
}

// FUNCTION: WEBSERVICE 0x10191cd0
boost::shared_ptr<Instance> Players::playerFromCharacter(boost::shared_ptr<Instance> character)
{
	boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > myPlayers = getPlayers();

	std::vector<boost::shared_ptr<Instance> >::const_iterator iter = myPlayers->begin();
	std::vector<boost::shared_ptr<Instance> >::const_iterator end = myPlayers->end();

	for (; iter != end; ++iter) {
		if (static_cast<Player*>(iter->get())->getCharacter() == character.get()) {
			return *iter;
		}
	}

	return boost::shared_ptr<Instance>();
}

// FUNCTION: WEBSERVICE 0x101921d0
void Players::onChildChanged(Instance* instance, const PropertyChanged& event)
{
	if (instance == localPlayer.get() && &event.getDescriptor() == &Player::prop_SuperSafeChat) {
		Notifier<Players, SuperSafeChanged>::raise(SuperSafeChanged(superSafeOn()));
	}
}

// FUNCTION: WEBSERVICE 0x10192860
void AbuseReport::addMessage(const ChatMessage& cm)
{
	int userID = cm.source ? Player::prop_userId.getValue(cm.source.get()) : 0;
	Message message = {userID, cm.message};
	messages.push_back(message);
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

// FUNCTION: WEBSERVICE 0x101935f0
Player* Players::getPlayerFromCharacter(Instance* character)
{
	if (Players* players = ServiceProvider::find<Players>(character)) {
		boost::shared_ptr<Instance> player = players->playerFromCharacter(shared_from(character));
		return static_cast<Player*>(player.get());
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x101943e0
Players::Players() : players(std::vector<boost::shared_ptr<Instance> >()), peer(NULL), maxPlayers(12)
{
	setName("Players");
	plugin.reset(new Plugin(this));
}

// FUNCTION: WEBSERVICE 0x101945e0
void AbuseReporter::add(AbuseReport& r, const std::list<ChatMessage>& chatHistory)
{
	std::for_each(chatHistory.begin(), chatHistory.end(), boost::bind(&AbuseReport::addMessage, boost::ref(r), _1));

	{
		boost::mutex::scoped_lock lock(_data->requestSync);
		_data->queue.push(r);
	}

	requestProcessor->wake();
}

// STUB: WEBSERVICE 0x10194680
worker_thread::work_result AbuseReporter::processRequests(boost::shared_ptr<data> _data, std::string abuseUrl)
{
	STUB(0x10194680);
	return worker_thread::done;
}

// FUNCTION: WEBSERVICE 0x10194b30
void Players::reportAbuse(Player* player, std::string comment)
{
	if (abuseReporter) {
		StandardOut::singleton()->print(
			MESSAGE_INFO,
			"Submitting abuse report from %s",
			player ? player->getName().c_str() : "[[[anonymous]]]"
		);

		AbuseReport r;
		r.submitterID = localPlayer ? localPlayer->getUserID() : 0;
		r.allegedAbuserID = player ? player->getUserID() : 0;
		r.comment = comment;
		abuseReporter->add(r, chatHistory);
	}
	else {
		StandardOut::singleton()->print(MESSAGE_INFO, "Sending abuse report to game server via Raknet");

		if (!peer) {
			throw std::runtime_error("Can't report abuse: Not in a networked game");
		}

		RakNet::BitStream bitStream;
		bitStream << (unsigned char) 'Q';
		bitStream << (localPlayer ? localPlayer->getUserID() : 0);
		bitStream << (player ? player->getUserID() : 0);
		bitStream << comment;

		peer->Send(&bitStream, HIGH_PRIORITY, RELIABLE, 2, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
}

// FUNCTION: WEBSERVICE 0x10194e10
void Players::reportAbuse(boost::shared_ptr<Instance> player, std::string comment)
{
	reportAbuse(dynamic_cast<Player*>(player.get()), comment);
}

// FUNCTION: WEBSERVICE 0x10194ff0
void Players::chat(std::string message)
{
	if (!localPlayer) {
		throw std::runtime_error("No local Player to chat from");
	}

	RakNet::BitStream bitStream;
	bitStream << (unsigned char) 'P';

	Guid::Data id = localPlayer->getGuid().data;

	bitStream << id.scope->toString();
	bitStream << id.index;
	bitStream << message;

	peer->Send(&bitStream, MEDIUM_PRIORITY, RELIABLE, 2, UNASSIGNED_SYSTEM_ADDRESS, true);

	ChatMessage event = {message, localPlayer};
	Notifier<Players, ChatMessage>::raise(event);
}

// FUNCTION: WEBSERVICE 0x101956e0
void Players::setMaxPlayers(int value)
{
	if (value != maxPlayers) {
		maxPlayers = value;
		raisePropertyChanged(propPlayerMaxCount);
	}
}

// FUNCTION: WEBSERVICE 0x10195710
boost::shared_ptr<Instance> Players::createLocalPlayer(int userId)
{
	if (localPlayer) {
		throw std::runtime_error("Local player already exists");
	}

	localPlayer = Creatable<Instance>::create<Player>();
	Player::prop_userId.setValue(localPlayer.get(), userId);
	localPlayer->setParent(this);
	raisePropertyChanged(propLocalPlayer);

	Notifier<Players, SuperSafeChanged>::raise(SuperSafeChanged(superSafeOn()));

	return localPlayer;
}

// FUNCTION: WEBSERVICE 0x10195890
void Players::addChatMessage(const ChatMessage& message)
{
	chatHistory.push_front(message);

	while (chatHistory.size() > GameSettings::singleton().chatHistory) {
		chatHistory.pop_back();
	}

	Notifier<Players, ChatMessage>::raise(message);
	Player::event_Chatted.fire(message.source.get(), message.message, message.destination);
}

// FUNCTION: WEBSERVICE 0x10195a30
bool Players::OnReceive(RakPeerInterface* peer, Packet* packet)
{
	switch (packet->data[0]) {
	case 'P':
		try {
			RakNet::BitStream inBitstream(packet->data, packet->length, false);
			inBitstream.IgnoreBits(8);

			std::string foo;
			inBitstream >> foo;

			Guid::Data id;
			id.scope = &Name::lookup(foo);
			inBitstream >> id.index;

			Instance* player = NULL;
			Instance::lookupByGuid(id, player);

			std::string message;
			inBitstream >> message;

			if (!player) {
				StandardOut::singleton()->print(
					MESSAGE_WARNING,
					"Chat message from unknown Player %s %s",
					packet->systemAddress.ToString(true),
					message.c_str()
				);
			}
			else if (player->getParent() == this) {
				ChatMessage event = {message, shared_from_polymorphic_downcast<Player>(player)};
				addChatMessage(event);

				peer->Send(
					reinterpret_cast<const char*>(packet->data),
					packet->length,
					MEDIUM_PRIORITY,
					RELIABLE_ORDERED,
					2,
					packet->systemAddress,
					true
				);
			}
		}
		catch (std::exception& e) {
			StandardOut::singleton()
				->print(MESSAGE_ERROR, "Players::OnReceive packet %d: %s", packet->data[0], e.what());
		}

		return false;

	case 'Q':
		try {
			if (abuseReporter) {
				RakNet::BitStream inBitstream(packet->data, packet->length, false);
				inBitstream.IgnoreBits(8);

				AbuseReport r;

				inBitstream >> r.submitterID;
				inBitstream >> r.allegedAbuserID;
				inBitstream >> r.comment;

				abuseReporter->add(r, chatHistory);
			}
		}
		catch (std::exception& e) {
			StandardOut::singleton()
				->print(MESSAGE_ERROR, "Players::OnReceive packet %d: %s", packet->data[0], e.what());
		}

		return false;

	default:
		return true;
	}
}

// STUB: WEBSERVICE 0x10195db0
void Players::onChildRemoving(Instance* child)
{
	if (Player* player = dynamic_cast<Player*>(child)) {
		boost::shared_ptr<std::vector<boost::shared_ptr<Instance> > >& list = players.write();
		list->erase(std::find(list->begin(), list->end(), shared_from(child)));

		if (ServiceProvider::findServiceProvider(this) && !Client::clientIsPresent(this, false)) {
			event_PlayerRemoving.fire(this, shared_from(player));
		}

		stopListening(player, this);
	}
}

// STUB: WEBSERVICE 0x10195f90
void Players::onChildAdded(Instance* child)
{
	if (Player* player = dynamic_cast<Player*>(child)) {
		if (player->getUserID() > 0) {
			boost::shared_ptr<Instance> otherPlayer = getPlayerByID(player->getUserID());

			if (otherPlayer) {
				otherPlayer->setParent(NULL);
			}
		}

		boost::shared_ptr<std::vector<boost::shared_ptr<Instance> > >& list = players.write();
		list->push_back(shared_from(child));

		if (ServiceProvider::findServiceProvider(this) && !Client::clientIsPresent(this, true)) {
			startListening(player, this);
			player->rebuildBackpack();

			if (Teams* teams = ServiceProvider::find<Teams>(this)) {
				teams->assignNewPlayerToTeam(player);
			}

			event_PlayerAdded.fire(this, shared_from(player));
		}
	}
}

} // namespace Network
} // namespace RBX

// FUNCTION: WEBSERVICE 0x101961a0
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

// FUNCTION: WEBSERVICE 0x101964b0
void Players::setAbuseReportUrl(std::string value)
{
	if (value.empty()) {
		abuseReporter.reset();
	}
	else {
		abuseReporter.reset(new AbuseReporter(value));
	}
}

} // namespace Network
} // namespace RBX
