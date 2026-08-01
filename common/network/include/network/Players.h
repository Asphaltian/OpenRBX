#ifndef NETWORK_PLAYERS_H
#define NETWORK_PLAYERS_H

#include "decomp.h"
#include "v8tree/Instance.h"

namespace RBX {
namespace Network {

// SIZE 0x158
class Players : public Instance
{
public:
	void setMaxPlayers(int value);

private:
	undefined m_unk0x0f8[0x154 - 0x0f8]; // 0x0f8
	int maxPlayers;                      // 0x154
};

DECOMP_SIZE_ASSERT(Players, 0x158)

} // namespace Network
} // namespace RBX

#endif // NETWORK_PLAYERS_H
