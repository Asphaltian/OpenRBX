#include "v8datamodel/FlagStand.h"

#include "v8datamodel/PartInstance.h"

namespace RBX {

const char sFlagStandService[] = "FlagStandService";

char sFlagStand[] = "FlagStand";

// STUB: WEBSERVICE 0x100e81d0
void FlagStand::AffixFlag(Flag* flag)
{
	STUB(0x100e81d0);
}

// STUB: WEBSERVICE 0x100e8270
FlagStand* FlagStandService::FindStandWithFlag(Flag* flag)
{
	STUB(0x100e8270);
	return NULL;
}

// FUNCTION: WEBSERVICE 0x100e83d0
FlagStandService::~FlagStandService()
{
}

// STUB: WEBSERVICE 0x100e84e0
FlagStand* FlagStandService::FindClosestEmptyStandForFlag(Flag* flag)
{
	STUB(0x100e84e0);
	return NULL;
}

// FUNCTION: WEBSERVICE 0x100e8fa0
void FlagStand::setTeamColor(BrickColor color)
{
	teamColor = color;
	raisePropertyChanged(PartInstance::prop_Color);
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::FlagStandService, RBX::Instance, RBX::sFlagStandService>;
template class RBX::Reflection::Described<
	RBX::FlagStandService,
	RBX::sFlagStandService,
	RBX::FactoryProduct<RBX::FlagStandService, RBX::Instance, RBX::sFlagStandService> >;
template class RBX::DescribedCreatable<RBX::FlagStandService, RBX::Instance, RBX::sFlagStandService>;
