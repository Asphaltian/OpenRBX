#include "v8datamodel/Hopper.h"

#include "util/Name.h"

namespace RBX {

const char sLegacyHopperService[] = "Hopper";

const char sStarterPackService[] = "StarterPack";

const char sHopperBin[] = "HopperBin";

const char sBackpackItem[] = "BackpackItem";

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sBackpackItem>();
template void RBX::Name::callDoDeclare<RBX::sBackpackItem>();
