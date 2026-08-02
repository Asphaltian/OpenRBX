#include "humanoid/Flying.h"

#include "util/Name.h"

namespace RBX {

const char sFlying[] = "Flying";

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sFlying>();
template void RBX::Name::callDoDeclare<RBX::sFlying>();
