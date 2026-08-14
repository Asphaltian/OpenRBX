#ifndef NETWORK_PLAYERS_H
#define NETWORK_PLAYERS_H

#include "decomp.h"
#include "network/Player.h"
#include "network/SuperSafeChanged.h"
#include "util/Events.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <boost/shared_ptr.hpp>

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

// SIZE 0x158
class Players : public Instance,
				public Notifier<Players, ChatMessage>,
				public Notifier<Players, SuperSafeChanged>,
				public Listener<Player, CharacterAdded>,
				public Service
{
public:
	// FUNCTION: WEBSERVICE 0x10190c40
	int getMaxPlayers() const { return maxPlayers; }

	void setMaxPlayers(int value);

	int numPlayers() const { return players->size(); }

	static RBX::ModelInstance* findLocalCharacter(const Instance* context);

private:
	virtual void onEvent(const Player* source, CharacterAdded event);

public:

private:
	undefined m_unk0x12c[0x140 - 0x12c];                             // 0x12c
	CopyOnWrite<std::vector<boost::shared_ptr<Instance> > > players; // 0x140
	undefined m_unk0x148[0x154 - 0x148];                             // 0x148
	int maxPlayers;                                                  // 0x154
};

DECOMP_SIZE_ASSERT(Players, 0x158)

} // namespace Network
} // namespace RBX

#endif // NETWORK_PLAYERS_H
