#ifndef V8DATAMODEL_TEAMS_H
#define V8DATAMODEL_TEAMS_H

#include "placeholder.h"
#include "util/Handle.h"
#include "util/Utilities.h"
#include "v8datamodel/BrickColor.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <G3D/Color3.h>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace RBX {

class Humanoid;
class Team;

namespace Network {
class Player;
}

extern const char sTeams[];
class Teams : public DescribedCreatable<Teams, Instance, sTeams>, public Service
{
public:
	Teams();
	virtual ~Teams();

	void assignNewPlayerToTeam(Network::Player* p);
	int getNumPlayersInTeam(BrickColor color);
	bool teamExists(BrickColor color);
	bool isTeamGame();
	void rebalanceTeams();
	BrickColor getUnusedTeamColor();
	Team* getTeamFromTeamColor(BrickColor c);
	Team* getTeamFromPlayer(Network::Player* p);
	boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > getTeams();
	G3D::Color3 getTeamColorForHumanoid(Humanoid* h);

private:
	CopyOnWrite<std::vector<boost::shared_ptr<Instance> > > teams;

protected:
	virtual void onChildAdded(Instance* child);
	virtual void onChildRemoving(Instance* child);
};

} // namespace RBX

#endif // V8DATAMODEL_TEAMS_H
