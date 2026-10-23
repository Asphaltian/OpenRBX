#include "v8datamodel/Teams.h"

#include "humanoid/Humanoid.h"
#include "network/Player.h"
#include "network/Players.h"
#include "v8datamodel/ModelInstance.h"

namespace RBX {

const char sTeams[] = "Teams";

// FUNCTION: WEBSERVICE 0x100dad30
G3D::Color3 Teams::getTeamColorForHumanoid(Humanoid* h)
{
	Network::Players* players = ServiceProvider::find<Network::Players>(this);

	for (unsigned int i = 0; i < players->numChildren(); i++) {
		if (Network::Player* player = dynamic_cast<Network::Player*>(players->getChild(i))) {
			if (!player->getNeutral()) {
				if (player->getCharacter()) {
					if (player->getCharacter()->findFirstChildOfType<Humanoid>() == h) {
						return player->getTeamColor().color3();
					}
				}
			}
		}
	}

	return G3D::Color3::white();
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::Teams, RBX::Instance, RBX::sTeams>;
template class RBX::Reflection::
	Described<RBX::Teams, RBX::sTeams, RBX::FactoryProduct<RBX::Teams, RBX::Instance, RBX::sTeams> >;
template class RBX::DescribedCreatable<RBX::Teams, RBX::Instance, RBX::sTeams>;
