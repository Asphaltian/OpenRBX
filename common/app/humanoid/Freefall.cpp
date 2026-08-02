#include "humanoid/Freefall.h"

#include "util/Name.h"

namespace RBX {

const char sFreefall[] = "Freefall";

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sFreefall>();
template void RBX::Name::callDoDeclare<RBX::sFreefall>();
