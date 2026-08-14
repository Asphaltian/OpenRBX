#include "network/Players.h"

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

// STUB: WEBSERVICE 0x101913e0
void Players::onEvent(const Player* source, CharacterAdded event)
{
	STUB(0x101913e0);
}

// STUB: WEBSERVICE 0x101935c0
RBX::ModelInstance* Players::findLocalCharacter(const Instance* context)
{
	STUB(0x101935c0);
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
