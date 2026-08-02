#include "v8datamodel/SpawnLocation.h"

#include "reflection/Property.h"

namespace RBX {

const char sSpawnerService[] = "SpawnerService";

char sSpawnLocation[] = "SpawnLocation";

static Reflection::PropDescriptor<SpawnLocation, BrickColor> prop_TeamColor;

// FUNCTION: WEBSERVICE 0x100ff2b0
void SpawnLocation::setTeamColor(BrickColor value)
{
	teamColor = value;
	raisePropertyChanged(prop_TeamColor);
}

} // namespace RBX
