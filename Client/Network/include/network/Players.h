#ifndef NETWORK_PLAYERS_H
#define NETWORK_PLAYERS_H

#include "decomp.h"
#include "network/Player.h"
#include "network/SuperSafeChanged.h"
#include "reflection/reflection.h"
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

// clang-format off
// SYNTHETIC: WEBSERVICE 0x101929c0
// SYNTHETIC: ROBLOX 0x004b2650
// RBX::Network::AbuseReport::AbuseReport
// SYNTHETIC: WEBSERVICE 0x10192a30
// SYNTHETIC: ROBLOX 0x004b26c0
// RBX::Network::AbuseReport::~AbuseReport
// SYNTHETIC: WEBSERVICE 0x101930b0
// SYNTHETIC: ROBLOX 0x004b2fa0
// RBX::Network::AbuseReport::operator=
// clang-format on

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
// SYNTHETIC: ROBLOX 0x004b09a0
// RBX::Network::AbuseReporter::~AbuseReporter
// SYNTHETIC: WEBSERVICE 0x10195460
// SYNTHETIC: ROBLOX 0x004b6770
// RBX::Network::AbuseReporter::data::~data
// SYNTHETIC: WEBSERVICE 0x101954c0
// STUB: ROBLOX 0x004b6890
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
	static Reflection::RefPropDescriptor<Players, Player> propLocalPlayer;

	Players();
	virtual ~Players();

	// SYNTHETIC: WEBSERVICE 0x10193090
	// RBX::Network::Players::`scalar deleting destructor'

	bool superSafeOn() const { return localPlayer ? Player::prop_SuperSafeChat.getValue(localPlayer.get()) : false; }
	boost::shared_ptr<Instance> createLocalPlayer(int userId);

	// FUNCTION: WEBSERVICE 0x10072560
	Player* getLocalPlayer() const { return localPlayer.get(); }
	int getNumPlayers() const;

	// FUNCTION: WEBSERVICE 0x10120bc0
	// FUNCTION: ROBLOX 0x004b05d0
	int numPlayers() const { return players->size(); }

	// FUNCTION: WEBSERVICE 0x10190c40
	int getMaxPlayers() const { return maxPlayers; }

	void setMaxPlayers(int value);

	// FUNCTION: WEBSERVICE 0x10120d00
	// FUNCTION: ROBLOX 0x004b0de0
	boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > getPlayers() { return players.read(); }

	void chat(std::string message);
	void reportAbuse(Player* player, std::string comment);
	std::list<ChatMessage>::const_iterator chatHistory_begin();
	std::list<ChatMessage>::const_iterator chatHistory_end();
	bool canReportAbuse() const;
	void reportAbuse(boost::shared_ptr<Instance> player, std::string comment);

	void setAbuseReportUrl(std::string value);

	bool OnReceive(RakPeerInterface* peer, Packet* packet);
	void setConnection(RakPeerInterface* peer);

	boost::shared_ptr<Instance> playerFromCharacter(boost::shared_ptr<Instance> character);
	boost::shared_ptr<Instance> getPlayerByID(int userID);

	static Player* getPlayerFromCharacter(Instance* character);

	static RBX::ModelInstance* findLocalCharacter(const Instance* context);
	static Player* findLocalPlayer(const Instance* context);

	static bool clientIsPresent(const Instance* context, bool testInDatamodel);
	static bool serverIsPresent(const Instance*, bool);
	static bool frontendProcessing(const Instance* context, bool testInDatamodel);
	static bool backendProcessing(const Instance* context, bool testInDatamodel);

protected:
	virtual bool askAddChild(const Instance* instance) const;
	virtual void onChildAdded(Instance* child);
	virtual void onChildRemoving(Instance* child);

private:
	void addChatMessage(const ChatMessage& message);
};

DECOMP_SIZE_ASSERT(Players, 0x158)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10190e60
// SYNTHETIC: ROBLOX 0x004afe60
// RBX::Network::Players::Plugin::`scalar deleting destructor'
// SYNTHETIC: WEBSERVICE 0x1021f4c0
// RBX::Network::`dynamic initializer for 'propPlayerCount''
// SYNTHETIC: WEBSERVICE 0x1021f4f0
// RBX::Network::`dynamic initializer for 'propPlayerMaxCount''
// SYNTHETIC: WEBSERVICE 0x1021f530
// `dynamic initializer for 'RBX::Network::Players::propLocalPlayer''
// SYNTHETIC: WEBSERVICE 0x1021f560
// RBX::Network::`dynamic initializer for 'func_GetPlayerByID''
// SYNTHETIC: WEBSERVICE 0x1021f590
// RBX::Network::`dynamic initializer for 'funcChat''
// SYNTHETIC: WEBSERVICE 0x1021f5c0
// RBX::Network::`dynamic initializer for 'funcReportAbuse''
// SYNTHETIC: WEBSERVICE 0x1021f5f0
// RBX::Network::`dynamic initializer for 'func_players''
// SYNTHETIC: WEBSERVICE 0x1021f620
// RBX::Network::`dynamic initializer for 'func_GetPlayers''
// SYNTHETIC: WEBSERVICE 0x1021f650
// RBX::Network::`dynamic initializer for 'func_createLocalPlayer''
// SYNTHETIC: WEBSERVICE 0x1021f680
// RBX::Network::`dynamic initializer for 'func_setAbuseReportUrl''
// SYNTHETIC: WEBSERVICE 0x1021f6b0
// RBX::Network::`dynamic initializer for 'func_playerFromCharacterOld''
// SYNTHETIC: WEBSERVICE 0x1021f6e0
// RBX::Network::`dynamic initializer for 'func_playerFromCharacter''
// SYNTHETIC: WEBSERVICE 0x1021f710
// RBX::Network::`dynamic initializer for 'event_PlayerAdded''
// SYNTHETIC: WEBSERVICE 0x1021f730
// RBX::Network::`dynamic initializer for 'event_PlayerRemoving''
// SYNTHETIC: WEBSERVICE 0x10225fa0
// `dynamic atexit destructor for 'RBX::Network::Players::propLocalPlayer''
// SYNTHETIC: WEBSERVICE 0x10225fc0
// RBX::Network::`dynamic atexit destructor for 'propPlayerMaxCount''
// SYNTHETIC: WEBSERVICE 0x10225fe0
// RBX::Network::`dynamic atexit destructor for 'propPlayerCount''
// SYNTHETIC: WEBSERVICE 0x10226000
// RBX::Network::`dynamic atexit destructor for 'func_GetPlayers''
// SYNTHETIC: WEBSERVICE 0x10226010
// RBX::Network::`dynamic atexit destructor for 'func_setAbuseReportUrl''
// SYNTHETIC: WEBSERVICE 0x10226020
// RBX::Network::`dynamic atexit destructor for 'func_playerFromCharacter''
// SYNTHETIC: WEBSERVICE 0x10226030
// RBX::Network::`dynamic atexit destructor for 'funcChat''
// SYNTHETIC: WEBSERVICE 0x10226040
// RBX::Network::`dynamic atexit destructor for 'funcReportAbuse''
// SYNTHETIC: WEBSERVICE 0x10226050
// RBX::Network::`dynamic atexit destructor for 'func_players''
// SYNTHETIC: WEBSERVICE 0x10226060
// RBX::Network::`dynamic atexit destructor for 'func_createLocalPlayer''
// SYNTHETIC: WEBSERVICE 0x10226070
// RBX::Network::`dynamic atexit destructor for 'func_playerFromCharacterOld''
// SYNTHETIC: WEBSERVICE 0x10226080
// RBX::Network::`dynamic atexit destructor for 'event_PlayerRemoving''
// SYNTHETIC: WEBSERVICE 0x10226090
// RBX::Network::`dynamic atexit destructor for 'func_GetPlayerByID''
// SYNTHETIC: WEBSERVICE 0x102260a0
// RBX::Network::`dynamic atexit destructor for 'event_PlayerAdded''
// TEMPLATE: WEBSERVICE 0x10193670
// TEMPLATE: ROBLOX 0x004b4460
// RBX::DescribedNonCreatable<RBX::Network::Players,RBX::Instance,&RBX::Network::sPlayers>::DescribedNonCreatable<RBX::Network::Players,RBX::Instance,&RBX::Network::sPlayers>
// clang-format on

} // namespace Network
} // namespace RBX

#endif // NETWORK_PLAYERS_H
