#ifndef V8DATAMODEL_TEAMS_H
#define V8DATAMODEL_TEAMS_H

#include "decomp.h"
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
// SIZE 0x100
class Teams : public DescribedCreatable<Teams, Instance, sTeams>, public Service
{
public:
	Teams();
	virtual ~Teams();

	// SYNTHETIC: WEBSERVICE 0x100dab80
	// RBX::Teams::`scalar deleting destructor'

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
	CopyOnWrite<std::vector<boost::shared_ptr<Instance> > > teams; // 0xf8

protected:
	virtual void onChildAdded(Instance* child);
	virtual void onChildRemoving(Instance* child);
};

DECOMP_SIZE_ASSERT(Teams, 0x100)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021c630
// RBX::`dynamic initializer for 'teams_rebalanceTeamsFunction''
// SYNTHETIC: WEBSERVICE 0x1021c660
// RBX::`dynamic initializer for 'func_teams''
// SYNTHETIC: WEBSERVICE 0x10223e80
// RBX::`dynamic atexit destructor for 'func_teams''
// SYNTHETIC: WEBSERVICE 0x10223e90
// RBX::`dynamic atexit destructor for 'teams_rebalanceTeamsFunction''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_TEAMS_H
