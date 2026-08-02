#include "network/Player.h"

#include "reflection/Property.h"

namespace RBX {
namespace Network {

char sPlayer[] = "Player";

static Reflection::PropDescriptor<Player, BrickColor> prop_teamColor;
static Reflection::PropDescriptor<Player, bool> prop_neutral;

// FUNCTION: WEBSERVICE 0x1018f9c0
void Player::setTeamColor(BrickColor value)
{
	if (value.number != teamColor.number) {
		teamColor = value;
		raisePropertyChanged(prop_teamColor);
	}
}

// FUNCTION: WEBSERVICE 0x1018f9f0
void Player::setNeutral(bool value)
{
	if (value != neutral) {
		neutral = value;
		raisePropertyChanged(prop_neutral);
	}
}

} // namespace Network
} // namespace RBX
