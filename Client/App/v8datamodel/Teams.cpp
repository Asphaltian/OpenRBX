#include "v8datamodel/Teams.h"

#include "humanoid/Humanoid.h"
#include "network/Player.h"
#include "network/Players.h"
#include "reflection/reflection.h"
#include "v8datamodel/ModelInstance.h"
#include "v8datamodel/Team.h"

#include <algorithm>

namespace RBX {

const char sTeams[] = "Teams";

static Reflection::BoundFuncDesc<Teams, void(), 0> teams_rebalanceTeamsFunction(
	&Teams::rebalanceTeams, "RebalanceTeams", Reflection::FunctionDescriptor::AnyCaller
);

static Reflection::BoundFuncDesc<Teams, boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > >(), 0>
	func_teams(&Teams::getTeams, "GetTeams", Reflection::FunctionDescriptor::AnyCaller);

Teams::~Teams()
{
}

Team* Teams::getTeamFromTeamColor(BrickColor c)
{
	for (unsigned int i = 0; i < numChildren(); i++) {
		if (Team* team = dynamic_cast<Team*>(getChild(i))) {
			if (team->getTeamColor() == c) {
				return team;
			}
		}
	}

	return NULL;
}

Team* Teams::getTeamFromPlayer(Network::Player* p)
{
	if (p->getNeutral()) {
		return NULL;
	}

	return getTeamFromTeamColor(p->getTeamColor());
}

boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > Teams::getTeams()
{
	return teams.read();
}

bool Teams::teamExists(BrickColor color)
{
	return getTeamFromTeamColor(color) != NULL;
}

bool Teams::isTeamGame()
{
	Network::Players* players = ServiceProvider::find<Network::Players>(this);

	for (unsigned int i = 0; i < players->numChildren(); i++) {
		if (Network::Player* player = dynamic_cast<Network::Player*>(players->getChild(i))) {
			if (!player->getNeutral()) {
				return true;
			}
		}
	}

	return false;
}

int Teams::getNumPlayersInTeam(BrickColor color)
{
	int count = 0;
	Network::Players* players = ServiceProvider::find<Network::Players>(this);

	for (unsigned int i = 0; i < players->numChildren(); i++) {
		if (Network::Player* player = dynamic_cast<Network::Player*>(players->getChild(i))) {
			if (!player->getNeutral() && player->getTeamColor() == color) {
				count++;
			}
		}
	}

	return count;
}

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

void Teams::assignNewPlayerToTeam(Network::Player* p)
{
	BrickColor best = BrickColor(BrickColor::lego_28);
	int best_count = 10000;
	bool found = false;

	for (unsigned int i = 0; i < numChildren(); i++) {
		if (Team* team = dynamic_cast<Team*>(getChild(i))) {
			if (team->getAutoAssignable() == true) {
				int count = getNumPlayersInTeam(team->getTeamColor());
				if (count < best_count) {
					best = team->getTeamColor();
					best_count = count;
					found = true;
				}
			}
		}
	}

	if (found) {
		p->setTeamColor(best);
		p->setNeutral(false);
	}
}

void Teams::onChildAdded(Instance* child)
{
	if (dynamic_cast<Team*>(child)) {
		boost::shared_ptr<std::vector<boost::shared_ptr<Instance> > >& t = teams.write();
		t->push_back(shared_from(child));
	}
}

void Teams::onChildRemoving(Instance* child)
{
	if (dynamic_cast<Team*>(child)) {
		boost::shared_ptr<std::vector<boost::shared_ptr<Instance> > >& t = teams.write();
		t->erase(std::find(t->begin(), t->end(), shared_from(child)));
	}
}

Teams::Teams() : teams(std::vector<boost::shared_ptr<Instance> >())
{
	setName(sTeams);
}

void Teams::rebalanceTeams()
{
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::Teams, RBX::Instance, RBX::sTeams>;
template class RBX::Reflection::
	Described<RBX::Teams, RBX::sTeams, RBX::FactoryProduct<RBX::Teams, RBX::Instance, RBX::sTeams> >;
template class RBX::DescribedCreatable<RBX::Teams, RBX::Instance, RBX::sTeams>;
