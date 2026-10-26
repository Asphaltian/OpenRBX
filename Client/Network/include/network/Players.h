#ifndef NETWORK_PLAYERS_H
#define NETWORK_PLAYERS_H

#include "decomp.h"
#include "network/Player.h"
#include "network/SuperSafeChanged.h"
#include "util/Events.h"
#include "util/boost.hpp"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <list>
#include <queue>
#include <string>

class RakPeerInterface;
struct Packet;

namespace RBX {

class ModelInstance;

namespace Network {

extern char sPlayers[];

// SIZE 0x2c
struct ChatMessage
{
	const std::string message;                   // 0x00
	const boost::shared_ptr<Player> source;      // 0x1c
	const boost::shared_ptr<Player> destination; // 0x24
};

DECOMP_SIZE_ASSERT(ChatMessage, 0x2c)

// SIZE 0x30
struct AbuseReport
{
	// SIZE 0x20
	struct Message
	{
		int userID;       // 0x00
		std::string text; // 0x04
	};

	int submitterID;             // 0x00
	int allegedAbuserID;         // 0x04
	std::string comment;         // 0x08
	std::list<Message> messages; // 0x24

	void addMessage(const ChatMessage& cm);
};

DECOMP_SIZE_ASSERT(AbuseReport::Message, 0x20)
DECOMP_SIZE_ASSERT(AbuseReport, 0x30)

// SIZE 0x0c
class AbuseReporter
{
	// SIZE 0x1c
	struct data
	{
		std::queue<AbuseReport> queue; // 0x00
		boost::mutex requestSync;      // 0x14
	};

	boost::shared_ptr<data> _data;                     // 0x00
	boost::scoped_ptr<worker_thread> requestProcessor; // 0x08

public:
	AbuseReporter(std::string abuseUrl);

	void add(AbuseReport& r, const std::list<ChatMessage>& chatHistory);

private:
	static worker_thread::work_result processRequests(boost::shared_ptr<data> _data, std::string abuseUrl);
};

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10191750
// RBX::Network::AbuseReporter::~AbuseReporter
// SYNTHETIC: WEBSERVICE 0x10195460
// RBX::Network::AbuseReporter::data::~data
// SYNTHETIC: WEBSERVICE 0x101954c0
// RBX::Network::AbuseReporter::data::data
// clang-format on

DECOMP_SIZE_ASSERT(AbuseReporter, 0x0c)

// SIZE 0x158
class Players : public DescribedNonCreatable<Players, Instance, sPlayers>,
				public Service,
				public Notifier<Players, ChatMessage>,
				public Notifier<Players, SuperSafeChanged>,
				public Listener<Player, CharacterAdded>
{
	boost::scoped_ptr<AbuseReporter> abuseReporter; // 0x12c
	std::list<ChatMessage> chatHistory;             // 0x130

	class Plugin;

	boost::scoped_ptr<Plugin> plugin;                                // 0x13c
	CopyOnWrite<std::vector<boost::shared_ptr<Instance> > > players; // 0x140
	boost::shared_ptr<Player> localPlayer;                           // 0x148
	RakPeerInterface* peer;                                          // 0x150
	int maxPlayers;                                                  // 0x154

	virtual void onEvent(const Player* source, CharacterAdded event);
	virtual void onChildChanged(Instance* instance, const PropertyChanged& event);

public:
	Players();
	virtual ~Players();

	// SYNTHETIC: WEBSERVICE 0x10193090
	// RBX::Network::Players::`scalar deleting destructor'

	bool superSafeOn() const { return localPlayer ? Player::prop_SuperSafeChat.getValue(localPlayer.get()) : false; }

	// FUNCTION: WEBSERVICE 0x10072560
	Player* getLocalPlayer() const { return localPlayer.get(); }

	int numPlayers() const { return players->size(); }

	// FUNCTION: WEBSERVICE 0x10190c40
	int getMaxPlayers() const { return maxPlayers; }

	void setMaxPlayers(int value);

	boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > getPlayers() { return players.read(); }

	void setAbuseReportUrl(std::string value);

	bool OnReceive(RakPeerInterface* peer, Packet* packet);
	void setConnection(RakPeerInterface* peer);

	boost::shared_ptr<Instance> playerFromCharacter(boost::shared_ptr<Instance> character);
	boost::shared_ptr<Instance> getPlayerByID(int userID);

	static Player* getPlayerFromCharacter(Instance* character);

	static RBX::ModelInstance* findLocalCharacter(const Instance* context);
	static Player* findLocalPlayer(const Instance* context);

	static bool clientIsPresent(const Instance* context, bool testInDatamodel);
	static bool frontendProcessing(const Instance* context, bool testInDatamodel);
	static bool backendProcessing(const Instance* context, bool testInDatamodel);

protected:
	virtual bool askAddChild(const Instance* instance) const;
	virtual void onChildAdded(Instance* child);
	virtual void onChildRemoving(Instance* child);
};

DECOMP_SIZE_ASSERT(Players, 0x158)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10190e60
// RBX::Network::Players::Plugin::`scalar deleting destructor'
// SYNTHETIC: WEBSERVICE 0x1021f4f0
// RBX::Network::`dynamic initializer for 'propPlayerMaxCount''
// SYNTHETIC: WEBSERVICE 0x1021f710
// RBX::Network::`dynamic initializer for 'event_PlayerAdded''
// SYNTHETIC: WEBSERVICE 0x1021f730
// RBX::Network::`dynamic initializer for 'event_PlayerRemoving''
// SYNTHETIC: WEBSERVICE 0x10225fc0
// RBX::Network::`dynamic atexit destructor for 'propPlayerMaxCount''
// SYNTHETIC: WEBSERVICE 0x10226080
// RBX::Network::`dynamic atexit destructor for 'event_PlayerRemoving''
// SYNTHETIC: WEBSERVICE 0x102260a0
// RBX::Network::`dynamic atexit destructor for 'event_PlayerAdded''
// TEMPLATE: WEBSERVICE 0x10193670
// RBX::DescribedNonCreatable<RBX::Network::Players,RBX::Instance,&RBX::Network::sPlayers>::DescribedNonCreatable<RBX::Network::Players,RBX::Instance,&RBX::Network::sPlayers>
// clang-format on

} // namespace Network
} // namespace RBX

#endif // NETWORK_PLAYERS_H
