#ifndef NETWORK_PLAYERS_H
#define NETWORK_PLAYERS_H

#include "decomp.h"
#include "v8tree/Instance.h"

namespace RBX {
namespace Network {

extern char sPlayers[];

// SIZE 0x158
class Players : public Instance
{
public:
	// FUNCTION: WEBSERVICE 0x10190c40
	int getMaxPlayers() const { return maxPlayers; }

	void setMaxPlayers(int value);

	int numPlayers() const { return players.read()->size(); }

private:
	undefined m_unk0x0f8[0x140 - 0x0f8];                             // 0x0f8
	CopyOnWrite<std::vector<boost::shared_ptr<Instance> > > players; // 0x140
	undefined m_unk0x148[0x154 - 0x148];                             // 0x148
	int maxPlayers;                                                  // 0x154
};

DECOMP_SIZE_ASSERT(Players, 0x158)

} // namespace Network
} // namespace RBX

#endif // NETWORK_PLAYERS_H
