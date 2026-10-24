#include "network/Players.h"

#include "network/Client.h"
#include "network/Server.h"
#include "reflection/property.h"
#include "v8datamodel/ModelInstance.h"

namespace RBX {
namespace Network {

char sPlayers[] = "Players";

static Reflection::PropDescriptor<Players, int> propPlayerMaxCount(
	"MaxPlayers",
	"Data",
	&RBX::Network::Players::getMaxPlayers,
	&RBX::Network::Players::setMaxPlayers
);

// FUNCTION: WEBSERVICE 0x10190c50
bool Players::clientIsPresent(const Instance* context, bool testInDatamodel)
{
	return Client::clientIsPresent(context, testInDatamodel);
}

// FUNCTION: WEBSERVICE 0x10190c60
bool Players::frontendProcessing(const Instance* context, bool testInDatamodel)
{
	const ServiceProvider* serviceProvider = ServiceProvider::findServiceProvider(context);

	return serviceProvider != NULL && !Server::serverIsPresent(context, testInDatamodel);
}

// FUNCTION: WEBSERVICE 0x10190c90
bool Players::backendProcessing(const Instance* context, bool testInDatamodel)
{
	const ServiceProvider* serviceProvider = ServiceProvider::findServiceProvider(context);

	return serviceProvider != NULL && !Client::clientIsPresent(context, testInDatamodel);
}

// STUB: WEBSERVICE 0x101913e0
void Players::onEvent(const Player* source, CharacterAdded event)
{
	STUB(0x101913e0);
}

// FUNCTION: WEBSERVICE 0x101935a0
Player* Players::findLocalPlayer(const Instance* context)
{
	const Players* players = ServiceProvider::find<Players>(context);

	return players ? players->getLocalPlayer() : NULL;
}

// FUNCTION: WEBSERVICE 0x101935c0
RBX::ModelInstance* Players::findLocalCharacter(const Instance* context)
{
	if (Player* player = findLocalPlayer(context)) {
		return player->getCharacter();
	}

	return NULL;
}

// FUNCTION: WEBSERVICE 0x101956e0
void Players::setMaxPlayers(int value)
{
	if (value != maxPlayers) {
		maxPlayers = value;
		raisePropertyChanged(propPlayerMaxCount);
	}
}

} // namespace Network
} // namespace RBX
