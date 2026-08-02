#include "v8datamodel/Team.h"

#include "reflection/property.h"

namespace RBX {

const char sTeam[] = "Team";

using namespace Reflection;

static PropDescriptor<Team, int> prop_Score;
static PropDescriptor<Team, BrickColor> prop_Color;
static PropDescriptor<Team, bool> prop_AutoAssignable;

// FUNCTION: WEBSERVICE 0x100be710
void Team::setScore(int value)
{
	score = value;
	raisePropertyChanged(prop_Score);
}

// STUB: WEBSERVICE 0x100be730
void Team::setTeamColor(BrickColor value)
{
	color = value;
	raisePropertyChanged(prop_Color);
}

// FUNCTION: WEBSERVICE 0x100be750
void Team::setAutoAssignable(bool value)
{
	autoAssignable = value;
	raisePropertyChanged(prop_AutoAssignable);
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::Team, RBX::Instance, RBX::sTeam>;
template class RBX::Reflection::
	Described<RBX::Team, RBX::sTeam, RBX::FactoryProduct<RBX::Team, RBX::Instance, RBX::sTeam> >;
template class RBX::DescribedCreatable<RBX::Team, RBX::Instance, RBX::sTeam>;
