#ifndef NETWORK_PLAYERS_H
#define NETWORK_PLAYERS_H

#include "network/Player.h"
#include "network/SuperSafeChanged.h"
#include "placeholder.h"
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

struct ChatMessage
{
	const std::string message;
	const boost::shared_ptr<Player> source;
	const boost::shared_ptr<Player> destination;
};

struct AbuseReport
{
	struct Message
	{
		int userID;
		std::string text;
	};

	int submitterID;
	int allegedAbuserID;
	std::string comment;
	std::list<Message> messages;

	void addMessage(const ChatMessage& cm);
};

class AbuseReporter
{
	struct data
	{
		std::queue<AbuseReport> queue;
		boost::mutex requestSync;
	};

	boost::shared_ptr<data> _data;
	boost::scoped_ptr<worker_thread> requestProcessor;

public:
	AbuseReporter(std::string abuseUrl);

	void add(AbuseReport& r, const std::list<ChatMessage>& chatHistory);

private:
	static worker_thread::work_result processRequests(boost::shared_ptr<data> _data, std::string abuseUrl);
};

class Players : public DescribedNonCreatable<Players, Instance, sPlayers>,
				public Service,
				public Notifier<Players, ChatMessage>,
				public Notifier<Players, SuperSafeChanged>,
				public Listener<Player, CharacterAdded>
{
	boost::scoped_ptr<AbuseReporter> abuseReporter;
	std::list<ChatMessage> chatHistory;

	class Plugin;

	boost::scoped_ptr<Plugin> plugin;
	CopyOnWrite<std::vector<boost::shared_ptr<Instance> > > players;
	boost::shared_ptr<Player> localPlayer;
	RakPeerInterface* peer;
	int maxPlayers;

	virtual void onEvent(const Player* source, CharacterAdded event);
	virtual void onChildChanged(Instance* instance, const PropertyChanged& event);

public:
	static Reflection::RefPropDescriptor<Players, Player> propLocalPlayer;

	Players();
	virtual ~Players();

	bool superSafeOn() const;
	boost::shared_ptr<Instance> createLocalPlayer(int userId);

	Player* getLocalPlayer() const { return localPlayer.get(); }
	int getNumPlayers() const;

	int numPlayers() const { return players->size(); }

	int getMaxPlayers() const { return maxPlayers; }

	void setMaxPlayers(int value);

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

} // namespace Network
} // namespace RBX

#endif // NETWORK_PLAYERS_H
