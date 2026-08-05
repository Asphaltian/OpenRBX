#ifndef NETWORK_PLAYER_H
#define NETWORK_PLAYER_H

#include "decomp.h"
#include "v8datamodel/BrickColor.h"
#include "v8tree/Instance.h"

namespace RBX {
namespace Network {

extern char sPlayer[];

// SIZE 0x178
class Player : public Instance
{
public:
	// FUNCTION: WEBSERVICE 0x100cd080
	BrickColor getTeamColor() const { return teamColor; }

	void setTeamColor(BrickColor value);
	// FUNCTION: WEBSERVICE 0x100cd090
	bool getNeutral() const { return neutral; }

	void setNeutral(bool value);

private:
	undefined m_unk0x0f8[0x130 - 0x0f8]; // 0x0f8
	BrickColor teamColor;                // 0x130
	bool neutral;                        // 0x134
	undefined m_unk0x135[0x178 - 0x135]; // 0x135
};

DECOMP_SIZE_ASSERT(Player, 0x178)

} // namespace Network
} // namespace RBX

#endif // NETWORK_PLAYER_H
