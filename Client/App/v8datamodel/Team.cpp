#include "v8datamodel/Team.h"

#include "reflection/property.h"

namespace RBX {

const char sTeam[] = "Team";

using namespace Reflection;

static PropDescriptor<Team, int> prop_Score("Score", "Data", &Team::getScore, &Team::setScore);

static PropDescriptor<Team, BrickColor> prop_Color("TeamColor", "Data", &Team::getTeamColor, &Team::setTeamColor);

static PropDescriptor<Team, bool> prop_AutoAssignable(
	"AutoAssignable", "Data", &Team::getAutoAssignable, &Team::setAutoAssignable
);

Reflection::BoundProp<bool, 1> Team::prop_AutoColorCharacters(
	"AutoColorCharacters", "Data", &Team::autoColorCharacters
);

int Team::getScore() const
{
	return score;
}

BrickColor Team::getTeamColor() const
{
	return color;
}

bool Team::getAutoAssignable() const
{
	return autoAssignable;
}

Team::~Team()
{
}

Team::Team() : score(0), autoAssignable(true), autoColorCharacters(true)
{
	setName(sTeam);
	color = BrickColor::legoWhite();
}

void Team::setScore(int score)
{
	this->score = score;
	raisePropertyChanged(prop_Score);
}

void Team::setTeamColor(BrickColor color)
{
	this->color = color;
	raisePropertyChanged(prop_Color);
}

void Team::setAutoAssignable(bool autoAssign)
{
	autoAssignable = autoAssign;
	raisePropertyChanged(prop_AutoAssignable);
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::Team, RBX::Instance, RBX::sTeam>;
template class RBX::Reflection::
	Described<RBX::Team, RBX::sTeam, RBX::FactoryProduct<RBX::Team, RBX::Instance, RBX::sTeam> >;
template class RBX::DescribedCreatable<RBX::Team, RBX::Instance, RBX::sTeam>;
