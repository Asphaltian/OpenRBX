#include "v8datamodel/SpawnLocation.h"

#include "reflection/Property.h"

namespace RBX {

static Reflection::PropDescriptor<SpawnLocation, BrickColor> prop_TeamColor;

// FUNCTION: WEBSERVICE 0x100ff2b0
void SpawnLocation::setTeamColor(BrickColor value)
{
	teamColor = value;
	raisePropertyChanged(prop_TeamColor);
}

} // namespace RBX
