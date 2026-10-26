#include "network/Player.h"

#include "reflection/property.h"
#include "security/SecurityContext.h"
#include "v8datamodel/Backpack.h"

namespace RBX {
namespace Network {

char sPlayer[] = "Player";

static Reflection::PropDescriptor<Player, BrickColor> prop_teamColor(
	"TeamColor",
	"Team",
	&RBX::Network::Player::getTeamColor,
	&RBX::Network::Player::setTeamColor
);

static Reflection::PropDescriptor<Player, bool> prop_neutral(
	"Neutral",
	"Team",
	&RBX::Network::Player::getNeutral,
	&RBX::Network::Player::setNeutral
);

// FUNCTION: WEBSERVICE 0x1018cfb0
Backpack* Player::getPlayerBackpack() const
{
	return findFirstChildOfType<Backpack>();
}

// STUB: WEBSERVICE 0x1018f870
void Player::setCharacter(ModelInstance* value)
{
	STUB(0x1018f870);
}

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

// STUB: WEBSERVICE 0x1018fb20
void Player::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	STUB(0x1018fb20);
}

// STUB: WEBSERVICE 0x1018fce0
Player::~Player()
{
	setCharacter(NULL);
}

// FUNCTION: WEBSERVICE 0x1018fef0
void Player::setName(const std::string& name)
{
	Security::Context::current().requirePermission(Security::Administrator, "set a Player's name");
	Instance::setName(name);
}

// FUNCTION: WEBSERVICE 0x1018ffd0
Player::Player()
	: teamColor(BrickColor::legoWhite()), neutral(true), under13(false), superSafeChat(false), userId(0),
	  lastActivityTime(0.0)
{
	Security::Context::current().requirePermission(Security::Administrator, "create a Player");
	setName(sPlayer);
}

} // namespace Network
} // namespace RBX

template class RBX::Reflection::Described<
	RBX::Network::Player,
	RBX::Network::sPlayer,
	RBX::FactoryProduct<RBX::Network::Player, RBX::Instance, RBX::Network::sPlayer> >;
template class RBX::DescribedCreatable<RBX::Network::Player, RBX::Instance, RBX::Network::sPlayer>;
