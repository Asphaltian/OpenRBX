#include "v8datamodel/FlagStand.h"

#include "v8datamodel/PartInstance.h"

namespace RBX {

const char sFlagStandService[] = "FlagStandService";

char sFlagStand[] = "FlagStand";

// FUNCTION: WEBSERVICE 0x100e8fa0
void FlagStand::setTeamColor(BrickColor value)
{
	teamColor = value;
	raisePropertyChanged(PartInstance::prop_Color);
}

} // namespace RBX
