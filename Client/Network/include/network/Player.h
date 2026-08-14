#ifndef NETWORK_PLAYER_H
#define NETWORK_PLAYER_H

#include "decomp.h"
#include "util/Events.h"
#include "v8datamodel/BrickColor.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>

namespace RBX {
namespace Network {

extern char sPlayer[];

// SIZE 0x8
struct CharacterAdded
{
	const boost::shared_ptr<Instance> character; // 0x00

	CharacterAdded(boost::shared_ptr<Instance> character) : character(character) {}
};

DECOMP_SIZE_ASSERT(CharacterAdded, 0x8)

// SIZE 0x8
struct CharacterRemoving
{
	const boost::shared_ptr<Instance> character; // 0x00

	CharacterRemoving(boost::shared_ptr<Instance> character) : character(character) {}
};

DECOMP_SIZE_ASSERT(CharacterRemoving, 0x8)

// SIZE 0x178
class Player : public Instance, public Notifier<Player, CharacterAdded>, public Notifier<Player, CharacterRemoving>
{
public:
	// FUNCTION: WEBSERVICE 0x100cd080
	BrickColor getTeamColor() const { return teamColor; }

	void setTeamColor(BrickColor value);
	// FUNCTION: WEBSERVICE 0x100cd090
	bool getNeutral() const { return neutral; }

	void setNeutral(bool value);

private:
	undefined m_unk0x128[0x130 - 0x128]; // 0x128
	BrickColor teamColor;                // 0x130
	bool neutral;                        // 0x134
	undefined m_unk0x135[0x178 - 0x135]; // 0x135
};

DECOMP_SIZE_ASSERT(Player, 0x178)

} // namespace Network
} // namespace RBX

#endif // NETWORK_PLAYER_H
